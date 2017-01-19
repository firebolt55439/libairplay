# airplay-lib

A C++ library to stream photos and videos via Airplay.

- Can be broadcasted to any Airplay-compatible device in the LAN via Bonjour
- Lightweight and readable code that allows for quick discovery of Airplay-compatible hosts,
including but not limited to Apple TV's.
- Straightforward options and actions (airplay-hangman repository uses this).

Be advised that this project makes use of system calls only available on macOS, and
will require some tweaking to make it compatible with Linux-based OS's.
