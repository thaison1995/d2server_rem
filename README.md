# D2Server REMastered

This is a work in progress, you are welcome to contribute.

## Goal

To create a remastered D2GS with modern C++, for extensibility and ease of maintenance.
* Compatible with D2CS/D2DBS from [PvPGN](https://github.com/pvpgn/pvpgn-server)
* Improved observability of server/game status
* No precompiled binary except for those binaries from D2

## Current Status
* Some of the basic functionalities are already in place
* Now we can up game server and communicate with D2CS
* Only Game Version 1.13 is supported (no plan to support others, however feel free to contribute)
* Cannot get rid of D2Server.dll completely because doing so would result in improperly initialized Storm.dll (will fix soon)

### TBD
* D2Game callbacks are not properly implemented
* D2DBS client is not properly implemented
* Most patches from marsgod's D2GS are not ported
* Player joining game and gaming experience are not verified
* Admin console (for observability)
* Prometheus metrics exporter (for observaility)

## Credit
* Thanks onlyer/faster and marsgod for his D2Server, made reverse engineering possible for me to figure out how D2Server should work
* Thanks [D2Template project](https://github.com/ThePhrozenKeep/D2Template) for a good template to start working on
* Thanks [D2Ex2 project](https://github.com/jankowskib/D2Ex2) and [PlugY](https://github.com/ChaosMarc/PlugY) for valuable game offsets and function defs