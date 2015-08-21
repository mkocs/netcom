# netcom

# About
Netcom attempts to be an easy-to-use in-network communication solution for UNIX (primary target Linux). The idea came up when I was testing Ethernet ports with a colleague and we had no way to easily communicate, sitting in different rooms. It's also a great way for me to expand my C knowledge.

# Usage
  - 'make' will compile netcom
  - 'make clean' cleans the directory (removes build/ and the binary)
  - './netcom -c <address>' runs the client part with the selected host address
  - './netcom -s' runs the server part on port 7891
  - './netcom -v' './netcom --version' shows the current version number
  - './netcom -h' shows the help dialog
