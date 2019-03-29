#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include<pthread.h>
#include <errno.h>
#define PORT	 8080
#define MAXLINE 1024
int sockfd;
char buffer[MAXLINE];
struct sockaddr_in servaddr;
char recvbuff[MAXLINE];
struct sockaddr_in cliaddr;
int i=0;
char data[100][100] = {"b0","a1","r2","a3","t4","h5"};
char ack[100][100]= {"ack0","ack1","ack2","ack3","ack4","ack5"};
int array_len=5;
int received;
int len, n;

int ack_array[10];
int count2=0;

void print_ack_array(){
	printf("\nAck array : ");
	


	for(int i=0;i<=array_len;i++)
		if(ack_array[i]==1)
			count2=count2+1;

	for(int i=0;i<=array_len;i++)
		printf("%d  ",ack_array[i]);
	printf("Count : %d\n",count2);
	
	if(count2 == array_len+1)
		exit(0);
	count2=0;
	/*if(count2==array_len){
		printf("Count : %d",count2);		
		exit(0);
	}
	else
		count2=0;*/
}
void *t_ack(void *vargp){
printf("\nTCK_ACK");
while(1){
	printf("\n");
	//printf("\0");
	struct timeval tv;
	fd_set readfds;

	tv.tv_sec = 1;
	tv.tv_usec = 100000;

	FD_ZERO(&readfds);
	FD_SET(sockfd, &readfds);
 	
	select(sockfd+1, &readfds, NULL, NULL, &tv);

	memset(&recvbuff,'0',sizeof(recvbuff));
	if (FD_ISSET(sockfd, &readfds)){
		//printf("\nAcknowledgement received\n");
		n=recvfrom(sockfd, (char *)recvbuff, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
		recvbuff[n]='\0';	
		printf("\nt_recv  : %s",recvbuff);
		
		for(int i=0;i<=array_len;i++)
			if(strcmp(ack[i],recvbuff)==0)
				ack_array[i]=1;

		
		print_ack_array();
			
	}
}
}


void *t_sen(void *vargp){
	for(int i=0;i<=array_len;i++){
		//printf("Inside t_sen\n");
		sendto(sockfd, (const char *)data[i], strlen(data[i]),MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
		//printf("\nmsg sent");
		//sendto(sockfd, (const char *)ack, strlen(ack),MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
	}
	
	//Check for acknowledgements
	sleep(3);
	printf("\nChecking Messages...");
	int notsent=0;
	while(1){
		for(int i=0;i<=array_len;i++){
			if(ack_array[i]==0){
				notsent++;
				printf("\n%s is not sent",data[i]);
				sendto(sockfd, (const char *)data[i], strlen(data[i]),MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
			}	
		}
		if(notsent==0){
			printf("\nAll Messages are sent\n");
			exit(0);	
		}
	}
}
	


int main()
{
	for(int i=0;i<=4;i++)
		ack_array[i]=0;
	pthread_t thread_id1,thread_id2;
	
	//printf("\nEnter Name to send : ");
	//fgets(data,50,stdin);
	//printf("\n%s",data);
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	//Threads
	
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	len=sizeof(cliaddr);
	n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
	buffer[n] = '\0';
	//printf("Client : %s\n", buffer);
	
	//int value = sendto(sockfd, (const char *)hello, strlen(hello),MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
	//printf("Value : %d\n",value);
	//if( value == -1)
	//	printf("Error in sendto : %s\n",strerror(errno));
	//sendto(sockfd, (const char *)data[0], strlen(data[0]),MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
	
	
	 
	pthread_create(&thread_id1, NULL, t_sen, NULL); 
	pthread_create(&thread_id2, NULL, t_ack, NULL);
	pthread_join(thread_id1, NULL);
	
	pthread_join(thread_id2, NULL); 
	return 0;
}
