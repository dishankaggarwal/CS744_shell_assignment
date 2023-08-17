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
    processTimeDetails(upid);
    //printf(2,"processDetails");
    exit(0);
}
