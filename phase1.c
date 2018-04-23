/*      	 "awesome_shell" presented by : 
	srikar chundury - 01fb15ecs306  , 
	sumanth s rao  - 01fb15ecs314 , 
	varun venkatesh - 01fb15ecs331
*/
#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sysexits.h>
#include<pthread.h>
#define C_MAX 1024
#define ARG_MAX 5
char **com_n_args(char *inp){
	char **res;
	res = (char **)malloc(sizeof(char *)*ARG_MAX);
	int k=0;
	char *token = strtok(inp," ");
	while(token != NULL){
		res[k]=(char *)malloc(sizeof(char)*10);
		strcpy(res[k++],token);
		token = strtok(NULL," ");
	}
	res[k] = (char *)malloc(sizeof(char)*10);
	res[k] = NULL;
	return res;
}
void do_func(){
	int status;
	pid_t pid;
	char *input;
        input = (char *)malloc(sizeof(char)*C_MAX);
	gets(input);
	char **list = com_n_args(input);
	if(strlen(input)==0)return ;
	if(strcmp(*list,"cd")==0){
		chdir(list[1]);
	}
	else{
		if((pid=fork()) == -1){
			fprintf(stderr,"awe_shell : can't fork : %s\n",strerror(errno));
		}
		else if(pid==0){
		
		
			execvp(*list,list);
			fprintf(stderr,"awe_shell : can't exec : %s\n",strerror(errno));
			exit(EX_DATAERR);
			 
		}
		if( (pid=waitpid(pid,&status,0)) < 0 ){
			fprintf(stderr,"awe_shell : waitpid error : %s\n",strerror(errno));
		}
	}
}
void main(){
	printf("\tWELCOME TO 'AWESOME' SHELL ....\n\n\t\tSrikar Chundury - 01FB15ECS306\n\t\tSumanth S Rao - 01FB15ECS314\n\t\tVarun V-01FB15ECS331\n\n");
	while(1){
		char* path = (char *)malloc(sizeof(char)*100);
		getcwd(path,100);
		printf("awesome_shell_:%s$",path);
		do_func();
	}
}
