#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
int main(int argc, char *argv[])
{
	int upid= atoi(argv[1]);
	if(argc<2){
	printf(2,"You might have forgot to mention the pid of the process \n");
	exit(-1);
	}
    //printf(1, "Total Number of Open Files: %d\n", nofd());
    //int fd;
    //fd = open("backup", O_CREATE | O_RDWR);
    //printf(1, "Total Number of Open Files: %d\n", nofd());
    //close(fd);
    //printf(1, "Total Number of Open Files: %d\n", nofd());
    //printf(2,"Hello World\n");
    nofd(upid);
    exit(0);
}
