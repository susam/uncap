Changelog
=========

0.3.0 (UNRELEASED)
------------------
### Added 
- Support mapping of injected keyboard events.
- Show `dwExtraInfo`, `nCode`, and `flags` in debug output.
- Show `LLKHF_LOWER_IL_INJECTED` flag as `LOW` in debug output.

### Fixed
- Console window is not hidden when Cmder is running.


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
