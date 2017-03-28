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



int Read_file_to_Socket(void *argv)
{
    int x,tempSocket;
    struct sockaddr_in dest_addr;
    tempSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
    
    printf("2  tempSocket socket is created!\n");
    memset(&dest_addr, 0, sizeof(dest_addr));            //每个字节都用0填充
    dest_addr.sin_family = AF_INET;                      //使用IPv4地址
    dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    dest_addr.sin_port = htons(1234);                    //端口
    printf("3  tempSocket is binded to ip address and port!\n");

    connect(tempSocket, (struct sockaddr*) &dest_addr, sizeof(dest_addr)); 
   
    printf("4  connecting to server!\n") ; 


    //while(1)
    //{
	
    char tempbuffer[BUF_SIZE];
    printf("0  reading files context\n");
    fread(tempbuffer,1,4096,fopen(argv,"r"));
     
      
    printf("1  file context is read to buffer:[%s]\n",tempbuffer);
    write(tempSocket, tempbuffer, BUF_SIZE); 
    printf("5  write tempbuffer context to fuzzer-socket buffer:[%s]\n",tempbuffer);
    //}
        
    close(tempSocket);
    memset(tempbuffer, 0, BUF_SIZE);
    printf("--------cleaning----------------------------------------------------------------------\n");
    return 1;
       
}

int main(int argc, char **argv)
{
    printf("argv 1 is %s\n", argv[1]);
    
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    printf("6  server socket is created!\n");

    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));            //每个字节都用0填充
    serv_addr.sin_family = AF_INET;                      //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);                    //端口
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    printf("7  server socket is bind to ip address and port!\n");

   //接收客户端请求

    printf("------------------------  server is listenning!-------------------------------------\n");
    listen(serv_sock, 10);
        
         
    
    
    
    //for(int i=0;i<1;i++) { 
    // while(1) {
     	
            printf("8  call read_file_to_socket function!\n");
            if(Read_file_to_Socket(argv[1])==0){
                printf("read file failed\n");
                return 0;
            }       
            struct sockaddr_in clnt_addr;
            socklen_t clnt_addr_size = sizeof(clnt_addr);
	        int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); 
            printf("9  accept socket is created, and connection is estabilished!\n");
                               
            // buffer is empty                
            while(1)
            {
                char ServerRevBuffer[BUF_SIZE]; 
                memset(ServerRevBuffer, 0, BUF_SIZE);  
                printf("empty ![%s]\n",ServerRevBuffer);
                //string read_stream;
                
                if((read(clnt_sock, ServerRevBuffer, BUF_SIZE))==0)
                {
                    printf("16 can not read message in buffer !\n");
                    //fflush(stdout);
                    break;
                } 
                else
                { 
                    
                    printf("10  server receive message from client![%s]\n",ServerRevBuffer);
                    //fflush(stdout);

                }


                //change the buffer content into multiple line string
                char * line = strtok(strdup(ServerRevBuffer), "\n");
                if(strcmp(line,"user")!=0 && strcmp(line,"USER")!=0)
               // if(line[0]!='u' || line[0]!='U')
                {
                        printf("please input user firstly!\n");
                        break; 
                }

                line  = strtok(NULL, "\n");

                while(line) 
                {
                        //command is exit
                        if(strcmp(line,"exit")==0||strcmp(line,"EXIT"))
                        {
                            printf("11 Exiting ask from client\n");
                            char exit_notice[]="server_closing";
                            write(clnt_sock, exit_notice, strlen(exit_notice));  
                            close(serv_sock); 
                            printf("12 Server closed\n"); 
                            break; 
                        }

                        //crash input
                        //if ((line[3] == 'c' && line[5]=='e')||(line[3] == 'C' && line[5]=='E') ){
                        if ((line[8] == 'c')||(line[8] == 'C')){
                             printf("14 Trigger crash\n");
                            int *a=NULL;
                            *a = 11111;//a crash point
                            break;  
                        }

                        // eash triger
                        if (line[0] == 'x'||line[0] == 'X') 
                        {
                            printf("13 buffer[0] is x\n");
                            break;
                        }


                        for(int j=0;j<256;j++)
                        {
                            line[j] = toupper(line[j]);  
                        } 
                        printf("15 erver send msg to client![%s]\n", line);                

                      // printf("%s\n", line);
                       line  = strtok(NULL, "\n");
                    
                    memset(ServerRevBuffer, 0, BUF_SIZE);  
                    printf("empty ![%s]\n",ServerRevBuffer); 
                             

                }


                //memset(ServerRevBuffer, 0, BUF_SIZE);
                close(clnt_sock);
                printf("closing connect socket................\n"); 
                // break;
           }
   // }       

    close(serv_sock);   
    printf("closing server socket................\n");   
    return 0;
}
