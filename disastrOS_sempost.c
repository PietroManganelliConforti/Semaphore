#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semPost(){

  	//1 prendo l'id del semaforo 
	int id = running->syscall_args[0]; 

	if(id<0){
		printf("[ERSEMP] Errore id invalido %d \n",id);
		running->syscall_retvalue = DSOS_ESEMID;
		return;
	}

	//Prendo il relativo descriptor
	SemDescriptor* sd= SemDescriptorList_byFd(&running->sem_descriptors, id);

 	if(!sd){
  		 printf("[ERSEMP] Errore nella semWait del semaforo %d\n",id);
		 running->syscall_retvalue=DSOS_ESEMDESC;
   		 return;
  	}

	//prendo il sem e aumento il contatore
  	Semaphore* sem = sd->semaphore;
	//printf("[SP]%d \n",sem->count);
	sem->count++; 
	//printf("[SP]%d \n",sem->count);	
	//Se il contatore è <=0 ho processi in attesa

	if(sem->count <=0 ){
		//rimuovo il descriptor del primo in attesa dalla relativa lista 
		SemDescriptorPtr* next_sd_ptr = (SemDescriptorPtr*) List_detach(&sem->waiting_descriptors, sem->waiting_descriptors.first);
		List_insert(&sem->descriptors,sem->descriptors.last,(ListItem*)next_sd_ptr);

		//rimuovo il processo dalla lista d'attesa
		List_detach(&waiting_list,(ListItem*)next_sd_ptr->descriptor->pcb);
		
     		//cambio lo stato del processo corrente in ready e lo aggiungo alla relativa lista
    		running->status = Ready;
		List_insert(&ready_list, ready_list.last, (ListItem*)running);

		//eseguo il processo in attesa
		running = next_sd_ptr->descriptor->pcb;
       	}

	printf("[SP]Sempost effettuata dal processo: %d \n",disastrOS_getpid());
	running->syscall_retvalue = 0;
	return;

}
