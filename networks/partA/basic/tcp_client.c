#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int send_message(int client_socket, char *message, struct sockaddr_in client_address)
{
  char *ip = "127.0.0.1";
  int port = 6667;

  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket < 0)
  {
    perror("Socket Error");
    exit(0);
  }
  memset(&client_address, '\0', sizeof(client_address));
  client_address.sin_family = AF_INET;
  client_address.sin_port = port;
  client_address.sin_addr.s_addr = inet_addr(ip);
  char receive_msg[2048] = {'\0'};
  if ((connect(client_socket, (const struct sockaddr *)&client_address, sizeof(client_address))) < 0)
  {
    perror("Connection Failed!");
    exit(0);
  }
  else
  {
    int sent_status = send(client_socket, message, strlen(message), 0);
    if (sent_status == -1)
    {
      perror("sending to Server Failed\n");
      exit(0);
    }
    else
    {
      printf("Sent to Server: %s\n", message);
    }
    // receive response
    memset(&receive_msg, '\0', strlen(receive_msg));
    int receive_status = recv(client_socket, receive_msg, sizeof(receive_msg), 0);
    if (receive_status == -1)
    {
      perror("\nRecieving Message Failed!");
      exit(0);
    }
    else
    {
      printf("Recevied from Server: %s\n", receive_msg);
    }
  }
  close(client_socket);
  return 0;
}
int main(int argc, char const *argv[])
{

  int client_socket;
  struct sockaddr_in client_address;
  socklen_t client_addr_length;
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