Changelog
=========

0.2.2 (2015-02-15)
------------------
- FIX: Possible buffer overflows while handling command line arguments.

0.2.1 (2015-01-15)
------------------
- MOD: Minor changes in the help and version texts.

0.2.0 (2015-12-22)
------------------
- NEW: Write verbose logs to file with `-f` or `--file` option.
- MOD: Switch from BSD 2-Clause License to MIT License.

0.1.0 (2015-09-13)
------------------
- NEW: Map Caps Lock key to Escape key by default.
- NEW: Map any key to any key with `MAP_KEY:TO_KEY` argument.
- NEW: No mapping occurs with `MAP_KEY:MAP_KEY` argument.
- NEW: Disable any key with `MAP_KEY:0` argument.
- NEW: Run in background without console by default.
- NEW: Kill other instances of the program with `-k` or `--kill` option.
- NEW: Run silently in console with `-c` or `--console` option.
- NEW: Run verbosely in console with `-d` or `--debug` option.
