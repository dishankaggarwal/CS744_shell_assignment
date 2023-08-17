#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"


#define MAX 10;
void runcommand(char*, int);
void executecommand(char*,int);
void pipecommand(char* , int, int);
void parallelcommand(char*,int, int);

struct pipecmd{

	char* left;
	char* right;

};



void command(char* cmd ,char* buffer, int size){
	int flag=0;
	if(!strcmp(cmd,"executeCommands")){
				//printf(2,"Reached the child process\n");
				flag=3;
			}
			
			
			//runcommand(buffer,sizeof(buffer));
			//pipecommand("cat README","wc");
			
			for(int i=0;i<sizeof(buffer);i++){
			
				if(flag==3){
				
				executecommand(buffer,sizeof(buffer));
				break;
				}
				else if(buffer[i]=='|'){
					flag=1;
					pipecommand(buffer,sizeof(buffer),i);
				}
				
				else if(buffer[i]==';'){
					flag=2;
					parallelcommand(buffer,sizeof(buffer),i);
				
				}
			}
			if(flag==0){
			
				runcommand(buffer,sizeof(buffer));
			}

}

char ** tokens(char* buf, int size , int* tokno){

	char **tok=(char**)malloc(sizeof(char)*10);
	
	int p;
	int q;
	int r;
	
	for(p=0,q=0,r=0;p<size;p++){
		tok[r]=(char *)malloc(sizeof(char)*100);
		if(buf[p]=='\n'){
		//Putting the null pointer whereever is a space or \n
		
		buf[p]='\0';
		//arg[r++]=&buf[q];
		strcpy(tok[r],(const char *)&buf[q]);
		
		r++;
		//test[r++]=&buf[q];
		q=p+1;
		
		}
		
		//last token
		strcpy(tok[r],(const char *)&buf[q]);
		*tokno=r+1;
		
		//arg[r]=&buf[q]; 
		
		//test[r]=&buf[q];
	}
	
	
	/*while(buf[i]!='\0'){
		tok[r]=(char *)malloc(sizeof(char)*100);
		if(buf[i]=='\n'){
			buf[i]='\0';
			strcpy(tok[r],(const char *)&buf[q]);
			r++;
		}
		q=i+1;
		i++;
		
	
	}
	tok[r]=(char *)malloc(sizeof(char)*100);
	strcpy(tok[r],(const char *)&buf[q]);
	*/
	return tok;
	

}
void writeFile(){
	//printf(2,"reached here");
	char input[80]="cat README\nls\nwc<README";
	int fd2 =open("input.txt",O_WRONLY|O_CREATE);
	write(fd2,input,sizeof(input));
	close(fd2);

}

void executecommand(char *buf, int size){

	int p=strlen("executecommands")+1	;
	//printf(2,"%d %d\n",p,size);
	char file[20];int i=0;
	while(buf[p]!='\n'){
		file[i++]=buf[p];
		p++;
	
	}
		file[i]='\0';
		
		//printf(2,"%s",file);
		writeFile();
		char buffer[100];
		//printf(2,"File created succesfully");
		//char input[80]="cat README\nls";
		int fd =open(file,O_RDONLY);
		if(fd<0){
		printf(2,"file not existed\n");
		return;
		}
		//int fd2 =open("input.txt",O_WRONLY|O_CREATE);
		//write(fd2,input,sizeof(input));
		//int s= lseek(fd , 0, SEEK_END);
		//printf(2,"%d",fd);
		read(fd,buffer,100);
		int tokno=0;
		char** tok = tokens(buffer, strlen(buffer) ,&tokno);
		
		
		for(int i=0;i<tokno;i++){
		
		
		printf(2,"'%s' command executing",tok[i]);
		if(fork()==0)
		runcommand(tok[i],strlen(tok[i]));
		
		wait(0);
		
		}
		
		
	
	

	return;

}
struct pipecmd* pipeTokenizer(char*,int ,int);
void inredircommand(char *buf, int size, int pos){


	struct pipecmd* rcmd = pipeTokenizer(buf ,size ,pos);
	//printf(2,"'%s'\n",rcmd->left);
	//printf(2,"'%s'\n",rcmd->right);
	
	 
	
	
		close(0);
		int file = open(rcmd->right,O_RDONLY);
		dup(file);
		runcommand(rcmd->left,100);
		close(file);
		exit(0);
	
}
void outredircommand(char *buf, int size, int pos){


	struct pipecmd* rcmd = pipeTokenizer(buf ,size ,pos);
	//printf(2,"'%s'\n",rcmd->left);
	//printf(2,"'%s'\n",rcmd->right);
	
	 
	
	
		close(1);
		int file = open(rcmd->right,O_WRONLY|O_CREATE);
		dup(file);
		runcommand(rcmd->left,100);
		close(file);
		exit(0);
	
}


