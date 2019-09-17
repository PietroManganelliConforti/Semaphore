# DisastrOS_semaphore
DisastrOS_semaphores implementation for SO course.

List of system calls added in DisastrOS:

```C
   int DisastrOS_semOpen(int semnum)
```
Creates a semaphore in the system, identified by id semnum and become accessible throughout it.
On success, the function call returns a non negative integer. On failure the function returns an error code (with a negative value),defined in disastrOS_constants.h:  
-`DSOS_ESEMID` (-14) : negative id  
-`DSOS_ESEMDESC` (-15) : invalid semDescriptor  
-`DSOS_ESEMALLOC` (-16) : failed to allocate the semDescriptor  


```C
   int DisastrOS_semClose(int semnum)
```
Close a semaphore referred by semnum, releasing the resources.
Returns 0 on success or an error code (negative value) in case of failure:  
-`DSOS_ESEMID` (-14) : negative id  
-`DSOS_ESEMDESC` (-15) : invalid semDescriptor  
-`DSOS_ENOSEM` (-17) : Semaphore not found  


```C
    int DisastrOS_semWait(int semnum);
```
Takes the semaphore by id and decrements it's counter.
If is <= 0, the caller is put into the wait queue.
Returns 0 on success or an error code (negative value) in case of failure:  
-`DSOS_ESEMID` (-14) : negative id   
-`DSOS_ESEMDESC` (-15) : invalid semDescriptor  
  

```C
   int DisastrOS_semPost(int semnum);
```
Takes the semaphore by id and increments it's counter.
if the semaphore was at 0, and some other thread was waiting,
the thread is resumed.
Returns 0 on success or an error code (negative value) in case of failure:   
-`DSOS_ESEMID` (-14) : negative id    
-`DSOS_ESEMDESC` (-15) : invalid semDescriptor   

# how to run
To compile do
`make`  
then `./disastrOS_test`  
to run the test.  
  
To check memory leaks:  
`valgrind ./disastrOS_test`


