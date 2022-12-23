#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXLINE 10000

void err_sys(const char *x)
{
	perror(x);
	exit(1);
}

void err_quit(const char *x)
{
	perror(x);
	exit(1);
}

typedef const struct sockaddr SA;

int main(int argc, char **argv)
{
	
	int sockfd, n;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5500); /* daytime server */
	if (inet_pton(AF_INET,"127.0.0.1" /*argv[1]*/, &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for");

	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	while ((n = read(sockfd, recvline, MAXLINE)) > 0)
	{
		recvline[n] = 0; /* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
	if (n < 0)
		err_sys("read error");

	exit(0);
}