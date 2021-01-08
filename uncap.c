/**
@file
@mainpage

Map Caps Lock key to Escape key, or any key to any key.

The MIT License (MIT)
---------------------
Copyright (c) 2015-2021 Susam Pal

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <tlhelp32.h>


/** Version of the program. */
#define VERSION "0.4.0-dev"

/** Author of the program. */
#define AUTHOR "Susam Pal"

/** Copyright notice. */
#define COPYRIGHT "Copyright (c) 2015-2021 " AUTHOR

/** URL to a copy of the license. */
#define LICENSE_URL "<https://susam.github.io/licenses/mit.html>"

/** URL to report issues to. */
#define SUPPORT_URL "<https://github.com/susam/uncap/issues>"

/** Maximum length of command line argument that is read. */
#define MAX_ARG_LEN 16

/** Maximum allowed length of an error message. */
#define MAX_ERR_LEN 256

/** Magic number to identify keyboard input injected by Uncap. */
#define UNCAP_INFO (WM_APP + 3195) /* 35963 */

#ifndef LLKHF_LOWER_IL_INJECTED
/** Workaround for missing definition in MinGW winuser.h. **/
#define LLKHF_LOWER_IL_INJECTED 0x00000002
#endif

/**
Check if two null-terminated byte strings are equal.

@param a Pointer to null-terminated byte string. (type: const char *)
@param b Pointer to null-terminated byte string. (type: const char *)

@return 1 if the two strings are equal; 0 otherwise.
*/
#define streq(a, b) (strcmp(a, b) == 0)


/**
Copy null-terminated byte string into a character array.

@param a Pointer to character array to copy the string to. (type: char *)
@param b Pointer to null-terminated byte string to copy. (type: const char *)
@param c Maximum number of characters to copy. (type: size_t)

@return a
*/
#define strcp(a, b, c) (a[0] = '\0', strncat(a, b, c - 1))


/**
Convert all characters of a string to lowercase.

@param s Pointer to null-terminated byte string.

@return s
*/
char *strlower(char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++)
        s[i] = (char) tolower(s[i]);
    return s;
}


/**
Return name of the leaf directory or file in the specified path.

Both backslash and forward slash are treated as path separators. A
pointer to the beginning of the substring between the last slash
(exclusive) and the end of the string is returned as the basename.
Therefore if the specified path ends with a slash then an empty string
is returned.

@param path Path string.

@return Name of the leaf directory or file in the specified path.
*/
const char *basename(const char *path)
{
    const char *base;
    if ((base = strrchr(path, '\\')) != NULL)
        return base + 1;
    else if ((base = strrchr(path, '/')) != NULL)
        return base + 1;
    else
        return path;
}


/**
Values returned by a function to indicate success or failure.

These return codes may be returned by a function to indicate success or
failure of its operation as well as the next course of action.
*/
enum action {
    GOOD, /**< Successful operation; program should continue. */
    EXIT, /**< Successful operation; program should exit normally. */
    FAIL  /**< Failed operation; program should exit with error. */
};


/**
Global state of this program.
*/
struct state {
    char name[MAX_ARG_LEN];  /**< Program name. */
    WORD keymap[256];        /**< Key mappings. */
    HHOOK hook;              /**< Handle to keyboard hook procedure. */
    int console;             /**< Whether console mode is enabled. */
    int debug;               /**< Whether verbose mode is enabled. */
    FILE *file;              /**< File to write verbose logs to. */
    char error[MAX_ERR_LEN]; /**< Error message for failed operation. */
} my; /**< Global state of this program. */


/**
Output error message on the standard error stream.

@param format Format string for printf.
@param ...    Additional arguments.

@return EXIT_FAILURE; the caller of this function may return this code
        to indicate abnormal termination of the program.
*/
int error(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    fprintf(stderr, "%s: ", my.name);
    vfprintf(stderr, format, ap);
    return EXIT_FAILURE;
}


/** Log format string. */
#define LOG_FMT \
    "%-10s %3d %5lu %3lu " \
    "%3lu (%#04lx) %3lu (%#04lx) " \
    "[%s%s%s%s%s%s%s]\n"


