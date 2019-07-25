#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semOpen(){

	int id = running->syscall_args[0]; //id 
	if(id<0)
		return;

	Semaphore* sem=SemaphoreList_byId(&semaphore_lst,id); //vedo se esiste un sem associato
	if (!sem){
        sem = Semaphore_alloc(id, 1); //in caso lo creo (type=1) e aggiungo in coda
        List_insert(&semaphores_list, semaphores_list.last, (ListItem*) sem);
    	}

	SemDescriptor* sd= SemDescriptor_alloc(running->last_fd,sem,running);
	if(!sd) return;

	running->last_fd++; //incremento fd

	SemDescriptorPtr* sd_ptr=SemDescriptorPtr_alloc(sd);
	List_insert(&running->sem_descriptors, running->sem_descriptors.last,(ListItem*)sd);
	
	//lo aggiungo 
	sd->ptr=sd_ptr;
	List_insert(&running->sem_descriptors,sem->descriptors.last,(ListItem*) sd_ptr);
	
	running->syscall_retvalue = sd->fd;

	return;


	
  // do stuff :)
  // creates a semaphore in the system, having num semnum
  // the semaphore is accessible throughuot the entire system
  // by its id.
  // on success, the function call returns semnum (>=0);
  // in failure the function returns an error code <0
  //- int s=DisastrOS_semOpen(int semnum)


}
