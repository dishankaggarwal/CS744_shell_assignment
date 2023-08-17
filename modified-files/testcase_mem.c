#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


int main(int argc, char *argv[])
{
	if(argc<2){
	printf(2,"You might have forgot to mention the pid of the process \n");
	exit(-1);
	}
	int upid= atoi(argv[1]);
	
  /*printf(1,"Memory allocated till now: %d bytes\n", memAlloc());
  sleep(10);

  char* x = sbrk(1024);
  printf(1,"Memory allocated till now: %d bytes\n", memAlloc());
  
  //free(x);
  char* y = sbrk(4096);
  printf(1,"Memory allocated till now: %d bytes\n", memAlloc());


  printf(1,"Memory allocated till now: %d bytes\n", memAlloc());
  

  char* z = sbrk(-10);
  printf(1,"Memory allocated till now: %d bytes\n", memAlloc());

  char* a = sbrk(-20000);
  printf(1,"Memory allocated till now: %d bytes\n", memAlloc());
	
	memAlloc(id);


  free(x);
  free(y);
  free(z);  
  free(a);
  */
  memAlloc(upid);
  //printf(2,"test case");
  exit(0);
}
