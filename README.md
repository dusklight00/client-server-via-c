# Packet Check: Simple TCP Client-Server in C

This project demonstrates a simple TCP client-server communication in C, compatible with both Windows and Linux. The server listens for incoming connections and acknowledges messages sent by the client.

## Features
- Cross-platform support (Windows and Linux)
- Simple TCP socket communication
- User-friendly prompts for IP and message
- Server acknowledges each client message

## Files
- `server.c`: TCP server implementation
- `client.c`: TCP client implementation
- `server.exe`, `client.exe`: Pre-built Windows executables (if present)

## How It Works

### Server
1. Listens on port 8080 for incoming TCP connections.
2. Accepts connections from any device on the network.
3. Prints the client's IP and port when a connection is made.
4. Receives a message from the client and prints it.
5. Sends an acknowledgment back to the client.
6. Handles multiple clients sequentially (one at a time).

### Client
1. Prompts the user for the server's IP address (defaults to localhost if blank).
2. Prompts the user for a message to send.
3. Connects to the server on port 8080.
4. Sends the message and waits for a response.
5. Prints the server's response.

## How to Build

### Windows (using MinGW or similar)
```
gcc server.c -o server.exe -lws2_32
gcc client.c -o client.exe -lws2_32
```

### Linux / macOS
```
gcc server.c -o server
gcc client.c -o client
```

## How to Run

1. **Start the server:**
	- On Windows: `./server.exe`
	- On Linux/macOS: `./server`

2. **Start the client (on the same or another machine):**
	- On Windows: `./client.exe`
	- On Linux/macOS: `./client`

3. **Follow the prompts in the client:**
	- Enter the server's IP address (or press Enter for localhost).
	- Enter the message to send.

4. **Observe the server's output:**
	- The server will display the client's IP, port, and message.
	- The client will display the server's acknowledgment.

## Notes
- Ensure the server is running before starting the client.
- Both programs use port 8080 by default. Change the `PORT` macro in both files if needed.
- On Windows, you may need to allow the programs through your firewall.
- To find your IP address:
  - Windows: Run `ipconfig` in Command Prompt.
  - Linux/macOS: Run `ifconfig` or `ip a` in Terminal.

## License
This project is provided for educational purposes.
