#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <pthread.h>




#include <netdb.h>




#define BUF_SIZE 4096

int main(int argc, char **argv)
{
    printf("argv 1 is %s\n", argv[1]);
    int optval = 1;
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval,sizeof (optval));
    printf("1  server socket is created!\n");

    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));            //每个字节都用0填充
    serv_addr.sin_family = AF_INET;                      //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);                    //端口
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    printf("2  server socket is bind to ip address and port!\n");

   //接收客户端请求

    printf("3  server is listenning!-------------------------------------\n");
    listen(serv_sock, 10);
        
         
    /****************** create a client socket************************************/
    int x,tempSocket;
    struct sockaddr_in dest_addr;
    tempSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
    setsockopt(tempSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval,sizeof (optval));
    printf("4 tempSocket socket is created!\n");
    memset(&dest_addr, 0, sizeof(dest_addr));            //每个字节都用0填充
    dest_addr.sin_family = AF_INET;                      //使用IPv4地址
    dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    dest_addr.sin_port = htons(1234);                    //端口
    printf("5 tempSocket is binded to ip address and port!\n");

    /****************** connect to server ************************************/

    connect(tempSocket, (struct sockaddr*) &dest_addr, sizeof(dest_addr)); 
   
    printf("6 connecting to server!\n") ; 

    /************************************************************************/
	
                 
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); 
    printf("7  accept socket is created, and connection is estabilished!\n");

   /************************************************************************/

           char ServerRevBuffer[BUF_SIZE]; 
           memset(ServerRevBuffer, 0, BUF_SIZE);  
           printf("Initial Empty ServerRevBuffer:\n%s\n",ServerRevBuffer);
        
                       
                /******************read file to client socket buffer************************************/
                
                 char tempbuffer[BUF_SIZE];
    		 printf("8  reading files context\n");
                 //read content from 
    		 fread(tempbuffer,BUF_SIZE,1,fopen(argv[1],"r"));
 		 write(tempSocket, tempbuffer, BUF_SIZE); 
                 printf("9  write tempbuffer context to fuzzer-socket buffer:\n%s\n",tempbuffer);

               /****************************************************************************************/

                
                if((read(clnt_sock, ServerRevBuffer, BUF_SIZE))==0)
                {
                    printf("Exception1: can not read message in buffer !\n");
                   // break;   
                } 
                else
                {        
                    printf("10  server receive message from client!\n%s\n",ServerRevBuffer);
                }


               
                //change the buffer content into multiple line string
                

                //fisrt msg is user
                
		if(ServerRevBuffer!=NULL&&strcmp(ServerRevBuffer,"user")!=0&&strcmp(ServerRevBuffer,"USER")!=0)
		   {
		        printf("branch0.1: please input username:user\n");
		        exit(0); 
		   }
                else
                   {
			printf("branch0.2: login successfully!\n");
                        memset(ServerRevBuffer, 0, BUF_SIZE); 
	           }


                
                while(1) 
                {
		        /******************read file to client socket buffer************************************/
		        
		        
	    		 printf("8  reading files context\n");
		         //read content from 
	    		 fread(tempbuffer,BUF_SIZE,1,fopen(argv[1],"r"));
	 		 write(tempSocket, tempbuffer, BUF_SIZE); 
		         printf("9  write tempbuffer context to fuzzer-socket buffer:\n%s\n",tempbuffer);

		       /****************************************************************************************/

		        
		        if((read(clnt_sock, ServerRevBuffer, BUF_SIZE))==0)
		        {
		            printf("Exception1: can not read message in buffer !\n");
		           // break;   
		        } 
		        else
		        {        
		            printf("10  server receive message from client!\n%s\n",ServerRevBuffer);
		        }               
                        
                        //command is exit
                        if(strcmp(ServerRevBuffer,"exit")==0||strcmp(ServerRevBuffer,"EXIT")==0)
                        {
                            printf("branch1: Exiting ask from client\n");
                            exit(0); 
                        }

                        //crash triger
                        if ((ServerRevBuffer[3] == 'c')||(ServerRevBuffer[3] == 'C')){
                             printf("branch2: Trigger crash\n");
                             int *a=NULL;
                             *a = 11111;//a crash point
                            //break;  
                        }

                        // crash2 triger
                        if(ServerRevBuffer[5]=='c'&&ServerRevBuffer[6]=='r'&&ServerRevBuffer[7]=='a'&&ServerRevBuffer[8]=='s'&&ServerRevBuffer[9]=='h')
                        {
                            printf("branch3: Trigger Crash2\n");
                            int *b=NULL;
                            *b = 2222;//a crash poin
                            
                        }

                        //uper
                        for(int j=0;j<strlen(ServerRevBuffer);j++)
                        {
                            ServerRevBuffer[j] = toupper(ServerRevBuffer[j]);  
                        } 
                        printf("branch4: Uper element to client![%s]\n", ServerRevBuffer);                
                        
                   
                    
                }
                
                memset(ServerRevBuffer, 0, BUF_SIZE); 
                memset(tempbuffer, 0, BUF_SIZE); 
        
                close(clnt_sock);
                printf("closing connected socket................\n");
		close(tempSocket);
		printf("closing client socket................\n"); 
		close(serv_sock);   
	    	printf("closing server socket................\n"); 
		exit(0);        
 		
                
           
	

   
     
    return 0;
}
