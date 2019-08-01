#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait(){

  	//1 prendo l'id del semaforo 
	int id = running->syscall_args[0]; 

	if(id<0){
		printf("[ERSEMW] Errore id invalido %d \n",id);
		running->syscall_retvalue = DSOS_ESEMID;
		return;
	}

	//Prendo il relativo descriptor
	SemDescriptor* sd= SemDescriptorList_byFd(&running->sem_descriptors, id);

 	if(!sd){
  		 printf("[ERSEMW] Errore nella semWait del semaforo %d\n",id);
		 running->syscall_retvalue=DSOS_ESEMDESC;
   		 return;
  	}

	//prendo il sem e decremento il contatore
  	Semaphore* sem = sd->semaphore; 
	sem->count--; 

	if(sem->count <=0 ){
		//
	}

	
	running->syscall_retvalue = 0;
	return;
}
