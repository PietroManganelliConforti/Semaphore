#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){

	//seguendo la disastrOS_close_resource

	int id = running->syscall_args[0]; //1 prendo l'id del semaforo 

	if(id<0){
		printf("[ERSEMC] Errore id invalido %d \n",id);
		return;
	}

	SemDescriptor* sd= SemDescriptorList_byFd(&running->sem_descriptors, id);

 	if(!sd){
  		 printf("[ERSEMC] Errore nella semClose del semaforo\n");
		 // running->syscall_retvalue=DSOS_ERESOURCECLOSE;
   		 return;
  	}

 	//3 rimuovo il descriptor dalla lista dei sem_desc del pcb
 	sd = (SemDescriptor*)List_detach(&running->sem_descriptors, (ListItem*)sd);
  	assert(sd);

 	// prendo la risorsa(semaforo)
  	Semaphore* sem = sd->semaphore;
	if(sem==0){
		printf("[ERSEMC] Errore nella chiusura del semaforo\n");
		return;	
	}
 	//4 rimuovo il puntatore al descriptor dalla resource list
 	SemDescriptorPtr* sd_ptr = (SemDescriptorPtr*)List_detach(&sem->descriptors, (ListItem*)(sd->ptr));
  	assert(sd_ptr);

  	SemDescriptor_free(sd);
  	SemDescriptorPtr_free(sd_ptr);
  

	//rimuovo il semaforo dalla lista
	if(sem->descriptors.size==0 && sem->waiting_descriptors.size==0){
		List_detach(&semaphores_list,(ListItem*)sem);
		Semaphore_free(sem);
		printf("[SEM] Semaforo con id:%d chiuso. \n",id);
		}

	running->syscall_retvalue = 0;

	return;
}