struct pipecmd* pipeTokenizer(char * buf , int size, int position){

	struct pipecmd* p = malloc(sizeof(*p));
	memset(p,0,sizeof(*p));
	//printf(2,"%d",size);
	p->left=&buf[0];
	int q=0;
	if(buf[position+1]=='&')
		q=position+2;
	else{
		q=position+1;
	}
	
	while(buf[q]==' '){
	
		q++;
		
	}
	p->right=&buf[q];
	
	
	int r=position-1;
	
	while(buf[r]==' '){
	
		
		r--;
	}
	buf[r+1]='\0';
	
	//buf[position]='\0';
	//buf[position-1]='\0';
	//buf[position+1]='\0';
	for(int i=q;i<size;i++){
	
		if(buf[i]=='\n'){
		buf[i]='\0';
		break;
		}
	}
	
	
	
	return p;
}

void successive(char* buf, int size,int position){

	struct pipecmd* pcmd =pipeTokenizer(buf,size,position);
	//printf(2,"'%s'",pcmd->left);
	//printf(2,"'%s'",pcmd->right);
	
	int status=2;
	
	if(fork()==0){
	runcommand(pcmd->left,100);
	exit(0);
	}
	int s=wait(&status);
	
	if(s!=0){
	
	if(fork()==0){
	
	runcommand(pcmd->right,100);
	exit(0);
	}
	wait(0);
	}
	
	

}

void parallelcommand(char* buffer, int size, int position){

	struct pipecmd* pcmd =pipeTokenizer(buffer,size,position);
	//printf(2,"%s",pcmd->left);
	//printf(2,"%s",pcmd->right);
	//child one
	int n1,n2;
	n1=fork();
	if(n1==0){
		runcommand(pcmd->left,100);
	}
	
	n2=fork();
	if(n2==0){
	
		runcommand(pcmd->right,100);
	
	}
	wait(0);
	wait(0);
	
	
	//runcommand(pcmd->left,sizeof(pcmd->left));
	//runcommand(pcmd->right,sizeof(pcmd->right));


}


void pipecommand(char* buffer , int size , int position){

	
	
	struct pipecmd* pcmd = pipeTokenizer(buffer,size,position);
	//printf(2,"'%s' '%d' '%d'",pcmd->left, strlen(pcmd->left), sizeof(pcmd->left));
	//printf(2,"'%s' '%d' '%d'",pcmd->right,strlen(pcmd->right),sizeof(pcmd->right));
	//printf(2,"%c",pcmd->right[2]);
	
	//printf(2,"%c",pcmd->right[2]);
	//printf(2,"%c",pcmd->right[2]);

	int pipefd[2];
	if(pipe(pipefd)<0){
		printf(2,"pipe error");
	}
	//child one
	if(fork()==0){
		close(1);

       	dup(pipefd[1]);

       	close(pipefd[0]);

       	close(pipefd[1]);
       	
       	runcommand(pcmd->left , strlen(pcmd->left));
	}
	
	//child two
	if(fork()==0){
		close(0);

       	dup(pipefd[0]);

       	close(pipefd[0]);

       	close(pipefd[1]);
		runcommand(pcmd->right, strlen(pcmd->right));
	}
	close(pipefd[0]);
    	close(pipefd[1]);
	wait(0);
	wait(0);

}

