Uncap
=====

Map Caps Lock key to Escape key, or any to any key, on Windows systems.

[![Download][Download SVG]][Download URL]
[![MIT License][License Badge]][L]
[![Twitter][Twitter SVG]][Twitter URL]

Apart from documenting how Uncap works on Windows, this README contains
detailed documentation about how to remap keys on Linux and Mac as well.

[Download SVG]: https://img.shields.io/badge/download-uncap.exe-brightgreen.svg
[Download URL]: https://github.com/susam/uncap/releases/download/0.3.0/uncap.exe
[License Badge]: https://img.shields.io/badge/license-MIT-blue.svg
[Twitter SVG]: https://img.shields.io/badge/twitter-%40susam-%231da1f2
[Twitter URL]: https://twitter.com/susam


Contents
--------

* [Features](#features)
* [Get Started](#get-started)
* [Custom Mapping](#custom-mapping)
* [Usage](#usage)
* [Known Issue with Microsoft Edge on Windows 10](#known-issue-with-microsoft-edge-on-windows-10)
* [Alternatives](#alternatives)
  * [Windows: Scancode Map Registry Value](#windows-scancode-map-registry-value)
  * [Windows: AutoHotkey](#windows-autohotkey)
  * [Linux: setxkbmap](#linux-setxkbmap)
  * [Linux: xmodmap](#linux-xmodmap)
  * [Linux: loadkeys](#linux-loadkeys)
  * [Mac: System Preferences](#mac-system-preferences)
  * [Mac: Seil](#mac-seil)
* [Resources](#resources)
* [License](#license)
* [Support](#support)


Features
--------

The following list briefly describes some of the features of Uncap.

 1. Map Caps Lock key to Escape key on a new Windows system with almost
    no effort. Just download and double-click. This is the primary
    reason why Uncap was written. This tool was written for users of
    vi/Vim editor who like to map their Caps Lock key to Escape key for
    convenience.
 2. Map any key to any key, multiple keys to other keys, or disable
    keys using command line arguments. No additional file or script is
    required.
 3. Enable, disable or modify key mappings without having to reboot the
    Windows system.
 4. Disable key mappings easily by stopping Uncap.
 5. It is a single-file executable. It is very lightweight. The
    executable is only about 100 KB in size. It occupies less than 1 MB
    of memory and negligible CPU while running.

There are several other methods and tools available on Windows to map
one key to another. But none of them seem to have all five features
enumerated above. For example, editing [Scancode
Map](#windows-scancode-map-registry-value) registry value requires the
Windows system to be rebooted after every change. On the other hand
[AutoHotkey](#windows-autohotkey) requires an additional script file to
be written. Therefore, Uncap was written to support all five features
described above.


Get Started
------------

Uncap is a single-file executable: [`uncap.exe`][Download URL].

The simplest way to run it is to download it and double-click it. No
output window is displayed. It runs in background. Once it is running,
whenever Caps Lock key is pressed, it is translated to the Escape key by
Uncap.


Custom Mapping
--------------

It is possible to override the default mapping of Caps Lock key to
Escape key by specifying a custom mapping, however to do so, Uncap must
be run with arguments either from Command Prompt or from Windows Run
dialog box. The following steps show how uncap.exe may be used to map
Caps Lock key to Left Control key.

 1. Copy `uncap.exe` to a directory specified in the Windows `PATH`
    variable. `C:\Windows` is a very convenient location to copy this
    file to.

 2. Launch Windows Run dialog box by pressing `Windows Logo Key` + `R`.

 3. Enter the following command.

        uncap 0x14:0xa2

 4. Press OK button. This would launch Uncap. No output window would be
    displayed. Uncap would run in background. Whenever Caps Lock key is
    pressed now, Uncap would translate it to Left Control key.

 5. To terminate Uncap, run the following command.

        uncap -k

In the above steps, Caps Lock key is mapped to Left Control key by
specifying `0x14:0xa2` as an argument to Uncap. The virtual-key code of
Caps Lock is `0x14` and the virtual-key code of Control key is `0xa2`.
Therefore, the argument `0x14:0xa2` maps Caps Lock key to Control key.

The list of virtual-key codes for every key can be found at
<https://msdn.microsoft.com/library/windows/desktop/dd375731.aspx>.


Usage
-----

When Uncap is run without any arguments, it maps Caps Lock key to Escape
key. It may be run with arguments to map Caps Lock key to any key, any
key to any key, or disable any key.

Each argument is a colon separated pair of virtual-key codes from
<https://msdn.microsoft.com/library/windows/desktop/dd375731.aspx>.

The virtual-key code may be specified either as hexadecimal integer as
mentioned in the above URL or its equivalent decimal notation. These
details are illustrated in the list of examples below.

 1. Map Caps Lock key to Escape key.

        uncap

 2. Swap Caps Lock key with Escape key.

        uncap 0x1b:0x14

    Caps Lock key is already mapped to Escape key by default. The above
    command maps Escape key to Caps Lock key, thus effectively swapping
    both keys.

 3. The hexadecimal integer is case-insensitive, so the following
    command is equivalent to the previous command.

        uncap 0x1B:0x14

 4. The virtual-key codes may be specified in decimal notation, so the
    following command is equivalent to the previous command.

        uncap 27:20

 5. Map Caps Lock key to Left Control key.

        uncap 0x14:0xa2

 6. Swap Caps Lock key and Left Control key.

        uncap 0x14:0xa2 0xa2:0x14

    This example shows that it is possible to specify more than one
    argument to map multiple keys to other keys.

 7. Here is another example that maps multiple keys to other keys. The
    following command maps Caps Lock key to Left Control key, Left
    Control key to Escape key and Escape key to Caps Lock key.

        uncap 0x14:0xa2 0xa2:0x1b 0x1b:0x14

 8. If a key is mapped to itself, then no mapping occurs for it. This
    may be used to override default behaviour of mapping Caps Lock key
    to Escape key and leave it unmapped while mapping another key to
    some key. Here is an example that unmaps Caps Lock key and maps F1
    key to escape key.

        uncap 0x14:0x14 0x70:0x1b

    When a key is mapped to another key, and that key is pressed, Uncap
    translates it to the key it is mapped to by synthesizing a new
    keystroke and injecting it into the system. But when a key is
    mapped to itself, and that key is pressed, it is ignored, thus no
    translation or synthesis of a new keystroke occurs.

 9. If a key is mapped to 0, then the key is disabled. Here is an
    example that disables Caps Lock key completely.

        uncap 0x14:0

10. Uncap runs without displaying a console by default. To terminate
    another instance of Uncap that may be running, use the `-k` or
    `--kill` option.

        uncap -k

    The above command kills all other instances of Uncap that are
    running with or without a console.

11. To run Uncap with a console, use the `-c` or `--console` option.

        uncap -c

    When Uncap is running in a console like this, it may be terminated
    by pressing `Ctrl` + `C`.

12. To run Uncap in debug mode, use the `-d` or `--debug` option.

        uncap -d

    The debug output contains one line of output with details about
    every press of a key or release of a key.

13. To log the keystroke details to a file, use the `-f` or `--file`
    option.

        uncap -f C:\keys.txt

    With this option alone, Uncap runs in background and logs the
    keystroke details to the specified file. This option may be
    combined with `-c` or `-d` to run Uncap in a console or print
    debug output on the console, respectively.

14. See the complete usage details along with options supported.

        uncap -h


Known Issue with Microsoft Edge on Windows 10
---------------------------------------------

When Uncap is downloaded using the Microsoft Edge web browser on Windows
10, it sometimes fails to map keys.

The following measures have found to be *unsuccessful* in resolving the
issue:

  - Right clicking `uncap.exe`, then selecting *Properties*, then
    selecting *Unblock*, and then clicking *OK* does not resolve the
    issue.

  - Right clicking `uncap.exe`, then selecting *Properties*, then going
    to *Security* tab, then clicking *Edit*, and then removing the
    following two accounts does not resolve the issue:

        Unknown Account(S-1-15-2-3624051433-2125758914-1423191267-1740899205-1073925389-3782572162-737981194)
        Unknown Account(S-1-15-3-3624051433-2125758914-1423191267-1740899205-1073925389-3782572162-737981194)

    These accounts are added automatically to files downloaded with
    Microsoft Edge.

The following workarounds have been found to resolve the issue
successfully:

  - Copy `uncap.exe` to a new file in the same directory. The new file
    when run remaps keys successfully. Finally, delete the downloaded
    `uncap.exe`, then rename the new file to `uncap.exe`, and use it
    normally.

  - Copy `uncap.exe` to some other directory. The new copied file when
    run remaps keys successfully.

This issue was observed with Microsoft Edge Microsoft Edge 44.17763.1.0
on Windows 10 Pro.


Alternatives
------------

There are other tools and methods available to map one key to another on
Windows, Linux and Mac. This section describes a few such tools and methods.


### Windows: Scancode Map registry value ###

One way to map a key to another key in Windows without using any
additional software is by editing the registry to add a value called
"Scancode Map" in "HKLM\SYSTEM\CurrentControlSet\Control\Keyboard
Layout" registry key.

For example, to map Caps Lock key to Escape key, create a registration
entries file, i.e. a file with reg as its extension, say uncap.reg, with
the following content.

    REGEDIT4
    [HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layout]
    "Scancode Map"=hex:00,00,00,00,00,00,00,00,02,00,00,00,01,00,3a,00,00,00,00,00

Then save the file. Then double-click this file to add it to Windows
registry. Then reboot the Windows system. After rebooting, whenever Caps
Lock key is pressed, it would function like Escape key.

The same registry value may be added by running the following command in
Command Prompt.

    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Keyboard Layout" /v "Scancode Map" /t REG_BINARY /d 00000000000000000200000001003a0000000000

The hexadecimal code in the registry value is the scan code map. The
format of the scan code map is described in detail at
<https://msdn.microsoft.com/library/windows/hardware/jj128267.aspx#scan_code_mapper_for_keyboards>.

The following command may be used to view this registry value.

    reg query "HKLM\SYSTEM\CurrentControlSet\Control\Keyboard Layout" /v "Scancode Map"

The following command may be used to remove this registry value.

    reg delete "HKLM\SYSTEM\CurrentControlSet\Control\Keyboard Layout" /v "Scancode Map"

While this method has an advantage that it requires no additional
software, a disadvantage of this method is that it requires the system
to be rebooted after every change in the registry.

There is a tool called [SharpKeys][] that comes with graphical user
interface to manage this registry value. However a reboot is still
required after every change made with SharpKeys.

[SharpKeys]: https://sharpkeys.codeplex.com/


### Windows: AutoHotkey ###

There is a fairly sophisticated scripting language for Windows called
[AutoHotkey][] that can be used to map one key to another, among many
other things.

It is quite simple to map one key to another using AutoHotkey. For
example, after installing AutoHotkey, to map Caps Lock key to Escape
key, create an AutoHotkey script, i.e. a file with ahk as its extension,
say uncap.ahk, with the following content.

    CapsLock::Esc

Then save the file. Then double-click this file to run AutoHotkey with
this script. Once this is done, whenever Caps Lock key is pressed, it is
translated to Escape key. An AutoHotkey icon appears in the notification
area (system tray). To stop it, right click on the icon and select
'Exit'.

This method has an advantage that it does not require the system to be
rebooted. However, AutoHotkey is a scripting language that can be used
to do a number of other things such as defining hotkeys (shortcut keys),
hotstrings (auto-replace), macros (scripted actions), etc. This may feel
akin to killing a fly with a sledgehammer if one just wants to map a key
to another key.

[AutoHotkey]: http://www.autohotkey.com/


### Linux: setxkbmap ###

On Linux, the `setxkbmap` command may be used to map one key to another
key in the X Window System. For example, the following command maps Caps
Lock key to Escape key.

    setxkbmap -option caps:escape

The list of options along with a short description for each can be found
at /usr/share/X11/xkb/rules in a .lst file with the same name as the
current XKB rule. The current XKB rule as well as the current mapping
options can be found in the output of this command.

    setxkbmap -query

For example, if 'evdev' is displayed as the current rules, then see
/usr/share/X11/xkb/rules/evdev.lst for the list of available options
along with a short description for each option.

In the output of the above command, the line that begins with 'rules:'
in the output of the above command shows the current XKB rules. The key
mapping option is displayed in the line that begins with 'option:' in
the output of the above command. If there is no such line, then no key
mapping option is set.

The following command removes any key mapping option.

    setxkbmap -option


### Linux: xmodmap ###

On Linux, there is another command called `xmodmap` that may be used to
map one key to another in the X Window System. However this command is
now considered obsolescent. It is also a little clumsy to use,
especially while removing a key mapping. The `setxkbmap` command
described in the previous section is the preferred way of mapping keys
in the X Window System. However, this section describes how to use
`xmodmap` to map one key to another because this method is still in use
although its use is gradually fading away. As an example, this section
describes how to map Caps Lock key to Escape key.

Before performing the key mapping, one needs to find the key code of
Caps Lock key in the X Window System. This is useful while reverting the
key mapping. Here is the command to get the key code for Caps Lock key.

    xmodmap -pke | grep Caps_Lock

For PC keyboards, the key code is usually `66`.

The following command maps Caps Lock key to Escape key.

    xmodmap -e "remove Lock = Caps_Lock" -e "keysym Caps_Lock = Escape"

Here is another command that also maps Caps Lock key to Escape key
if the key code for Caps Lock is 66. This command makes use of `keycode`
instead of `keysym`.

    xmodmap -e "remove Lock = Caps_Lock" -e "keycode 66 = Escape"

The following two commands display the current details.

    xmodmap -pm -pke

The following command undoes the mapping.

    xmodmap -e "keycode 66 = Caps_Lock" -e "add Lock = Caps_Lock"

Note that `keycode` is used in the above command. If `keysym Escape =
Caps_Lock` is used instead in the above command, then along with
reverting the Caps Lock key to function like Caps Lock, it would also
change the behaviour of the Escape key to function like Caps Lock. Using
`keycode 66 = Caps_Lock` ensures that only the behaviour of Caps Lock
key only is restored.


### Linux: loadkeys

On Linux, the `loadKeys` command may be used to map one key to another
in console. This section describes how to map Caps Lock key to Escape
key. All commands in this section must be run as root or superuser.

Before performing the key mapping, one needs to find the key code of
Caps Lock key in the console. This is also going to be useful while
reverting the key mapping.

Here is a quick way to find the key code of Caps Lock key in console.

    dumpkeys | grep -E "CtrlL_Lock|Caps_Lock"

In this section, we assume that the key code for Caps Lock key is `58`
and it is mapped to `Caps_Lock` key, which is indeed true on most
systems. On Debian based systems, the key code `58` may be mapped to
`CtrlL_Lock` to work around Debian bug [514464][] and kernel bug
[7746][].

The following command maps Caps Lock key to Escape key.

    (dumpkeys | grep keymaps; echo keycode 58 = Escape) | loadkeys

The following command undoes the mapping.

    (dumpkeys | grep keymaps; echo keycode 58 = Caps_Lock) | loadkeys

[514464]: https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=514464
[7746]: https://bugzilla.kernel.org/show_bug.cgi?id=7746


### Mac: System Preferences

Since macOS Sierra Version 10.12.1, it is easy to map Caps Lock to Escape via
*System Preferences*. Perform the following steps to do so.

 1. Go to the Apple menu > *System Preferences* > *Keyboard*.
 2. Click *Modifier Keys*.
 3. Set *Caps Lock Key* to *Escape*.
 4. Click *OK*.


### Mac: Seil

In OS X, the mapping options available via *System Preferences* are very
limited. For example, it does not allow Caps Lock to be mapped to Escape.
Therefore in general, it becomes necessary to install additional software to
map keys in OS X.

A popular tool for mapping one key to another on OS X is [Seil][].  Here is an
article by my beautiful girlfriend and wife that describes how to map Caps Lock
to Escape on OS X using Seil:
<http://sunainapai.in/blog/map-caps-lock-to-escape-on-mac-os-x/>.

[Seil]: https://pqrs.org/osx/karabiner/seil.html


Resources
---------

Here is a list of useful links about this project.

- [Download][Download URL]
- [Source code](https://github.com/susam/uncap)
- [Issue tracker](https://github.com/susam/uncap/issues)
- [Changelog](https://github.com/susam/uncap/blob/master/CHANGES.md)


License
-------

This is free and open source software. You can use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of it,
under the terms of the MIT License. See [LICENSE.md][L] for details.

This software is provided "AS IS", WITHOUT WARRANTY OF ANY KIND,
express or implied. See [LICENSE.md][L] for details.

[L]: LICENSE.md


Support
-------

To report bugs, suggest improvements, or ask questions, please visit
<https://github.com/susam/uncap/issues>.
