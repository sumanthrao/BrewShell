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

#include "fcntl.h"

#define C_MAX 1024
#define ARG_MAX 5
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

FILE *hist;
int saved_stdout,saved_stdin;
int in, out;
int ou=0,inter=0;
int status,status1;
pid_t pid,pid1;

char **cd_stack;
int pd=0;

char *pre_process(char *inp){
	char *new = (char *)malloc(sizeof(char)*(strlen(inp)+10));
	int k=0,i;
	for(i=0;i<strlen(inp);i++){
		if(inp[i]=='|' || inp[i]=='<' || inp[i]=='>'){
			new[k++]=' ';new[k++]=inp[i];new[k++]=' ';
		}
		else{
			new[k++]=inp[i];
		}
	}
	int j;
	char *line = (char *)malloc(sizeof(char)*(strlen(inp)+10));
	line = new;
	for(i=0;line[i]!='\0';++i){
		while (!( (line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z') || line[i] == '\0' ||line[i] == '|' ||line[i] == '<' || line[i] == '>' || line[i] == '-' || line[i] == ' ' || line[i] == '_' || line[i] == '.' || line[i] == ':'|| line[i]=='/' || line[i]=='0' || line[i]=='1'|| line[i]=='2'|| line[i]=='3'|| line[i]=='4'|| line[i]=='5'|| line[i]=='6'|| line[i]=='7'|| line[i]=='8'|| line[i]=='9')){
			for(j = i; line[j] != '\0'; ++j){
				line[j] = line[j+1];
			}
			line[j] = '\0';
		}
	}
	return line;
}
char **com_n_args(char *inp){
	char **res;
	res = (char **)malloc(sizeof(char *)*ARG_MAX);
	int k=0;
	char *token = strtok(inp," ");
	while(token != NULL){
		res[k]=(char *)malloc(sizeof(char)*100);
		strcpy(res[k++],token);
		token = strtok(NULL," ");
	}
	res[k] = (char *)malloc(sizeof(char)*100);
	res[k] = NULL;
	return res;
}
void pip_pls(char **inp){
	int pipefd[2];
	int m=0,j,r=0;
	char *cmd1 = (char *)malloc(sizeof(char)*100);
	char *cmd2 = (char *)malloc(sizeof(char)*100);
	while( strcmp(inp[m],"|")!=0 ){
		for(j=0;j<strlen(inp[m]);j++){
			cmd1[r++] = inp[m][j];
		}
		cmd1[r++]=' ';
		cmd1[r]='\0';
		m++;
	}
	cmd1[r-1]='\0';
	m++;
	r=0;
	while( inp[m]!=NULL ){
		for(j=0;j<strlen(inp[m]);j++){
			cmd2[r++] = inp[m][j];
		}
		cmd2[r++]=' ';
		cmd2[r]='\0';
		m++;
	}
	cmd2[r-1]='\0';
	char *p_1 = pre_process(cmd1);
	char **list1 = com_n_args(p_1);
	char *p_2 = pre_process(cmd2);
	char **list2 = com_n_args(p_2);
	pipe(pipefd);
	pid1 = fork();
	if (pid1 == 0){
		pid_t pid2 = fork();
		if(pid2==0){
			dup2(pipefd[0],0);
			close(pipefd[1]);
			execvp(*list2,list2);
		}
		else{
			dup2(pipefd[1],1);
			close(pipefd[0]);
			execvp(*list1,list1);
		}
	}
	else{
		waitpid(pid1,&status1,0);
	}
}
char *do_pip_stuff(char **inp){
	int i=0;
	char *new = (char *)malloc(sizeof(char)*(ARG_MAX*100));
	int k=0;
	do{
		if( strcmp(inp[i],">")==0 ){
			char dest[100];
			strcpy(dest,inp[i+1]);
			out = open(dest, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			saved_stdout = dup(1);
			dup2(out, 1);
			close(out);
			ou = 1;
			i++;
		}
		else if( strcmp(inp[i],"<")==0 ){
			char src[100];
			strcpy(src,inp[i+1]);
			in = open(src, O_RDONLY);
			saved_stdin = dup(0);
			dup2(in, 0);
			close(in);
			inter = 1;
			i++;
		}
		else if( strcmp(inp[i],"|")==0 ){
			return "piped";
		}
		else{
			int j;
			for(j=0;j<strlen(inp[i]);j++){
				new[k++] = inp[i][j];
			}
			new[k++]=' ';
		}
		i++;
	}while( inp[i] != NULL );
	new[k]='\0';
	return new;
}
void del(char* str, char ch){
	int i, j = 0;
	int size;
	char ch1;
	char str1[10];
	size = strlen(str);
	for(i=0;i<size;i++){
		if(str[i] != ch){
			ch1 = str[i];
			str1[j] = ch1;
			j++;
		}
	}
	str1[j] = '\0';
}
void do_func(){
	char *input2;
	char* input = (char *)malloc(sizeof(char)*C_MAX) ;
	input2 = (char *)malloc(sizeof(char)*C_MAX);
	//fgets(input2,C_MAX,0);
	gets(input2);
	if(strchr(input2,'\\') != NULL){
		int i, j = 0;
		int size;
		char ch1;
		size = strlen(input2);
		for(i=0;i<size;i++){
			if(input2[i] != '\\'){
				ch1 = input2[i];
				input[j] = ch1;
				j++;
			}
		}
		input[j] = '\0';
		do_func();
	}
  	else{
  		input = input2;
	}
	if(strlen(input)==0)return;
	char *p_input = pre_process(input);
	char **list = com_n_args(p_input);
	p_input = do_pip_stuff(list);
	if( strcmp(p_input,"piped") ==0 ){
		hist = fopen("/home/sumanth/shell/shell_history.txt", "a");
		fprintf(hist,"%s\n",input);
		fclose(hist);
		pip_pls(list);
	}
		list = com_n_args(p_input);

					char *line = malloc(sizeof(char)*100);
					FILE *new = fopen("/home/sumanth/shell/alias.txt","r");
					while( fgets(line,100,new) !=NULL ){
						int z=0,q=0,r=0;
						char *alias = malloc(sizeof(char)*10);
						char *actual = malloc(sizeof(char)*100);
						while(line[z]!=':'){
							alias[q++]=line[z];
							z++;
						}
						alias[q]='\0';
						z++;
						if( strcmp(*list,alias)==0 ){
							while(line[z]!='\0'){
								actual[r++]=line[z];
								z++;
							}
							actual[r-1]='\0';
							strcpy(input,actual);
							p_input = pre_process(input);
							list = com_n_args(p_input);
						}
					}
					fclose(new);

		if(strcmp(*list,"hist")==0){
			char line[100];
			FILE *new = fopen("/home/sumanth/shell/shell_history.txt","r");
			while( fgets(line,100,new) !=NULL ){
				printf("%s",line);
			}
			hist = fopen("/home/sumanth/shell/shell_history.txt", "a");
			fprintf(hist,"hist\n");
			fclose(hist);
			fclose(new);
			return;
		}
		else if(strcmp(*list,"cd")==0){
			cd_stack[pd] = malloc(sizeof(char *)*100);
			getcwd(cd_stack[pd],100);
			printf("\n---cd_stack[%d]=%s---\n",pd,cd_stack[pd]);
			pd++;
			chdir(list[1]);
			hist = fopen("/home/sumanth/shell/shell_history.txt", "a");
			fprintf(hist,"%s\n",input);
			fclose(hist);
		}
		else if(strcmp(*list,"weather")==0){
			pid_t pid6;
			system("python /home/sumanth/shell/weather.py");
			/*if(pid6=fork()<0){
				fprintf(stderr,"awe_shell : can't fork : %s\n",strerror(errno));
			}
			if(pid6==0){
				system("python weather.py");
			}
			int status6;
			if( (pid6=waitpid(pid6,&status6,0)) < 0 ){
				fprintf(stderr,"awe_shell : waitpid error : %s\n",strerror(errno));
			}
			*/


		}
		else if(strcmp(*list,"prevdir")==0){
			chdir(cd_stack[pd-1]);
			pd--;
			hist = fopen("/home/sumanth/shell/shell_history.txt", "a");
			fprintf(hist,"%s\n",input);
			fclose(hist);
		}
		else if(strcmp(*list,"alias")==0){
			FILE *new = fopen("/home/sumanth/shell/alias.txt","a");
			int x = 1;
			while(list[x]!=NULL){
				fprintf(new,"%s ",list[x]);
				x++;
			}
			fprintf(new,"\n");
			hist = fopen("/home/sumanth/shell/shell_history.txt", "a");
			fprintf(hist,"%s\n",input);
			fclose(hist);
			fclose(new);
			return;
		}
		else{
			hist = fopen("/home/sumanth/shell/shell_history.txt", "a");
			fprintf(hist,"%s\n",input);
			fclose(hist);
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
	cd_stack = malloc(sizeof(char *)*10);
	printf(KNRM "\n-----------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf(KYEL "\t\t\t\t\t\t\t\tWELCOME TO 'AWESOME' SHELL [PHASE - 3]\n\t\t");
	printf(KCYN "\t\t\t\t\t\t* Srikar Chundury - 01FB15ECS306\n\t\t");
	printf(KMAG "\t\t\t\t\t\t* Sumanth S Rao - 01FB15ECS314\n\t\t");
	printf(KGRN "\t\t\t\t\t\t* Varun V - 01FB15ECS331");
	printf(KNRM "\n-----------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n");
	while(1){
		char* path = (char *)malloc(sizeof(char)*100);
		getcwd(path,100);
		printf(KRED "awesome_shell_:");
		printf(KBLU "%s",path);
		printf(KRED "$");
		do_func();
		if(ou){
			dup2(saved_stdout, 1);
			close(saved_stdout);
		}
		if(inter){
			dup2(saved_stdin, 0);
			close(saved_stdin);
		}
	}
}
