#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
 char *ip = "127.0.0.1";
 int port1 = 8080;
 int port2 = 8081;

 int socket_fd1;
 int socket_fd2;

 struct sockaddr_in server_address1;
 struct sockaddr_in server_address2;

 struct sockaddr_in client_address1;
 struct sockaddr_in client_address2;

 socklen_t socket1_addr_len;
 socklen_t socket2_addr_len;

 char message1[2048] = {'\0'};
 char message2[2048] = {'\0'};

 socket_fd1 = socket(AF_INET, SOCK_DGRAM, 0);
 socket_fd2 = socket(AF_INET, SOCK_DGRAM, 0);

 if (socket_fd1 < 0 || socket_fd2 < 0)
 {
  perror("Socket Error!");
  exit(0);
 }

 memset(&server_address1, '\0', sizeof(server_address1));
 server_address1.sin_family = AF_INET;
 server_address1.sin_port = htons(port1);
 server_address1.sin_addr.s_addr = inet_addr(ip);

 int bind_status = bind(socket_fd1, (struct sockaddr *)&server_address1, sizeof(server_address1));
 if (bind_status < 0)
 {
  perror("Bind error");
  exit(0);
 }

 memset(&server_address2, '\0', sizeof(server_address2));
 server_address2.sin_family = AF_INET;
 server_address2.sin_port = htons(port2);
 server_address2.sin_addr.s_addr = inet_addr(ip);

 bind_status = bind(socket_fd2, (struct sockaddr *)&server_address2, sizeof(server_address2));
 if (bind_status < 0)
 {
  perror("Bind error");
  exit(0);
 }

 while (1)
 {
  socket1_addr_len = sizeof(client_address1);
  socket2_addr_len = sizeof(client_address2);

  int receive_status1 = recvfrom(socket_fd1, message1, 2048, 0, (struct sockaddr *)&client_address1, &socket1_addr_len);
  int receive_status2 = recvfrom(socket_fd2, message2, 2048, 0, (struct sockaddr *)&client_address2, &socket2_addr_len);

  if (receive_status1 == -1 || receive_status2 == -1)
  {
   perror("\nRecieving Message Failed!");
   exit(0);
  }
  else
  {
   printf("Recevied from Client1: %s\n", message1);
   printf("Recevied from Client2: %s\n", message2);
  }
  int choiceA = atoi(message1);
  int choiceB = atoi(message2);
  int result;

  if (choiceA == choiceB)
  {
   result = 0; // Draw
  }
  else if ((choiceA == 0 && choiceB == 2) || (choiceA == 1 && choiceB == 0) || (choiceA == 2 && choiceB == 1))
  {
   result = 1; // ClientA wins
  }
  else
  {
   result = 2; // ClientB wins
  }
  char result_str[] = "Game is Draw";
  char result_str1[] = "You won";
  char result_str2[] = "You lost";
  switch (result)
  {
  case 0:
   sendto(socket_fd1, result_str, 2048, 0, (struct sockaddr *)&client_address1, socket1_addr_len);
   sendto(socket_fd2, result_str, 2048, 0, (struct sockaddr *)&client_address2, socket2_addr_len);
   break;
  case 1:
   sendto(socket_fd1, result_str1, 2048, 0, (struct sockaddr *)&client_address1, socket1_addr_len);
   sendto(socket_fd2, result_str2, 2048, 0, (struct sockaddr *)&client_address2, socket2_addr_len);
   break;
  case 2:
   sendto(socket_fd1, result_str2, 2048, 0, (struct sockaddr *)&client_address1, socket1_addr_len);
   sendto(socket_fd2, result_str1, 2048, 0, (struct sockaddr *)&client_address2, socket2_addr_len);
   break;
  default:
   break;
  }
  // Send the result to both clients
  printf("ClientA: %d, ClientB: %d, Result: %d\n\n", choiceA, choiceB, result);

  receive_status1 = recvfrom(socket_fd1, message1, 2048, 0, (struct sockaddr *)&client_address1, &socket1_addr_len);
  receive_status2 = recvfrom(socket_fd2, message2, 2048, 0, (struct sockaddr *)&client_address2, &socket2_addr_len);

  if (receive_status1 == -1 || receive_status2 == -1)
  {
   perror("\nRecieving Message Failed!");
   exit(0);
  }

  char ready = '1';
  if (!(message1[0] == 1 && message2[0] == 1))
  {
   ready = '0';
   sendto(socket_fd1, &ready, 2048, 0, (struct sockaddr *)&client_address1, socket1_addr_len);
   sendto(socket_fd2, &ready, 2048, 0, (struct sockaddr *)&client_address2, socket2_addr_len);
  }
  else
  {
   sendto(socket_fd1, &ready, 2048, 0, (struct sockaddr *)&client_address1, socket1_addr_len);
   sendto(socket_fd2, &ready, 2048, 0, (struct sockaddr *)&client_address2, socket2_addr_len);
  }
 }
 close(socket_fd1);
 close(socket_fd2);

 return 0;
}
