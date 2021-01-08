Changelog
=========

0.4.0 (UNRELEASED)
------------------

### Changed

- Concise debug log format, so that it is easier to read.


0.3.0 (2019-03-24)
------------------

### Added 

- Support mapping of injected keyboard events.
- Show `dwExtraInfo`, `nCode`, and `flags` in debug output.
- Show `LLKHF_LOWER_IL_INJECTED` flag as `LOW` in debug output.

### Fixed

- Issue that prevents Uncap console window from becoming hidden when
  Cmder is running. With this release, Uncap console window disappears
  as expected regardless of whether Cmder is runing or not.


0.2.2 (2015-02-15)
------------------

### Fixed

- Possible buffer overflows while handling command line arguments.


0.2.1 (2015-01-15)
------------------

### Changed

- Minor changes in the help and version texts.


0.2.0 (2015-12-22)
------------------

### Added

- Write verbose logs to file with `-f` or `--file` option.

### Changed

- Switch from BSD 2-Clause License to MIT License.


0.1.0 (2015-09-13)
------------------

### Added

- Map Caps Lock key to Escape key by default.
- Map any key to any key with `MAP_KEY:TO_KEY` argument.
- No mapping occurs with `MAP_KEY:MAP_KEY` argument.
- Disable any key with `MAP_KEY:0` argument.
- Run in background without console by default.
- Kill other instances of the program with `-k` or `--kill` option.
- Run silently in console with `-c` or `--console` option.
- Run verbosely in console with `-d` or `--debug` option.
