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

#define BUF_SIZE 100

int main()
{
    //创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int optval=1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval,sizeof (optval));
    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(7891);  //端口

    
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    //进入监听状态，等待用户发起请求
    listen(serv_sock, 20);
    
    char buffer[BUF_SIZE];
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);

 	    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);          
            char str[]="welcome, plese login in firstly\n";
            write(clnt_sock, str, strlen(str));
  

            read(clnt_sock, buffer, BUF_SIZE); 

 	    if(buffer!=NULL&&strcmp(buffer,"user")!=0&&strcmp(buffer,"USER")!=0)
            {
               char *res="login failed, please input:user\n";
               write(clnt_sock, res, strlen(res)); 
               close(clnt_sock); 
               return 0;
            }
            else
  
          {
               char *res="login successfully.\n";
               write(clnt_sock, res, strlen(res)); 
            }
            memset(buffer,0,BUF_SIZE);  
             	      
//  while(1)
for (int i=0;i<10;i++)
		{
                        
	                //int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); 
                        read(clnt_sock, buffer, BUF_SIZE); 
			//command is exit
                        if(strcmp(buffer,"exit")==0||strcmp(buffer,"EXIT")==0)
                        {
                            printf("branch1: Exiting ask from client\n");
                            exit(0); 
                        }

                        //crash triger
                        if ((buffer[3] == 'c')||(buffer[3] == 'C')){
                             printf("branch2: Trigger crash\n");
                             int *a=NULL;
                             *a = 11111;//a crash point
                            //break;  
                        }

                        // crash2 triger
                        if(buffer[5]=='c'&&buffer[6]=='r'&&buffer[7]=='a'&&buffer[8]=='s'&&buffer[9]=='h')
                        {
                            printf("branch3: Trigger Crash2\n");
                            int *b=NULL;
                            *b = 2222;//a crash poin
                            
                        }

                        //uper
                        for(int j=0;j<strlen(buffer);j++)
                        {
                            buffer[j] = toupper(buffer[j]);  
                        } 
                        printf("branch4: Uper element to client![%s]\n", buffer);

 			write(clnt_sock, buffer, BUF_SIZE);  
    	        	printf("server send msg: %s\n", buffer);
                	memset(buffer,0,BUF_SIZE);                
                       
                   
//  		exit(0); 
    	       }  
    	return 0;
}
