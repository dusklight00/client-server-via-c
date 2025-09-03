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
#define BUFFER_SIZE 1024

int main()
{
#ifdef _WIN32
  WSADATA wsaData;
  SOCKET sock;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
  {
    printf("WSAStartup failed\n");
    return -1;
  }
#else
  int sock;
#endif

  struct sockaddr_in serv_addr;
  char buffer[BUFFER_SIZE] = {0};
  char message[BUFFER_SIZE] = {0};
  char server_ip[50] = {0};

  // Get server IP address from user
  printf("Enter server IP address (or press Enter for localhost): ");
  fgets(server_ip, sizeof(server_ip), stdin);

  // Remove newline character if present
  size_t ip_len = strlen(server_ip);
  if (ip_len > 0 && server_ip[ip_len - 1] == '\n')
  {
    server_ip[ip_len - 1] = '\0';
  }

  // Use localhost if no IP entered
  if (strlen(server_ip) == 0)
  {
    strcpy(server_ip, "127.0.0.1");
  }

  // Get message from user
  printf("Enter message to send to server: ");
  fgets(message, BUFFER_SIZE, stdin);

  // Remove newline character if present
  size_t len = strlen(message);
  if (len > 0 && message[len - 1] == '\n')
  {
    message[len - 1] = '\0';
  }

  // Create socket
#ifdef _WIN32
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
  {
#else
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
#endif
    printf("\n Socket creation error \n");
#ifdef _WIN32
    WSACleanup();
#endif
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 address from text to binary form
  serv_addr.sin_addr.s_addr = inet_addr(server_ip);
  if (serv_addr.sin_addr.s_addr == INADDR_NONE)
  {
    printf("\nInvalid IP address: %s\n", server_ip);
#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif
    return -1;
  }

  printf("Attempting to connect to %s:%d...\n", server_ip, PORT);

  // Connect to server
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\nConnection Failed to %s:%d\n", server_ip, PORT);
    printf("Make sure the server is running and the IP address is correct\n");
#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif
    return -1;
  }

  printf("Connected successfully to %s:%d\n", server_ip, PORT);

  // Send request
  send(sock, message, strlen(message), 0);
  printf("Message sent: %s\n", message);

  // Read response
#ifdef _WIN32
  int valread = recv(sock, buffer, BUFFER_SIZE - 1, 0);
#else
  int valread = read(sock, buffer, BUFFER_SIZE - 1);
#endif

  if (valread > 0)
  {
    buffer[valread] = '\0';
    printf("Response received:\n%s\n", buffer);
  }
  else
  {
    printf("No response received or error occurred\n");
  }

#ifdef _WIN32
  closesocket(sock);
  WSACleanup();
#else
  close(sock);
#endif
  return 0;
}