void runcommand(char *buf ,  int bufferSize){
	
	//char *arg[100];
	//printf(2,"'%s'",buf);
	int p,q,r;
	char **test=(char**)malloc(sizeof(char)*10);;
	int size=0;
	//printf(2,"buffersize:%d \n",bufferSize);
	for(int i=0;i<bufferSize;i++){
	size++;
	if(buf[i]=='\n'){
	
		break;
	}
	}
	//printf(2,"buffersize:%d \n",size);
	
	
	//for(int i=0;i<bufferSize;i++){
	
		//if(buf[i]=='|'){
		//	char* left;
		//	char* right;
		
		//}
	
	//}
			//printf(2,"\n%s\n",buf);
	//Loop for tokenizing the buffer received from the commmand line
	
	int inputredirpos=0;
	int outputredirpos=0;
	for(int i=0;i<bufferSize;i++){
	
		if(buf[i]=='<'){
	
			inputredirpos=i;
				
		}
		if(buf[i]=='>'){
			outputredirpos=i;
			
		}
	}
	
	if(inputredirpos!=0){
	
		inredircommand(buf, bufferSize, inputredirpos);
	}
	if(outputredirpos!=0){
	
		outredircommand(buf, bufferSize, outputredirpos);
	}
	
	
	
	
	for(p=0,q=0,r=0;p<size;p++){
		test[r]=(char *)malloc(sizeof(char)*100);
		if(buf[p]==' ' || buf[p]=='\n'){
		//Putting the null pointer whereever is a space or \n
		
		buf[p]='\0';
		//arg[r++]=&buf[q];
		strcpy(test[r],(const char *)&buf[q]);
		r++;
		//test[r++]=&buf[q];
		q=p+1;
		
		}
		
		//last token
		strcpy(test[r],(const char *)&buf[q]);
		
		//arg[r]=&buf[q]; 
		
		//test[r]=&buf[q];
	}
	
		//Exit logic but not
		//printf(2,"%d",strcmp(arg[0],"exit"));
		
		//printf(2,"\n%s %s '%s'\n",arg[0], arg[1] , arg[2]);
	
		//exec(arg[0], arg);
		exec(test[0], test);
		//printf(2,"failed to %s execute" , arg[0]);
		//memset(buf,0,bufferSize);
		return;
}


int getcommand(char *buffer , int bufferSize){

	printf(1,"MyShell>");
	memset(buffer,0,bufferSize);
	gets(buffer,bufferSize);
	if(buffer[0]==0){
		return -1;
	}

	return 0;
}

int main(void){

	static char buffer[100];
	int fd;
	
	// Ensure that three file descriptors are open.
  	while((fd = open("console", O_RDWR)) >= 0){
    		if(fd >= 3){
      			close(fd);
      			break;
    		}
  	}

	while(getcommand(buffer, sizeof(buffer))>=0){
		
		char cmd[20];int p=0;
		for(int i=0;i<strlen(buffer);i++){
		
			if(buffer[i]!=' '&& buffer[i]!='\n' && buffer[i]!='<' && buffer[i]!='>' && buffer[i]!='|' && buffer[i]!=';'){
				
				cmd[p++]=buffer[i];
		
			}
			else
			break;
		}
		cmd[p]='\0';
		
		//printf(2,"%s",cmd);
		if(!strcmp(cmd,"exit")){
		wait(0);
		exit(0);
		}
		
		
		if(strcmp(cmd,"wc")&&strcmp(cmd,"grep")&&strcmp(cmd,"echo")&&strcmp(cmd,"ls")&&strcmp(cmd,"cat")&&strcmp(cmd,"executeCommands")&&strcmp(cmd,"ps")&&strcmp(cmd,"procinfo")){
		
			printf(2,"Illegal command or arguments\n");
			continue;
		}
	
		//if(buffer[0]=='e' && buffer[1]=='x'&&buffer[2]=='i' && buffer[3]=='t'){
		//	exit(0);
		//}
		
		
	
		if(fork()==0){
		
		//command(cmd,buffer,sizeof(buffer));
		
			int flag=0;
			if(!strcmp(cmd,"executeCommands")){
				//printf(2,"Reached the child process\n");
				flag=3;
			}
			
			
			//runcommand(buffer,sizeof(buffer));
			//pipecommand("cat README","wc");
			
			for(int i=0;i<sizeof(buffer);i++){
			
				if(flag==3){
				
				executecommand(buffer,sizeof(buffer));
				break;
				}
				else if(buffer[i]=='|'){
					flag=1;
					pipecommand(buffer,sizeof(buffer),i);
				}
				
				else if(buffer[i]==';'){
					flag=2;
					parallelcommand(buffer,sizeof(buffer),i);
				
				}
				else if(buffer[i]=='&'){
					flag=5;
					if(buffer[i+1]=='&'){
					successive(buffer,sizeof(buffer),i);
					}
					else
					break;
				}
			}
			if(flag==0){
			
				runcommand(buffer,sizeof(buffer));
			}
			
			
			
			
		
		}
		wait(0);
	
	}
	exit(0);
	
}



