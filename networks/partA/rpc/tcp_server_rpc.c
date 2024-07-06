#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{

 int server_socket1;
 int server_socket2;
 int client_socket1;
 int client_socket2;

 struct sockaddr_in server_address1;
 struct sockaddr_in server_address2;
 struct sockaddr_in client1_address;
 struct sockaddr_in client2_address;
 socklen_t server_addr_length;
 socklen_t client1_addr_length;
 socklen_t client2_addr_length;

 int bind_status;
 char *ip_address = "127.0.0.1";
 int port1 = 6667;
 int port2 = 6666;

 char *message = "I am server sending you the connection Confirmation.";

 server_socket1 = socket(AF_INET, SOCK_STREAM, 0);
 server_socket2 = socket(AF_INET, SOCK_STREAM, 0);
 if (server_socket1 < 0 || server_socket2 < 0)
 {
  perror("Socket error");
 }

 memset(&server_address1, '\0', sizeof(server_address1));
 server_address1.sin_family = AF_INET;
 server_address1.sin_port = port1;
 server_address1.sin_addr.s_addr = inet_addr(ip_address);

 bind_status = bind(server_socket1, (struct sockaddr *)&server_address1, sizeof(server_address1));
 if (bind_status < 0)
 {
  perror("Bind Error");
 }

 memset(&server_address2, '\0', sizeof(server_address2));
 server_address2.sin_family = AF_INET;
 server_address2.sin_port = port2;
 server_address2.sin_addr.s_addr = inet_addr(ip_address);
 bind_status = bind(server_socket2, (struct sockaddr *)&server_address2, sizeof(server_address2));
 if (bind_status < 0)
 {
  perror("Bind Error");
 }

 int listen_status1 = listen(server_socket1, 5);
 int listen_status2 = listen(server_socket2, 5);
 if (listen_status1 < 0 || listen_status2 < 0)
 {
  perror("Listen Error");
 }
 client1_addr_length = sizeof(client1_address);
 client2_addr_length = sizeof(client2_address);
 char recieve_msg1[10];
 char recieve_msg2[10];
 while (1)
 {
  client_socket1 = accept(server_socket1, (struct sockaddr *)&client1_address, &client1_addr_length);
  int receive_status1 = recv(client_socket1, &recieve_msg1, sizeof(recieve_msg1), 0);
  client_socket2 = accept(server_socket2, (struct sockaddr *)&client2_address, &client2_addr_length);
  int receive_status2 = recv(client_socket2, &recieve_msg2, sizeof(recieve_msg2), 0);
  if (receive_status1 == -1 && receive_status2 == -1)
  {
   perror("\nRecieving Message Failed!");
  }
  else
  {
   printf("Recevied from Client1: %c\n", recieve_msg1[0]);
   printf("Recevied from Client1: %c\n", recieve_msg2[0]);
   int choiceA = atoi(recieve_msg1);
   int choiceB = atoi(recieve_msg2);
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
    send(client_socket1, &result_str, sizeof(result_str), 0);
    send(client_socket2, &result_str, sizeof(result_str), 0);
    break;
   case 1:
    send(client_socket1, &result_str1, sizeof(result_str1), 0);
    send(client_socket2, &result_str2, sizeof(result_str2), 0);
    break;
   case 2:
    send(client_socket1, &result_str2, sizeof(result_str2), 0);
    send(client_socket2, &result_str1, sizeof(result_str1), 0);
    break;
   default:
    break;
   }
   // Send the result to both clients
   printf("ClientA: %d, ClientB: %d, Result: %d\n\n", choiceA, choiceB, result);

   // client_socket1 = accept(server_socket1, (struct sockaddr *)&client1_address, &client1_addr_length);
   receive_status1 = recv(client_socket1, &recieve_msg1, sizeof(recieve_msg1), 0);
   // client_socket2 = accept(server_socket2, (struct sockaddr *)&client2_address, &client2_addr_length);
   receive_status2 = recv(client_socket2, &recieve_msg2, sizeof(recieve_msg2), 0);
   char ready = '1';
   if (!(recieve_msg1[0] == 1 && recieve_msg2[0] == 1))
   {
    ready = '0';
    send(client_socket1, &ready, sizeof(ready), 0);
    send(client_socket2, &ready, sizeof(ready), 0);
   }
   else
   {
    send(client_socket1, &ready, sizeof(ready), 0);
    send(client_socket2, &ready, sizeof(ready), 0);
   }
  }

  close(client_socket1);
  close(client_socket2);
 }

 return 0;
}