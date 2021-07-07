#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>
#define MAX 1024
#define PORT 8012
#define SA struct sockaddr 


// GENERATE MEANINGLESS RANDOM STRING
void gen_random(char *s, const int len) {
    static const char alphanum[] =     " ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz ";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] ='\n';
    s[len+1]=0;
}




// FILE TRANSFER
int sendfile(int connfd)
{
      struct stat st;
	char buffer[80];
	stat("myfile.mp4", &st);
	int size = st.st_size;
      	bzero(buffer,sizeof(buffer));
      	snprintf(buffer,sizeof(buffer),"%d",size);
      	send(connfd,buffer,80,0);
	
        FILE *fp = fopen("myfile.mp4","r");
        if(fp==NULL)
        {
            printf("File open error");
            return 1;   
        }   

        while(1)
        {
            
            char buff[500];
            bzero(buff, 500);
            int nread = fread(buff,1,500,fp);
         
            
            if(nread > 0)
            {
                write(connfd, buff, nread);
            }
            if (nread < 500)
            {
                if (feof(fp))
		{
                    printf("End of file\n");
		    printf("File transfer completed\n");
		}
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }
        }
	
	
        
   
        printf("This Video is sent\n");
        return 0;
       
        

}


// Function designed for chat between client and server. 
void func(int newsockfd) 
{ 
	char buff[1024]; 
	int n; 

		
	// infinite loop for chat 
	while(1) { 
		bzero(buff, 1024); 

		// read the message from client and copy it in buffer 
		int s=read(newsockfd, buff, sizeof(buff)); 
		
		// if msg contains "Bye" then server will exit and chat's ended. 
		if (strncmp("Bye", buff, 3) == 0||s==0) 
		{ 
			if(s)
			{
				printf("From CLIENT : %s", buff);
			}
			printf("END OF CLIENT \n"); 
			break; 
		} 
		
		printf("From CLIENT : %s", buff); 
		
		
		if((strncmp("GivemeyourVideo",buff,15))==0)
		{
			
			printf("\nInitiating the Sending Process...\n");
			sendfile(newsockfd);
		
		}
		
		else
		{
		bzero(buff, 1024); 
		gen_random(buff,rand()%10);
		printf("To CLIENT : %s",buff);
		write(newsockfd, buff, sizeof(buff));
		} 

		
	} 
} 






// Main Function
int main() 
{ 
	int sockfd, newsockfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 	
			sockfd = socket(AF_INET, SOCK_STREAM, 0); 
			if (sockfd == -1) 
			{ 
				printf("SOCKET CREATION FAILED \n"); 
				exit(0); 
			} 
			else
			printf("SOCKET IS CREATED\n"); 
	
			bzero(&servaddr, sizeof(servaddr)); 
	
	// assign IP, PORT 
			servaddr.sin_family = AF_INET; 
			servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
			servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
			if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) 
			{ 
				printf("SOCKET BINDING IS FAILED\n"); 
				exit(0); 
			} 
			else
				printf("SOCKET IS BINDED \n"); 

	
	
	// Now server is ready to listen and verification 
			if ((listen(sockfd, 5)) != 0) 
			{ 
				printf("LISTENING IS FAILED\n"); 
				exit(0); 
			}		 
			else
				printf("SERVER IS LISTENING\n"); 
	
			len = sizeof(cli); 

	// Accept the data packet from client and verification 
			newsockfd = accept(sockfd, (SA*)&cli, &len); 
			if (newsockfd < 0) 
			{ 
				printf("SERVER ACCEPTANCE IS  FAILED\n"); 
				exit(0); 
			} 
			else
				printf("SERVER ACCEPTED THE CLIENT\n"); 
		
		
	
	
	// Function for chatting between client and server 
			func(newsockfd); 

	// After chatting close the socket 
			close(sockfd); 
} 

