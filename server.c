#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) 
{
	int sock;
	int new_sock;
	socklen_t size;
	char buf[1024];
	ssize_t bytes_read;
	struct sockaddr_un server_address;

	size = sizeof(struct sockaddr_un);
	sock = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (sock == -1)
	{
		fprintf(stderr, "Incorrect server socket\n");
		exit(1);
	}

	if(remove("/tmp/udpser") == -1 && errno != ENOENT)
	{
		fprintf(stderr, "Incorrect\n");
		exit(1);
	}
    
	server_address.sun_family = AF_LOCAL;
	strncpy(server_address.sun_path, "/tmp/udpser", sizeof(server_address.sun_path) - 1);
	
	if(bind(sock, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		perror("what");
		fprintf(stderr, "Incorrect server bind\n");
		exit(1);
	}
		
	if(listen(sock, 10) == -1)
	{
		fprintf(stderr, "Incorrect server listen\n");
		exit(1);
	}
	
	new_sock = accept(sock, NULL, NULL);
	if (new_sock == -1)
	{
		fprintf(stderr, "Incorrect server accept\n");
		exit(1);
	}

	while(1) 
	{	
		bytes_read = recv(new_sock, buf, 1024, 0);
	    if(bytes_read == -1)
		{
			fprintf(stderr, "Incorrect server recv\n");
			exit(1);
		}

	    printf("%s", buf);
	    	
		for(int i = 0; i  < bytes_read; i++)
		{
			buf[i] = toupper((unsigned char) buf[i]);
		}
		
		if(send(new_sock, buf, bytes_read, 0) == -1)
		{
			fprintf(stderr, "Incorrect server send\n");
			exit(1);
		}
	}
	shutdown(new_sock, SHUT_RDWR);
    shutdown(sock, SHUT_RDWR);

    return 0;
}
