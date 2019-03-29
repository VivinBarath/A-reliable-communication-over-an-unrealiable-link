#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define PORT	 8080 
#define MAXLINE 1024 
// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from client"; 
	char *ackrecv = "Ack received";
	struct sockaddr_in servaddr;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&servaddr, 0, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
	char recvd_msg[50];

	int n, len;

	sendto(sockfd, (const char *)hello, strlen(hello),MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	int count=0;
	printf("---------------------------------------------");
	printf("\n| Server | Message | Ack num | Actual ACK |");
	printf("\n---------------------------------------------");
	while(1){
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *) &servaddr,&len); 
		buffer[n] = '\0';
		char msg[1];
		strncpy(msg,buffer,n-1);
		printf("\n| %s\t | %s\t   | %c\t     |", buffer,msg,buffer[n-1]);
		char id[1];
		strncpy(id,buffer+(n-1),n);
		char ack[4]="ack";
		strcat(ack,id);
		printf(" %s\t  |",ack);
		if(strcmp(buffer,"r2") !=0)
			sendto(sockfd, (const char *)ack, strlen(ack),MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		else
			count = count+1;
		if(count==2)
			sendto(sockfd, (const char *)ack, strlen(ack),MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		printf("\n---------------------------------------------");
	}
	
	close(sockfd); 
	return 0; 
} 
