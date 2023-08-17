#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


int main(int argc, char *argv[])
{
    
    int upid= atoi(argv[1]);
    procinfo(upid);
    
    
    exit(0);
}
