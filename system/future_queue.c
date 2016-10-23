#include <prodcons.h>
#include <stdlib.h>

//add to queue
//queue must be valid.
void Enqueue(pid32 pid){
  if(rear==-1 && front==-1) {
    queue[0]=pid;
    front=0; rear=0;
   }        
   else {
     rear++;
     queue[rear]=pid;
   }
}

//pop first element of queue
pid32 Dequeue(){
  pid32 pid=queue[front];
  if(rear==-1 && front==-1) {
    return SYSERR;
  }
  else {
    queue[front]=-1;
    front++;      
   }
  return pid;
}

void nq_set(future *f){
    set_tail +=1;
    set_q[set_tail-1] = *f;
}

void nq_get(future *f){
    get_tail +=1;
    get_q[get_tail-1] = *f;
}

future* dq_set(){
    future *t = &(set_q[set_head]);
    set_head +=1;
    return t;
}

future* dq_get(){
    future *t = &(get_q[get_head]);
    get_head +=1;
    return t;
}
