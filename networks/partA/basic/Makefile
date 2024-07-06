all: q1 q2 q3 q4

q1 : 
	gcc tcp_server.c -o tcps

q2 : 
	gcc tcp_client.c -o tcpc

q3 : 
	gcc udp_server.c -o udps

q4 : 
	gcc udp_client.c -o udpc

clean:
	rm -f */*.o $(*.o) $(*.exe)