/**
Log details of a key stroke to a specified file.

@param file File to write to. (type: FILE *)
*/
#define logKeyTo(file) \
            fprintf(file, LOG_FMT, \
                    wParamStr, nCode, p->dwExtraInfo, p->flags, \
                    p->scanCode, p->scanCode, p->vkCode, p->vkCode, \
                    vkStr, upStr, extStr, altStr, lowStr, injStr, uncapStr)

/**
Log details of a key stroke.

@param nCode  Code used to determine how to process the message.
@param wParam Identifier of the keyboard message.
@param lParam Pointer to KBDLLHOOKSTRUCT structure.
*/
void logKey(int nCode, WPARAM wParam, LPARAM lParam)
{
    const KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *) lParam;

    char wParamStr[16];
    char vkStr[16];
    char extStr[16];
    char lowStr[16];
    char injStr[16];
    char altStr[16];
    char upStr[16];
    char uncapStr[16];

    /* Translate identifier of keyboard message to string notation. */
    switch (wParam) {
    case WM_KEYDOWN:
        strcpy(wParamStr, "KEYDOWN");
        break;

    case WM_KEYUP:
        strcpy(wParamStr, "KEYUP");
        break;

    case WM_SYSKEYDOWN:
        strcpy(wParamStr, "SYSKEYDOWN");
        break;

    case WM_SYSKEYUP:
        strcpy(wParamStr, "SYSKEYUP");
        break;
    }

    /* Translate virtual-key code to string notation. */
    if (p->vkCode == VK_RETURN)
        sprintf(vkStr, "RETURN");
    else if (p->vkCode == VK_CAPITAL)
        sprintf(vkStr, "CAPITAL");
    else if (p->vkCode == VK_ESCAPE)
        sprintf(vkStr, "ESCAPE");
    else if (p->vkCode == VK_LCONTROL)
        sprintf(vkStr, "LCONTROL");
    else if (p->vkCode == VK_RCONTROL)
        sprintf(vkStr, "RCONTROL");
    else if (p->vkCode == VK_LMENU)
        sprintf(vkStr, "LMENU");
    else if (p->vkCode == VK_RMENU)
        sprintf(vkStr, "RMENU");
    else if (p->vkCode == VK_LWIN)
        sprintf(vkStr, "LWIN");
    else if (p->vkCode == VK_RWIN)
        sprintf(vkStr, "RWIN");
    else if ((p->vkCode >= '0' && p->vkCode <= '9') ||
             (p->vkCode >= 'A' && p->vkCode <= 'Z'))
        sprintf(vkStr, "%c", p->vkCode);
    else if (p->vkCode >= VK_NUMPAD0 && p->vkCode <= VK_NUMPAD9)
        sprintf(vkStr, "NUMPAD%d", p->vkCode - VK_NUMPAD0);
    else if (p->vkCode >= VK_F1 && p->vkCode <= VK_F24)
        sprintf(vkStr, "F%d", p->vkCode - VK_F1 + 1);
    else
        sprintf(vkStr, "%#x", p->vkCode);

    /* Translate each flag to string notation. */
    strcpy(extStr, p->flags & LLKHF_EXTENDED ? " EXT" : "");
    strcpy(lowStr, p->flags & LLKHF_LOWER_IL_INJECTED ? " LOW": "");
    strcpy(injStr, p->flags & LLKHF_INJECTED ? " INJ" : "");
    strcpy(altStr, p->flags & LLKHF_ALTDOWN ? " ALT" : "");
    strcpy(upStr, p->flags & LLKHF_UP ? " UP" : " DN");
    strcpy(uncapStr, p->dwExtraInfo == UNCAP_INFO ? " UNCAP" : "");

    /* Log key to standard error stream if verbose mode is enabled. */
    if (my.debug)
        logKeyTo(stderr);

    /* Log key to user specified file if file logging is enabled. */
    if (my.file != NULL)
        logKeyTo(my.file);

    fflush(NULL);
}


