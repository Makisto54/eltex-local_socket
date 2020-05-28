#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) 
{
	int i;
	int j;
	int sock;
	int new_sock;
	socklen_t size;
	char buf[1024];
	char buf2[1024];
	ssize_t bytes_read;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;

	size = sizeof(struct sockaddr_un);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		fprintf(stderr, "Incorrect client socket\n");
		exit(1);
	}

	if (remove("/tmp/udpser") == -1 && errno != ENOENT)
	{
		fprintf(stderr, "Incorrect\n");
		exit(1);
	}
    
	server_address.sun_family = AF_LOCAL;
	client_address.sun_family = AF_LOCAL;

	strncpy(server_address.sun_path, "/tmp/udpser", sizeof(server_address.sun_path) - 1);
	strncpy(client_address.sun_path, "/tmp/udpcli", sizeof(server_address.sun_path) - 1);
	
	if(bind(sock, (struct sockaddr *)&client_address, sizeof(client_address)) == -1)
	{
		fprintf(stderr, "Incorrect client bind\n");
		exit(1);
	}
	
	if(connect(sock, (struct sockaddr *)& server_address, sizeof(server_address)) == -1)
	{
		fprintf(stderr, "Incorrect client connect\n");
		exit(1);
	}
	
	i = 0;

	puts("Enter messages:");
	while(i < 10) 
	{
		if(!memset(buf, 0, 1024))
		{
			fprintf(stderr, "Incorrect buf memset\n");
			exit(1);
		}
		if(!memset(buf2, 0, 1024))
		{
			fprintf(stderr, "Incorrect buf2 memset\n");
			exit(1);
		}
			
		fgets(buf, 1024, stdin);
			
		if(send(sock, buf, 1024, 0) == -1)
		{
			fprintf(stderr, "Incorrect client send\n");
			exit(1);
		}
		if(recv(sock, buf2, 1024, 0) == -1)
		{
			fprintf(stderr, "Incorrect client recv\n");
			exit(1);
		}
		printf("%s", buf2);
		i++;
	}
	shutdown(sock, SHUT_RDWR);

	return 0;
}
