#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semOpen(){
	int aux=0;
	int id = running->syscall_args[0]; //id 
	int counter = running->syscall_args[1]; //counter
	//printf("[SO]open con id:%d,counter:%d\n",id,counter);
	if(id<0){
		printf("[SEMO] errore sem_id invalido (%d < 0!) \n",id);
		running->syscall_retvalue = DSOS_ESEMID;	
		return;
	}
	
	
	Semaphore* sem=SemaphoreList_byId(&semaphores_list,id); //vedo se esiste un sem associato

	if (!sem){
		aux=1;
       		sem = Semaphore_alloc(id, counter); //in caso lo creo (type!=1) e aggiungo in coda
			if(!sem) {
				printf("[SEMO] errore nell'allocazione del semaforo!\n");
				running->syscall_retvalue = DSOS_ESEMALLOC;
				return;
			}
        List_insert(&semaphores_list, semaphores_list.last, (ListItem*) sem);
    	}	
	//printf("[SO]count %d\n",sem->count);

	SemDescriptor* sd= SemDescriptor_alloc(running->last_sem_fd,sem,running);

	if(!sd) {
		printf("[SEMO] errore nell'allocazione del semDescriptor!\n");
		running->syscall_retvalue = DSOS_ESEMDESC;
		return;
	}

	running->last_sem_fd++; //incremento sem_fd

	SemDescriptorPtr* sd_ptr=SemDescriptorPtr_alloc(sd);
	List_insert(&running->sem_descriptors, running->sem_descriptors.last,(ListItem*)sd);
	
	//lo aggiungo 
	sd->ptr=sd_ptr;
	List_insert(&sem->descriptors,sem->descriptors.last,(ListItem*) sd_ptr);
	if(aux==1)
	printf("[Pid:%d][SEMO] semaforo con id: %d, counter: %d creato con successo\n",disastrOS_getpid(),id,counter);
	else
	printf("[Pid:%d][SEMO] semaforo con id: %d, counter: %d aperto con successo\n",disastrOS_getpid(),id,counter);
	running->syscall_retvalue = sd->fd;

	return;

};
