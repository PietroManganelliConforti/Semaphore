#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include "disastrOS.h"
#include <assert.h>
#define BUF_SIZE 20

int buf[BUF_SIZE];
int idx_scrittura;
int idx_lettura;
int sem1,sem2;
int idx=1;

// we need this to handle the sleep state
void sleeperFunction(void* args){
  printf("Hello, I am the sleeper, and I sleep %d\n",disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}

void childFunction(void* args){
  	printf("Hello, I am the child function of: %d\n",disastrOS_getpid());

	sem1=disastrOS_semOpen(1,0); //semaforo scrittori
	sem2=disastrOS_semOpen(2,10); //semaforo lettori
	printf("-------------\n");
	
	for(int i=0;i< 10;i++){ //10

	if(disastrOS_getpid()%2==0){ //scrive

		disastrOS_semWait(sem2);
	
		buf[idx_scrittura] = idx;
 		printf("[Processo %d] Scritto %d in buf[%d]\n", disastrOS_getpid(), idx, idx_scrittura);
  		idx_scrittura+= 1;
		idx_scrittura%= BUF_SIZE;
  		idx++;

		//printf("effettuata sw da [[%d]]\n",disastrOS_getpid());
		disastrOS_semPost(sem1);
	}

	else{ //legge
	

		disastrOS_semWait(sem1);

		printf("[Processo %d] Leggo %d da buf[%d]\n", disastrOS_getpid(), buf[idx_lettura], idx_lettura);
  		idx_lettura+= 1;
		idx_lettura%= BUF_SIZE;

		//printf("effettuata sp da [[%d]] \n",disastrOS_getpid());
		disastrOS_semPost(sem2);

	}

	} 
	disastrOS_semClose(sem1);
  	disastrOS_semClose(sem2);
	printf("-------------\n");

  disastrOS_exit(disastrOS_getpid()+1);
}


void initFunction(void* args) {
	disastrOS_printStatus();
	printf("hello, I am init and I just started\n");
	//disastrOS_spawn(sleeperFunction, 0);
	
	printf("\n--Test funzioni--\n\n");
	
	int sem1=disastrOS_semOpen(3,1);
	int sem2=disastrOS_semOpen(-4,-3); //errore
	int sem3=disastrOS_semOpen(3,1); //apre se già esistente
	disastrOS_semWait(sem1);
	disastrOS_semWait(22); //errore
	disastrOS_semPost(22); //errore
	disastrOS_semPost(sem1);
	disastrOS_semClose(sem1);
  	disastrOS_semClose(sem2); //errore
	disastrOS_semClose(sem3);
	disastrOS_semClose(sem3); //errore
	
  	printf("\n--Fine test--\n\n");
  
  printf("Creo un buffer con tutti i valori a 0..\n[ ");
  for(int i=0; i<BUF_SIZE; i++){
	buf[i]=0;	
	printf("%d ", buf[i]);
  }
  printf("]\n\n");


  printf("I feel like to spawn 10 nice threads\n");
  int alive_children=0;

  for (int i=0; i<10; ++i) {
    //int type=0;
    //int mode=DSOS_CREATE;
    //printf("mode: %d\n", mode);
    //printf("opening resource (and creating if necessary)\n");
    //int fd=disastrOS_openResource(i,type,mode);
    printf("[INIT]creato thread: %d \n", i+1 ); // fd);     ---!!!
    disastrOS_spawn(childFunction, 0);
    alive_children++;
  }

  disastrOS_printStatus();
  int retval;
  int pid;
  while(alive_children>0 && (pid=disastrOS_wait(0, &retval))>=0){ 
    disastrOS_printStatus();
    printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
	   pid, retval, alive_children);
    --alive_children;
  }

  printf("\n\nBuffer a fine esecuzione: \n\n[ ");
  for(int i=0; i<BUF_SIZE; i++){
    printf("%d ", buf[i]);
  }
  printf("]\n\n");
  printf("shutdown!\n");
  disastrOS_shutdown();
}

int main(int argc, char** argv){
  char* logfilename=0;
  if (argc>1) {
    logfilename=argv[1];
  }
  // we create the init process processes
  // the first is in the running variable
  // the others are in the ready queue
  printf("the function pointer is: %p", childFunction);
  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}