/**
Map one key to another key.

@param nCode  Code used to determine how to process the message.
@param wParam Identifier of the keyboard message.
@param lParam Pointer to KBDLLHOOKSTRUCT structure.

@return 1 if the keyboard message is processed, i.e. a key is mapped to
        another key, otherwise call CallNextHookEx and return the value
        it returns.
*/
LRESULT CALLBACK keyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *) lParam;
    WORD keyCode = (WORD) p->vkCode;
    WORD mapCode = my.keymap[keyCode];

    if (my.debug || my.file) {
        logKey(nCode, wParam, lParam);
    }

    if (mapCode == 0) {
        /* If key pressed is unmapped, disable the key press. */
        return 1;
    } else if (keyCode != mapCode && p->dwExtraInfo != UNCAP_INFO &&
               nCode >= 0) {
        /* If key is mapped, translate it to what it is mapped to. */
        INPUT inputs[1];
        PKEYBDINPUT ki = &inputs[0].ki;

        inputs[0].type = INPUT_KEYBOARD;
        ki->time = ki->wScan = 0;

        ki->wVk = mapCode;
        ki->dwFlags = (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
                      ? KEYEVENTF_KEYUP : 0;
        ki->dwExtraInfo = UNCAP_INFO;

        SendInput(1, inputs, sizeof *inputs);
        return 1;
    }

    return CallNextHookEx(my.hook, nCode, wParam, lParam);
}


/**
Kill other running instances of this program.

@return Next action to take based on whether this function could
        terminate other running instances of this program or not. FAIL
        is returned if this function failed to terminate at least one
        other running instance of this program. EXIT is returned if this
        function successfully terminated all other running instances of
        this program or if there were no other running instances of this
        program.
*/
enum action kill(void)
{
    char myExeFile[MAX_ARG_LEN + 4];
    char *dot;
    PROCESSENTRY32 entry;
    HANDLE snapshotHandle;
    int failure = 0;

    /* Take a snapshot of all processes running on the system. */
    snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshotHandle == NULL) {
        sprintf(my.error, "Cannot take snapshot of processes; ",
                          "error: %lu.", GetLastError());
        return FAIL;
    }

    /* Begin iterating through each process in the snapshot. */
    entry.dwSize = sizeof entry;
    if (!Process32First(snapshotHandle, &entry)) {
        sprintf(my.error, "Cannot retrieve process from snapshot; ",
                          "error: %lu.", GetLastError());
        return FAIL;
    }

    /* Name of the process to be killed. */
    strcpy(myExeFile, my.name);
    strlower(myExeFile);
    dot = strrchr(myExeFile, '.');
    if (dot == NULL || !streq(dot, ".exe"))
        strcat(myExeFile, ".exe");

    /* Iterate through each process in the snapshot, find the process to
       be killed and kill it. */
    do {
        HANDLE processHandle;

        /* Ignore current process. */
        if (entry.th32ProcessID == GetCurrentProcessId())
            continue;

        /* Ignore other programs. */
        if (!streq(strlower(entry.szExeFile), myExeFile))
            continue;

        /* Open another instance of this program. */
        processHandle = OpenProcess(PROCESS_ALL_ACCESS, 0,
                                    entry.th32ProcessID);
        if (processHandle == NULL) {
            error("Cannot open process \"%s\" (PID %lu); error %lu.",
                  entry.szExeFile, entry.th32ProcessID, GetLastError());
            failure = 1;
            continue;
        }

        /* Terminate another instance of this program. */
        if (TerminateProcess(processHandle, 0)) {
            printf("Terminated %s (PID %lu).\n",
                   entry.szExeFile, entry.th32ProcessID, GetLastError());
        } else {
            error("Cannot terminate %s (PID %lu); error %lu.\n",
                  entry.szExeFile, entry.th32ProcessID, GetLastError());
            failure = 1;
        }
        CloseHandle(processHandle);

    } while (Process32Next(snapshotHandle, &entry));

    CloseHandle(snapshotHandle);

    if (failure) {
        sprintf(my.error, "Failed to terminate all running instances "
                          "of %s.", myExeFile);
        return FAIL;
    }

    return EXIT;
}


