#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/wait.h>

void error_message(char *msg){
     printf("%s:%s",msg,strerror(errno));
     exit(1);
     }
     
void open_url(char *url1){
       execlp("/usr/bin/firefox","/usr/bin/firefox",url1,NULL);
         error_message("exec failed");
       }

int main(int argc,char* argv[]){
    char *feeds[]={"https://www.computerworld.com/in/index.rss","http://feeds.bbci.co.uk/news/technology/rss.xml","https://rss.nytimes.com/services/xml/rss/nyt/Technology.xml","https://gadgets.ndtv.com/rss"};
    char var[200];
    char *phrase=argv[1];
    int pid_status=0;
    pid_t pid;
    int i=0;
    int fd[2];
   FILE *f=fopen("feed.txt","w");
    if(!f) error_message("file cannot be opened");
    size_t no_of_feeds=sizeof(feeds)/sizeof(*feeds);
  //  if(dup2(fileno(f),1)==-1) error_message("file descriptor change failed");
  if(pipe(fd)==-1) error_message("pipe creation failed");
 
  char line[300];
    for(int i=0;i<no_of_feeds;++i){
        sprintf(var,"RSS_FEED=%s",feeds[i]);
        char *env_var[]={var,NULL};
       pid=fork();
       if(pid==-1) error_message("error creating child process");
       else if(pid==0){
        close(fd[0]);
        dup2(fd[1],1);
        execle("/usr/bin/python","/usr/bin/python","./rssgossip.py","-u",phrase,NULL,env_var);
        fprintf(stderr,"Failed :%s",strerror(errno)); 
       } 
       waitpid(pid,&pid_status,0);
        }
        close(fd[1]);
        dup2(fd[0],0);
        dup2(fileno(f),fd[0]);
        while(fgets(line,300,stdin)){
             if(line[0]=='\t'){
             pid=fork();
             
             if(pid==0)
             open_url(line+1);
             }
             }
             waitpid(pid,&pid_status,0);
     
    
        return 1;
        }
