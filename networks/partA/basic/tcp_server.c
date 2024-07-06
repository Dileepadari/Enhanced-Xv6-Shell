#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{

  int server_socket;
  int client_socket;

  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  socklen_t server_addr_length;
  socklen_t clinet_addr_length;

  int bind_status;
  char *ip_address = "127.0.0.1";
  int port = 6667;

  char *message = "I am server sending you the connection Confirmation.";

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0)
  {
    perror("Socket error");
  }

  memset(&server_address, '\0', sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = port;
  server_address.sin_addr.s_addr = inet_addr(ip_address);

  bind_status = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
  if (bind_status < 0)
  {
    perror("Bind Error");
    exit(0);
  }

  int listen_status = listen(server_socket, 5);
  if (listen_status < 0)
  {
    perror("Listen Error");
    exit(0);
  }
  clinet_addr_length = sizeof(client_address);
  char recieve_msg[2048] = {'\0'};
  while (1)
  {
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &clinet_addr_length);
    memset(&recieve_msg, '\0', 2048);
    int receive_status = recv(client_socket, recieve_msg, sizeof(recieve_msg), 0);
    if (receive_status == -1)
    {
      perror("\nRecieving Message Failed!");
      exit(0);
    }
    else
    {
      printf("Recevied from Client: %s\n", recieve_msg);
    }
    int sent_status = send(client_socket, message, strlen(message), 0);
    if (sent_status == -1)
    {
      perror("sending to client Failed\n");
      exit(0);  
    }
    else
    {
      printf("Sent to Client: %s\n", message);
    }
    printf("\n");
    close(client_socket);
  }

  close(server_socket);
  return 0;
}
