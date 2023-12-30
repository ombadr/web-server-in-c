#include <sys/socket.h> 
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>

int main() {
	
	int s = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr = {
		AF_INET,
		htons(8080),
		0 
	};


	bind(s, (struct sockaddr*)&addr, sizeof(addr));
	
	listen(s, 10);	
	
	int clientfd = accept(s, NULL, 0);
	
	char buffer[256] = {0};
	recv(clientfd, buffer, 256, 0);	

	// GET /file.html ...

	char* f = buffer + 5;

	*strchr(f, ' ') = 0;	
	int opened_fd = open(f, O_RDONLY);
	off_t offset = 0;
	off_t len = 256;
	
	off_t sent_bytes;
	sendfile(opened_fd, clientfd, offset, &sent_bytes, NULL, 0);
	
	close(opened_fd);
	close(clientfd);
	close(s);


	return 0;
}
