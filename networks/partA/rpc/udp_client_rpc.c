#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int send_message(int client_socket, char *message, struct sockaddr_in client_address, int port)
{
 char *ip = "127.0.0.1";
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
 printf("\nDo you want to play again(0 to exit or 1 to play).");
 int x;
 scanf("%d", &x);
 sent_status = sendto(socket_fd, &x, 2048, 0, (struct sockaddr *)&client_address, sizeof(client_address));
 if (sent_status == -1)
 {
  perror("sending to Server Failed\n");
  exit(0);
 }
 receive_status = recvfrom(socket_fd, receive_msg, 2048, 0, (struct sockaddr *)&client_address, &socket_addr_len);
 if (receive_status == -1)
 {
  perror("\nRecieving Message Failed!");
  exit(0);
 }
 close(socket_fd);
 if (receive_msg[0] == '1')
 {
  return 1;
 }
 printf("Both users are not willing to play game.\n");
 return 0;
}

int main(int argc, char **argv)
{

 int client_socket;
 int port = atoi(argv[1]);
 struct sockaddr_in client_address;
 char message[20];

 int x = 1;

 while (x != 0)
 {
  fflush(stdin);
  printf("Enter your choice : ");
  scanf("%s", message);
  if (!(message[0] == '0' || message[0] == '1' || message[0] == '2'))
  {
   printf("Invalid Option! Try again\n");
   continue;
  }
  x = send_message(client_socket, message, client_address, port);
 }

 return 0;
}