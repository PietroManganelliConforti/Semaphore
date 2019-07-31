#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semOpen(){

	int id = running->syscall_args[0]; //id 

	if(id<0){
		printf("[SEM] errore sem_id invalido (%d < 0!) \n",id);
		//val di ritorno da aggiungere..		
		return;
	}

	Semaphore* sem=SemaphoreList_byId(&semaphores_list,id); //vedo se esiste un sem associato

	if (!sem){
       		sem = Semaphore_alloc(id, 1); //in caso lo creo (type=1) e aggiungo in coda
			if(!sem) {
				printf("[SEM] errore nell'allocazione del semaforo!\n");
				return;
			}
        List_insert(&semaphores_list, semaphores_list.last, (ListItem*) sem);
    	}	
	

	SemDescriptor* sd= SemDescriptor_alloc(running->last_sem_fd,sem,running);

	if(!sd) {
		printf("[SEM] errore nell'allocazione del semDescriptor!\n");
		return;
	}

	running->last_sem_fd++; //incremento sem_fd

	SemDescriptorPtr* sd_ptr=SemDescriptorPtr_alloc(sd);
	List_insert(&running->sem_descriptors, running->sem_descriptors.last,(ListItem*)sd);
	
	//lo aggiungo 
	sd->ptr=sd_ptr;
	List_insert(&sem->descriptors,sem->descriptors.last,(ListItem*) sd_ptr);
	
	printf("[SEM] semaforo con id: %d creato\n",id);
	
	running->syscall_retvalue = sd->fd;

	return;

}
