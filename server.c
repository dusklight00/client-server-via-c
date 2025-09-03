#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080

int main()
{
#ifdef _WIN32
  WSADATA wsaData;
  SOCKET server_fd, new_socket;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
  {
    printf("WSAStartup failed\n");
    return -1;
  }
#else
  int server_fd, new_socket;
#endif

  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  const char *ack_response = "Message received successfully!";

  // Create socket file descriptor
#ifdef _WIN32
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
#else
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
#endif
  {
    perror("socket failed");
#ifdef _WIN32
    WSACleanup();
#endif
    exit(EXIT_FAILURE);
  }

  // Bind to port
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; // Listen on all network interfaces
  address.sin_port = htons(PORT);

  // Enable socket reuse to avoid "Address already in use" errors
#ifdef _WIN32
  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
  {
    perror("setsockopt failed");
  }
#else
  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
  {
    perror("setsockopt failed");
  }
#endif

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
#ifdef _WIN32
    closesocket(server_fd);
    WSACleanup();
#else
    close(server_fd);
#endif
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0)
  {
    perror("listen");
#ifdef _WIN32
    closesocket(server_fd);
    WSACleanup();
#else
    close(server_fd);
#endif
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", PORT);
  printf("Server is accessible from any device on the network\n");
  printf("Clients can connect using this machine's IP address\n");
  printf("To find your IP address, run 'ipconfig' on Windows or 'ifconfig' on Linux/Mac\n");
  printf("Waiting for connections...\n\n");

  while (1)
  {
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
      perror("accept");
      continue;
    }

    // Print client connection info
    char *client_ip = inet_ntoa(address.sin_addr);
    printf("Connection from: %s:%d\n", client_ip, ntohs(address.sin_port));

#ifdef _WIN32
    int valread = recv(new_socket, buffer, sizeof(buffer) - 1, 0);
#else
    int valread = read(new_socket, buffer, sizeof(buffer) - 1);
#endif

    if (valread > 0)
    {
      buffer[valread] = '\0';
      printf("Message received from client: %s\n", buffer);

      // Send acknowledgment back to client
      send(new_socket, ack_response, strlen(ack_response), 0);
    }
    else
    {
      printf("No data received or connection error\n");
    }
#ifdef _WIN32
    closesocket(new_socket);
#else
    close(new_socket);
#endif
  }

#ifdef _WIN32
  closesocket(server_fd);
  WSACleanup();
#else
  close(server_fd);
#endif
  return 0;
}