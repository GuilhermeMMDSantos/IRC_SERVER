*This project has been created as part of the 42 curriculum by nfigueir, gudos-sa, mefernan.*

## Description
This project is about creating a functional Internet Relay Chat (IRC) server in C++98. The server is capable of handling multiple clients simultaneously without hanging, utilizing non-blocking I/O operations and a single multiplexing function like `poll()` to manage all connections.

The server supports standard IRC client features, including authentication, setting a nickname and username, joining channels, and sending/receiving private messages. It also implements specific channel operator commands such as KICK, INVITE, TOPIC, and MODE (i, t, k, o, l).

## Instructions

### Compilation
To compile the project, a `Makefile` is provided. Simply run the following command at the root of the repository:
```bash
make
```
### Execution
The executable requires two arguments to run: the listening port and the connection password.
```bash
./ircserv <port> <password>
```
* **port:** The port number on which the server will listen for incoming IRC connections.
* **password:** The connection password required for any IRC client attempting to connect.

### Connecting a Client
You can test the server using any standard IRC client. For example, to connect using a client like irssi, configure the connection to point to `localhost` (or your local IP) on the specified `<port>`, and provide the `<password>` you set during execution.

## Resources
* [RFC 1459 - Internet Relay Chat](https://datatracker.ietf.org/doc/html/rfc1459)
* [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)

## AI Usage

During the development of this project, AI tools were used primarily for:
* Understanding the structure and requirements of the IRC RFCs.
* Used to generate the initial structure and content of this `README.md` file, ensuring it complies with the specific formatting requirements of the ft_irc project.
* No core logic or direct code was copy-pasted from AI tools; all implementations were written manually and reviewed to ensure full understanding.
---
