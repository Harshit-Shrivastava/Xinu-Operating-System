#include <prodcons.h>
#include <stdlib.h>

//add to queue
//queue must be valid.
void Enqueue(queue *q,pid32 process){
    while(q->next!=NULL){
        q=q->next;
    }
    q->next=(queue*)getmem(sizeof(queue));
    ((queue*)(q->next))->thread=process;
    ((queue*)(q->next))->next=NULL;
}

pid32 check_thread(queue *q){
    return q->thread;
}

//pop first element of queue
void Dequeue(queue *q){
    queue * new=q->next;
    if(new==NULL){
        q->thread=-1;
        return;
    }
    q->next=new->next;
    q->thread=new->thread;
    // freemem(next);
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
