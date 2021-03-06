/*
    Name: Hai Dang Hoang
    Email: goldsea5191@gmail.com

    signal_flaw.c: The program shows that signals cannot be used to
    count the occurrence of events in other process. The parent installs
    a SIGCHLD handler, and then creates some children, each of which runs for 1 second
    and then terminates. In the meantime, the parent waits for a line of input from the
    terminal and then processes it. This processing is modeled by an infinite loop. when
    each child terminates, the kernel notifies the parent by sendin it a SIGCHLD signal.
    The parent catches the SIGCHLD, reaps one child. However, only two of total singals were
    received, and thus the parent only reaped two children. --> it remains some "zombie" child
*/
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAXBUF 1024
static void sigHandler(int sig)
{
  pid_t pid;
  pid = wait(NULL);     // Wait for children terimnal
  if(pid == -1)
  {
      perror("Error wait \n");
      exit(EXIT_FAILURE);
  }

  printf("Handler reaped child %d\n",(int) pid);
  sleep(2);
  return;
}


int main (int argc, char * argv[])
{
  char buf[MAXBUF];
  ssize_t n;
  if(argc > 2)
  {
      fprintf(stderr,"Usage: %s num_child \n",argv[0]);
      exit(EXIT_FAILURE);
  }

  pid_t pid;

  if(signal(SIGCHLD, sigHandler) == SIG_ERR)     // Signal handling for child
  {
      perror("Error Signal \n");
      exit(EXIT_FAILURE);
  }
  for(int i = 0 ;i < atoi(argv[1]); i++)
  {
      pid = fork();
      switch(pid)
      {
          case -1:
                    perror("Error fork !");
                    exit(EXIT_FAILURE);
          case 0:  // Child
                    printf("Hello from child %d \n",(int)getpid());
                    sleep(1);
                    _exit(EXIT_SUCCESS);
          default: break;
      }
  }

  /* Parent waits for terminal input and then processes it */
  if(n= read(STDIN_FILENO,buf,sizeof(buf)) < 0)
  {
        perror("Error read \n");
        exit(EXIT_FAILURE);
  }

   printf("Parent processing input \n");
  while(1)  // Loop forever
  ;
  exit(EXIT_SUCCESS);
}
