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

int main(int argc, char **argv)
{
    //创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(7891);  //端口

    setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,1,BUF_SIZE);
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    //进入监听状态，等待用户发起请求
    listen(serv_sock, 20);
    
    char buffer[BUF_SIZE];
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);	      
    while(1){


    // create a client to send msg to server

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(7891);  //端口
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
   // end client and connect


     
	int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);          
        char str[]="welcome\n";
        write(clnt_sock, str, strlen(str));

   // client read content from stdin or file, and send to server
       char tempbuffer[BUF_SIZE];
       if(fread(tempbuffer, BUF_SIZE, 1, fopen(argv,"r"))<0)
	{
		printf("read file failed!");
	}
       else
	{
		write(sock, tempbuffer, BUF_SIZE); 
	}
     
  
            read(clnt_sock, buffer, BUF_SIZE); 
            if(strcmp(buffer,"exit")==0)
            {
		printf("exit ask from client\n");
                char exit_notice[]="close_connection.....\n";
                write(clnt_sock, exit_notice, strlen(exit_notice));

                close(clnt_sock); 
                close(serv_sock);                  
                exit (0);
	    } 
            else
            {

                printf("server receive msg: %s\n", buffer); 

                for(int j=0;j<sizeof(buffer)-1;j++)
                {
                      buffer[j] = toupper(buffer[j]);  
                }          
                write(clnt_sock, buffer, BUF_SIZE);  
    	        printf("server send msg: %s\n", buffer);
                memset(buffer,0,BUF_SIZE);
            }
   }
    
    return 0;
}
