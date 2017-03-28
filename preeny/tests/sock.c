#include <sys/socket.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>

int main()
{
	int s = socket(AF_INET, SOCK_STREAM, 0);
	char buf[1024];
	FILE *zomg = fdopen(s, "w");
        
	assert(send(s, "HI!\n", 4, 0) == 4);
        printf("sock id:%d\n",s);	
        recv(s, buf, 1024, 0);
        printf("You wrote: %s\n", buf);
        recv(s, buf, 1024, 0);
        printf("You wrote second: %s\n", buf);

}
