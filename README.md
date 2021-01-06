# D2Server REMastered

## Goal

To create a remastered D2GS with modern C++, for extensibility and ease of maintenance.
* Compatible with D2CS/D2DBS from [PvPGN](https://github.com/pvpgn/pvpgn-server)
* Improved observability of server/game status
* No precompiled binary except for those binaries from D2

## Current Status
* Some of the basic functionalities are already in place
* Now we can up game server and communicate with D2CS

### TBD
* D2Game callbacks are not properly implemented
* D2DBS client is not properly implemented
* Most patches from marsgod's D2GS are not ported
* Player joining game and gaming experience are not verified
* Admin console (for observability)