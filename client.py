import socket

PORT = 8080
BUFFER_SIZE = 1024

def main():
    server_ip = input("Enter server IP address (or press Enter for localhost): ").strip()
    if not server_ip:
        server_ip = "127.0.0.1"
    message = input("Enter message to send to server: ").strip()
    if not message:
        print("No message entered. Exiting.")
        return

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        try:
            print(f"Attempting to connect to {server_ip}:{PORT}...")
            sock.connect((server_ip, PORT))
            print(f"Connected successfully to {server_ip}:{PORT}")
            sock.sendall(message.encode('utf-8'))
            print(f"Message sent: {message}")
            response = sock.recv(BUFFER_SIZE)
            if response:
                print("Response received:")
                print(response.decode('utf-8'))
            else:
                print("No response received or error occurred")
        except Exception as e:
            print(f"Connection Failed to {server_ip}:{PORT}")
            print("Make sure the server is running and the IP address is correct")
            print(f"Error: {e}")

if __name__ == "__main__":
    main()
