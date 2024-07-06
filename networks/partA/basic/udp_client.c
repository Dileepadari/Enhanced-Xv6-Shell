#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int send_message(int client_socket, char *message, struct sockaddr_in client_address)
{
  char *ip = "127.0.0.1";
  int port = 8080;
  int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd < 0)
  {
    perror("Socket Error");
    exit(0);
  }

  memset(&client_address, '\0', sizeof(client_address));
  client_address.sin_family = AF_INET;
  client_address.sin_port = htons(port);
  client_address.sin_addr.s_addr = inet_addr(ip);

  int sent_status = sendto(socket_fd, message, 2048, 0, (struct sockaddr *)&client_address, sizeof(client_address));
  if (sent_status == -1)
  {
    perror("sending to Server Failed\n");
    exit(0);
  }
  else
  {
    printf("Sent to Server: %s\n", message);
  }

  char receive_msg[2048] = {'\0'};
  socklen_t socket_addr_len = sizeof(client_address);
  int receive_status = recvfrom(socket_fd, receive_msg, 2048, 0, (struct sockaddr *)&client_address, &socket_addr_len);
  if (receive_status == -1)
  {
    perror("\nRecieving Message Failed!");
    exit(0);
  }
  else
  {
    printf("Recevied from Server: %s\n", receive_msg);
  }
  close(socket_fd);
}

int main(int argc, char **argv)
{

  int client_socket;
  struct sockaddr_in client_address;
  char *message = "I am a client requesting a message.";

  int x = 1;

  while (x != 0)
  {
    send_message(client_socket, message, client_address);
    printf("\nDo you want to send it again(0 to exit) : ");
    fflush(stdin);
    scanf("%d", &x);
  }

  return 0;
}