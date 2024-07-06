#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "sys/fcntl.h"
#include <sys/time.h>
#include <sys/select.h>
#define PORT 8080
#define CHUNK_SIZE 20

struct info_struct
{
 int seq_no;
 int done;
 int total_chunks;
 char data[CHUNK_SIZE];
 time_t send_time;
};

struct ack_struct
{
 int seq_no;
};

void send_message(char *data_str, int totalsize, int udp_server, struct sockaddr_in server_address)
{
 struct info_struct data_struct;
 int server_address_len = sizeof(server_address);
 int total_chunks = (totalsize / CHUNK_SIZE) + 1;
 int ackn[total_chunks];
 memset(ackn, 0, sizeof(ackn));
 int remaining_data_size = total_chunks;
 int i = 0;
 int seq_no = 1;
 int flag = 0;

 struct timeval timeout;
 fd_set readfds;
 timeout.tv_sec = 0;
 timeout.tv_usec = 100000;
 while (1)
 {
  int present_chunk_size = CHUNK_SIZE;
  remaining_data_size = totalsize - ((seq_no - 1) * CHUNK_SIZE);
  if (remaining_data_size <= 0 || flag == 1)
  {

   int i;
   for (i = 0; i < total_chunks; i++)
   {
    if (ackn[i] == 1)
    {
     continue;
    }
    else
    {
     break;
    }
   }
   if (i == total_chunks)
   {
    memset(data_struct.data, 0, sizeof(data_struct.data));
    data_struct.seq_no = seq_no++;
    data_struct.total_chunks = total_chunks;
    strcpy(data_struct.data, "done");
    data_struct.done = 1;

    sendto(udp_server, &data_struct, sizeof("done") + 12, 0, (struct sockaddr *)&server_address, sizeof(server_address));
    printf("All chunks are sent\n");
    break;
   }
   else
   {
    seq_no = i + 1;
    flag = 0;
    continue;
   }
  }
  if (remaining_data_size < CHUNK_SIZE)
  {
   present_chunk_size = remaining_data_size;
  }
  memset(data_struct.data, 0, sizeof(data_struct.data));
  strncpy(data_struct.data, &data_str[(seq_no - 1) * CHUNK_SIZE], present_chunk_size);
  data_struct.data[present_chunk_size] = '\0';

  data_struct.seq_no = seq_no++;
  data_struct.total_chunks = total_chunks;
  data_struct.done = 0;

  sendto(udp_server, &data_struct, present_chunk_size + 12, 0, (struct sockaddr *)&server_address, sizeof(server_address));

  FD_ZERO(&readfds);
  FD_SET(udp_server, &readfds);

  int select_result = select(udp_server + 1, &readfds, NULL, NULL, &timeout);
  if (select_result == -1)
  {
   perror("select");
   exit(1);
  }
  else if (select_result == 0)
  {
   for (int i = 0; i < total_chunks; i++)
   {
    if (ackn[i] == 0)
    {
     int present_chunk_size = (i == total_chunks - 1) ? remaining_data_size : CHUNK_SIZE;
     memset(data_struct.data, 0, sizeof(data_struct.data));
     strncpy(data_struct.data, &data_str[i * CHUNK_SIZE], present_chunk_size);
     data_struct.data[present_chunk_size] = '\0';

     data_struct.seq_no = i + 1;
     data_struct.total_chunks = total_chunks;
     data_struct.done = 0;

     sendto(udp_server, &data_struct, present_chunk_size + 12, 0, (struct sockaddr *)&server_address, sizeof(server_address));
     printf("Resending chunk %d\n", data_struct.seq_no - 1);
    }
   }
  }
  else if (FD_ISSET(udp_server, &readfds))
  {
   struct ack_struct acknowledge;
   int acknowledge_stat = recvfrom(udp_server, &acknowledge, sizeof(acknowledge), 0, NULL, NULL);
   if (acknowledge_stat > 0)
   {
    ackn[acknowledge.seq_no - 1] = 1;
    printf("Received ACK for chunk %d\n", acknowledge.seq_no - 1);
   }
  }
 }
}

int main()
{
 char ip[] = "127.0.0.1";
 int udp_server = socket(AF_INET, SOCK_DGRAM, 0);
 int to_not_stop = fcntl(udp_server, F_GETFL, 0);
 to_not_stop = to_not_stop | O_NONBLOCK;
 fcntl(udp_server, F_SETFL, to_not_stop);
 if (udp_server < 0)
 {
  perror("Socket");
  exit(1);
 }

 struct sockaddr_in server_address;
 struct sockaddr_in client_address;
 memset(&server_address, 0, sizeof(server_address));
 server_address.sin_family = AF_INET;
 server_address.sin_port = htons(PORT);
 server_address.sin_addr.s_addr = inet_addr(ip);
 int bind_status = bind(udp_server, (struct sockaddr *)&server_address, sizeof(server_address));
 if (bind_status < 0)
 {
  perror("Bind");
  close(udp_server);
  exit(1);
 }

 struct info_struct *received_chunk_data = (struct info_struct *)malloc(sizeof(struct info_struct));
 int total_recv_chunks = 0;
 while (1)
 {
  int end_it = 0;
  struct info_struct temp_chunk;
  memset(temp_chunk.data, 0, sizeof(temp_chunk.data));
  socklen_t client_size = sizeof(client_address);
  int recv_size = recvfrom(udp_server, &temp_chunk, sizeof(temp_chunk), 0, (struct sockaddr *)&client_address, &client_size);
  if (recv_size < 0)
  {
   continue;
  }
  int seq_no = temp_chunk.seq_no - 1;
  int total_chunks = temp_chunk.total_chunks;
  struct ack_struct acknowledge;
  acknowledge.seq_no = seq_no + 1;
  sendto(udp_server, &acknowledge, sizeof(acknowledge), 0, (struct sockaddr *)&client_address, client_size);
  if (temp_chunk.done == 1)
  {
   end_it = 1;
   goto exit_fun;
  }
  printf("Received chunk with sequence number %d\n", seq_no);

  if (total_recv_chunks == 0)
  {
   received_chunk_data = (struct info_struct *)realloc(received_chunk_data, (total_chunks * sizeof(struct info_struct) + 1));
   for (int i = 0; i < total_chunks; i++)
   {
    received_chunk_data[i].seq_no = -1;
   }
  }
  received_chunk_data[seq_no] = temp_chunk;
  total_recv_chunks++;
 exit_fun:
  int received = 1;
  for (int i = 0; i < total_chunks; i++)
  {
   if (received_chunk_data[i].seq_no == -1)
   {
    received = 0;
    break;
   }
  }

  if (received && end_it == 1)
  {
   printf("Received Data:\n");
   for (int i = 0; i < total_chunks; i++)
   {
    printf("Chunk %d data : %s\n", received_chunk_data[i].seq_no, received_chunk_data[i].data);
   }
   printf("\n");
   break;
  }
 }
 if (total_recv_chunks > 0)
 {
  free(received_chunk_data);
 }
 char temp_data[] = "This is a test data from the server to client.";

 send_message(temp_data, strlen(temp_data), udp_server, client_address);
 close(udp_server);

 return 0;
}
