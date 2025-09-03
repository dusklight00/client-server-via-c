import socket

PORT = 8080
BUFFER_SIZE = 1024

# Create a TCP/IP socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind(("0.0.0.0", PORT))
    server_socket.listen(3)
    print(f"Server listening on port {PORT}")
    print("Server is accessible from any device on the network")
    print("Clients can connect using this machine's IP address")
    print("To find your IP address, run 'ipconfig' on Windows or 'ifconfig' on Linux/Mac")
    print("Waiting for connections...\n")

    while True:
        client_socket, client_address = server_socket.accept()
        with client_socket:
            print(f"Connection from: {client_address[0]}:{client_address[1]}")
            data = client_socket.recv(BUFFER_SIZE)
            if data:
                message = data.decode('utf-8')
                print(f"Message received from client: {message}")
                ack_response = "Message received successfully!"
                client_socket.sendall(ack_response.encode('utf-8'))
            else:
                print("No data received or connection error")
