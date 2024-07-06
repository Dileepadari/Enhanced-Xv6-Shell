#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
  char *ip = "127.0.0.1";
  int port = 8080;
  int socket_fd;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  socklen_t socket_addr_len;
  char message[2048] = {'\0'};

  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd < 0)
  {
    perror("Socket Error!");
    exit(0);
  }

  memset(&server_address, '\0', sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = inet_addr(ip);

  int bind_status = bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));
  if (bind_status < 0)
  {
    perror("Bind error");
    exit(0);
  }
  while (1)
  {
    socket_addr_len = sizeof(client_address);
    int receive_status = recvfrom(socket_fd, message, 2048, 0, (struct sockaddr *)&client_address, &socket_addr_len);

    if (receive_status == -1)
    {
      perror("\nRecieving Message Failed!");
      exit(0);
    }
    else
    {
      printf("Recevied from Client: %s\n", message);
    }
    char sent_msg[] = "This is msg to client through udp server";
    int sent_status = sendto(socket_fd, sent_msg, 2048, 0, (struct sockaddr *)&client_address, socket_addr_len);

    if (sent_status == -1)
    {
      perror("sending to client Failed\n");
      exit(0);
    }
    else
    {
      printf("Sent to Client: %s\n", sent_msg);
    }
    printf("\n");
  }
  close(socket_fd);
  return 0;
}