/**
Show usage and help details of this program.
*/
void showHelp(void)
{
    const char *usage =
"Usage: %s [-k] [-c] [-d] [-f FILE] [-h] [-v] [[MAP_KEY:TO_KEY]...]\n\n";

    const char *summary =
"Map Caps Lock key to Escape key, or any key to any key.\n\n";

    const char *description1 =
"Caps Lock key is mapped to Escape key by default. This may be\n"
"overridden by specifying a new mapping for Caps Lock key. Any key\n"
"may be mapped to any key with one or more MAP_KEY:TO_KEY arguments.\n"
"Each argument is a colon separated pair of virtual-key codes from\n"
"<https://msdn.microsoft.com/library/windows/desktop/dd375731.aspx>.\n\n"

"The virtual-key code may be specified either as hexadecimal integer\n"
"as mentioned in the above URL or its equivalent decimal notation.\n\n";

    const char *description2 =
"If MAP_KEY equals TO_KEY, then no mapping occurs for it. If TO_KEY\n"
"equals 0, then the key mapped to 0 is disabled.\n\n";

    const char *details =
"Options:\n"
"  -k, --kill       Kill other instances of uncap.\n"
"  -c, --console    Run silently in console.\n"
"  -d, --debug      Run verbosely in console.\n"
"  -f, --file FILE  Write verbose logs to file.\n"
"  -h, --help       Show this help and exit.\n"
"  -v, --version    Show version and exit.\n\n"

"Arguments:\n"
"  MAP_KEY          Virtual-key code of key to map.\n"
"  TO_KEY           Virtual-key code of key to map to.\n\n"

"Report bugs to " SUPPORT_URL ".\n";

    printf(usage, my.name);
    printf(summary);
    printf(description1);
    printf(description2);
    printf(details);
}


/**
Show version and copyright details of this program.
*/
void showVersion(void)
{
    char name[MAX_ARG_LEN];

    const char *s =
    "%s " VERSION "\n"
    COPYRIGHT "\n\n"

    "This is free and open source software. You can use, copy, modify,\n"
    "merge, publish, distribute, sublicense, and/or sell copies of it,\n"
    "under the terms of the MIT License. You can obtain a copy of the\n"
    "MIT License at " LICENSE_URL ".\n\n"

    "This software is provided \"AS IS\", WITHOUT WARRANTY OF ANY KIND,\n"
    "express or implied. See the MIT License for details.\n";

    strcpy(name, my.name);
    name[0] = (char) toupper(name[0]);
    printf(s, name);
}


/**
Output love.
*/
void qtpi(void)
{
    const int show[] = {'.', '@', ' ', '\n'};
    const int love[] = {
        252, 7, 252, 7, 72, 6, 29, 6, 36, 6, 29,
        6,  72, 7, 60, 6,  17, 22, 17, 6, 12, 6,

              17, 22,           17,  6,
           60, 7, 52, 6,     13,  46,  13,
         6, 13, 46, 13, 6, 52, 7, 48, 6, 13,
        62, 5, 62, 13,  6, 48,  7, 48, 6, 13,
        18, 1, 70, 13,  6, 48,  7, 48, 6, 13,
        126, 13, 6, 48, 7, 52, 6, 13,  38, 2,
         38, 13, 6,  52, 7,  60, 6,  17, 94,
           17, 6,  60, 7,  72, 6,  17, 22,
              3, 18,  17, 6, 72, 7, 84,
                 6, 17,  46,  17, 6,
                    84, 7, 96, 6,
                       17, 22,
                         17,

        6, 96, 7, 108, 6,  13, 6, 13, 6, 108, 7,
        120,  6, 5,  6, 120, 7,  252, 7, 252, 7,
    };
    const char *say[] = {"", "Cutie Pai,", "I love you!", "-- Susam"};
    size_t i;
    int j;
    for (i = 0; i < sizeof love / sizeof *love; i++) {
        if (love[i] < 4)
            printf(say[love[i]]);
        else
            for (j = 0; j < love[i] / 4; j++)
                putchar(show[love[i] % 4]);
    }
}


