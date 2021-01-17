# D2Server REMastered

This is a work in progress, you are welcome to contribute.
For now this server can work with basic gaming experience.

## Goal

To create a remastered D2GS with modern C++, for extensibility and ease of maintenance.
* Compatible with D2CS/D2DBS from [PvPGN](https://github.com/pvpgn/pvpgn-server)
* Improved observability of server/game status
* No precompiled binary except for those binaries from D2

## Current Status
* All basic functionalities have been implemnted and verified
* Only Game Version 1.13 is supported (no plan to support others, however feel free to contribute)
* Cannot get rid of D2Server.dll completely because doing so would result in improperly initialized Storm.dll (will fix soon)

### TBD
* Admin console (for observability)
* Prometheus metrics exporter (for observaility)

## Build Instructions
* CMake 3.14 or higher version is required
* VS2019 or higher version is required
* Step 1: Run cmake_vs2019.bat
* Step 2: Build generated vs2019 solution: build/d2server_rem.sln


## Credit
* Thanks onlyer/faster and marsgod for his D2Server, made reverse engineering possible for me to figure out how D2Server should work
* Thanks [D2Template project](https://github.com/ThePhrozenKeep/D2Template) for a good template to start working on
* Thanks [D2Ex2 project](https://github.com/jankowskib/D2Ex2) and [PlugY](https://github.com/ChaosMarc/PlugY) for valuable game offsets and function defs