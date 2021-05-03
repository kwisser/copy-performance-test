#include <chrono>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include<stdio.h>
#include <fcntl.h>
#include <errno.h>


int* workaround;int fd, fd1;
int nread = 1; int start=1;
int nread1 = 1;char buf[10];

void handler(int s)
{
    printf("\ninterrupt by signal handler\n%d\n", nread);
    return;
}

void* help(void * a){
while(start);
pthread_kill((pthread_t)a, SIGALRM);
return nullptr;
}

void* test(void* a){
printf("in cont read thread");
while(nread1>0)nread1 = read(fd, &buf, sizeof(buf));
printf("finished");
return nullptr;
}

void* copyFile( void* help){ 
workaround = &errno;
start = 0;
while(nread>0)
nread = write(STDOUT_FILENO, &buf, sizeof(buf));
return nullptr;
}

int main() { 
//fcntl(fd, F_SETFL, 0);
fd = open("/home/marcel/open_pearl_projektarbeit-main/5mb", O_RDONLY);
fd1 = open("/home/marcel/open_pearl_projektarbeit-main/5mb", O_RDONLY);

pthread_t tt, te, tb; int rc, rd, d;
	
	struct sigaction act;

    	sigemptyset(&act.sa_mask);
   	act.sa_handler = handler;
   	act.sa_flags = 0;
   	
   	sigaction(SIGALRM, &act, NULL);
   	
   	rd = pthread_create(&te, NULL, test, NULL); 
	if (rd) { 
        	printf("Error:unable to create thread, %d\n", rc);
        	_exit(-1);
      	}
   	
   	rc = pthread_create(&tt, NULL, copyFile, NULL); 
	if (rc) { 
        	printf("Error:unable to create thread, %d\n", rc);
        	_exit(-1);
      	}
      	
      	d = pthread_create(&tb, NULL, help, (void*)tt); 
	if (d) { 
        	printf("Error:unable to create thread, %d\n", rd);
        	_exit(-1);
      	}
	
	pthread_join(tt, NULL); 
	printf("\n%d %d\n",nread ,*workaround);
}
