#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <arpa/inet.h>
#define MAX 1024
#define PORT 8012
#define SA struct sockaddr 


int receive_video(int sockfd)
{
	char recvBuff[500];
    	memset(recvBuff, '0', sizeof(recvBuff));	// put all 0s in recvbuff
	FILE *fp;
	printf("Receiving file...\n");
   	fp = fopen("myfile1.mp4", "w"); 
    	if(NULL == fp)
    	{
       		printf("Error opening file");
         	return 1;
    	}
    	
    	long double sz=1;
    	
    	char buffer[80];
    	recv(sockfd,buffer,80,0);
    	int size = atoi(buffer);
    	
    	int total=0;
    	int bytesReceived;
    
    	while((bytesReceived = read(sockfd, recvBuff, 500)) > 0)
    	{
    		total+=bytesReceived;
		fflush(stdout);
        	fwrite(recvBuff, 1,bytesReceived,fp);
        	memset(recvBuff, '0', sizeof(recvBuff));
        	if(total>=size)
        	{
        		printf("\nFile Receiving....Completed\n");
    			return 0;
        	}
    	}

    	if(bytesReceived < 0){
        	printf("\n Read Error \n");
        	return 0;
        	}
        	
    	printf("\nFile Receiving....Completed\n");
    	return 0;
 
}

void receive(int sockfd) 
{ 
	char buff[1024]; 
	int n; 
	while(1) 
	{ 
		bzero(buff, sizeof(buff)); 
		printf("Enter the message   : "); 
		n = 0; 
		
		// client -> server
				while ((buff[n++] = getchar()) != '\n'); 
				write(sockfd, buff, sizeof(buff)); 
				if (strncmp(buff, "Bye", 3) == 0) 
				{ 
					printf("END OF CLIENT\n"); 
					break; 
				}
				if(strncmp(buff,"GivemeyourVideo",15)==0)
				{
					receive_video(sockfd);
				}	
		// server -> client
				else
				{
				bzero(buff, sizeof(buff)); 
				int s=read(sockfd, buff, sizeof(buff)); 
				if(s==0)
				{
					printf("Server is closed\n");
					return;
				}
				printf("Message from SERVER : %s", buff); 
				}
				
		 
	} 
} 


int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	// socket create and varification 
			sockfd = socket(AF_INET, SOCK_STREAM, 0); 
			if (sockfd == -1) 
			{ 
				printf("CREATION OF SOCKET'S FAILED\n"); 
				exit(0); 
			} 
			else
				printf("SOCKET IS CREATED\n"); 
	
			bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
			servaddr.sin_family = AF_INET; 
			servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
			servaddr.sin_port = htons(PORT); 

	// connect the client socket to server socket 
			if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) 
			{
			 	printf("CONNECTION WITH SERVER IS FAILED\n"); 
				exit(0); 
			} 
			else
				printf("CONNECTED TO SERVER\n"); 

	// function for chat 
			receive(sockfd); 

	// close the socket 
			close(sockfd); 
} 

