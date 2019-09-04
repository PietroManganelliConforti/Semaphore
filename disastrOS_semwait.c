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
  		 printf("[ERSEMW] Errore nella semWait del semaforo con id: %d\n",id);
		 running->syscall_retvalue=DSOS_ESEMDESC;
   		 return;
  	}

	//prendo il sem e decremento il contatore
  	Semaphore* sem = sd->semaphore; 
	//printf("[SW]%d \n",sem->count);	
	sem->count--; 
	//printf("[SW]%d \n",sem->count);
	//se il count è <0 metto il processo in attesa
	if(sem->count < 0 ){

		//rimuovo il descriptor del processo corrente e lo metto nella relativa lista d'attesa
		List_detach(&sem->descriptors,(ListItem*)sd->ptr);
		List_insert(&sem->waiting_descriptors, sem->waiting_descriptors.last,(ListItem*)sd->ptr);
		
		//metto il processo corrente in attesa
		running->status=Waiting;
		List_insert(&waiting_list, waiting_list.last, (ListItem*) running);
		
		//prendo il primo della lista ready e lo faccio partire
		running=(PCB*) List_detach(&ready_list, (ListItem*)ready_list.first);
	}

	printf("[SW]SemWait effettuata dal processo: %d \n",disastrOS_getpid());
	running->syscall_retvalue = 0;
	return;
}
