#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){

	//seguendo la disastrOS_close_resource

	int id = running->syscall_args[0]; //1 prendo l'id 

	if(id<0){
		printf("[SEM] errore id invalido %d \n",id);
		return;
	}

	SemDescriptor* sd= SemDescriptorList_byFd(&running->sem_descriptors, id);

 	if(!sd){
  		 printf("[SEM] errore nella semClose \n");
		 // running->syscall_retvalue=DSOS_ERESOURCECLOSE;
   		 return;
  	}

 	//3 rimuovo il descriptor dalla lista dei processi
 	sd = (SemDescriptor*)List_detach(&running->sem_descriptors, (ListItem*)sd);
  	assert(sd);

 	// prendo la risorsa(semaforo)
  	Semaphore* sem = sd->semaphore;

 	//4 rimuovo il puntatore al descriptor dalla resource list
 	SemDescriptorPtr* sd_ptr = (SemDescriptorPtr*)List_detach(&sem->descriptors, (ListItem*)(sd->ptr));
  	assert(sd_ptr);

  	SemDescriptor_free(sd);
  	SemDescriptorPtr_free(sd_ptr);
  
  	running->syscall_retvalue = 0;

	printf("[SEM] Semaforo con id:%d chiuso. \n",id);

	return;
}
