#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


int main(int argc, char *argv[])
{
    if(argc<2){
	printf(2,"You may have forgot to mention the pid of the process \n");
	exit(-1);
	}
    int upid= atoi(argv[1]);
    
    procinfo(upid);
    
    
    exit(0);
}