/**
Parse command line arguments.

@param argc Argument count
@param argv Argument vector

@return Next action to take based on whether parsing of command line
        arguments was successful or not. GOOD is returned if command
        line arguments were parsed successfully and the program should
        continue. EXIT is returned if command line arguments were parsed
        and processed successfully and the program should exit. FAIL is
        returned if an error was found in command line arguments.
*/
enum action parseArguments(int argc, const char **argv)
{
    int i;

    /* Set default values. */
    strcp(my.name, basename(argv[0]), sizeof my.name);

    for (i = 0; i < (int) (sizeof my.keymap / sizeof *my.keymap); i++)
        my.keymap[i] = (WORD) i;

    my.keymap[VK_CAPITAL] = VK_ESCAPE;

    my.console = 0;
    my.debug = 0;
    my.file = NULL;

    /* Parse command line options. */
    i = 1;
    while (i < argc) {
        const char *arg = argv[i];
        if (streq(arg, "-h") || streq(arg, "--help")) {
            showHelp();
            return EXIT;
        } else if (streq(arg, "-v") || streq(arg, "--version")) {
            showVersion();
            return EXIT;
        } else if (streq(arg, "--qtpi")) {
            qtpi();
            return EXIT;
        } else if (streq(arg, "-c") || streq(arg, "--console")) {
            my.console = 1;
            ++i;
        } else if (streq(arg, "-d") || streq(arg, "--debug")) {
            my.debug = 1;
            ++i;
        } else if (streq(arg, "-f") || streq(arg, "--file")) {
            if (i == argc - 1) {
                sprintf(my.error, "Option '%.*s' must be followed by "
                                  "file path", MAX_ARG_LEN, arg);
                return FAIL;
            }
            arg = argv[++i];
            if ((my.file = fopen(arg, "a")) == NULL) {
                sprintf(my.error, "Cannot open %.*s.", MAX_ARG_LEN, arg);
                return FAIL;
            }
            ++i;
        } else if (streq(arg, "-k") || streq(arg, "--kill")) {
            return kill();
        } else if (streq(arg, "--")) {
            ++i;
            break;
        } else if (arg[0] == '-' && arg[1] != 0) {
            sprintf(my.error, "Unknown option '%.*s'.", MAX_ARG_LEN, arg);
            return FAIL;
        } else {
            break;
        }
    }

    /* Parse command line options. */
    while (i < argc) {
        const char *arg = argv[i];
        char *colon;
        unsigned long int mapKey;
        unsigned long int toKey;

        if ((colon = strchr(arg, ':')) == NULL) {
            sprintf(my.error, "Colon is missing from argument '%.*s'.",
                    MAX_ARG_LEN, arg);
            return FAIL;
        }

        mapKey = strtoul(arg, NULL, 0);
        toKey = strtoul(colon + 1, NULL, 0);

        if (mapKey < 1 || mapKey > 254 || toKey > 254) {
            sprintf(my.error, "Invalid key code in argument '%.*s'.",
                    MAX_ARG_LEN, arg);
            return FAIL;
        }

        my.keymap[mapKey] = (WORD) toKey;
        ++i;
    }

    /* Command line arguments parsed successfully. */
    return GOOD;
}


/**
Start the program.

@param argc Argument count.
@param argv Argument vector.

@return EXIT_SUCCESS if the program terminates normally;
        EXIT_FAILURE if an error occurs.
*/
int main(int argc, char **argv)
{
    enum action a;
    MSG msg;

    /* Parse command line arguments. */
    if ((a = parseArguments(argc, (const char **) argv)) == FAIL)
        return error(my.error);
    else if (a == EXIT)
        return EXIT_SUCCESS;

    /* Set visibility of console. */
    if (!my.console && !my.debug) {
        HWND h = GetConsoleWindow();
        if (h != NULL)
            ShowWindow(h, SW_HIDE);
        else
            error("Cannot find console window; error %lu.", GetLastError());
    }

    /* Install hook to monitor low-level keyboard input events. */
    my.hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHook, NULL, 0);
    if (my.hook == NULL)
        return error("Cannot install hook; error %lu.", GetLastError());

    /* Begin message delivery. */
    if (GetMessage(&msg, NULL, 0, 0) == -1)
        return error("Cannot retrieve message; error %lu.", GetLastError());

    return EXIT_SUCCESS;
}
