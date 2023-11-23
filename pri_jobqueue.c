/*
 * Replace the following string of 0s with your student number
 * 220605069
 */
#include <stdlib.h>
#include <stdbool.h>
#include "pri_jobqueue.h"
#include <string.h>

/* 
 * TODO: you must implement this function that allocates a job queue and 
 * initialise it.
 * Hint:
 * - see job_new in job.c
 */
pri_jobqueue_t* pri_jobqueue_new() {
    // Dynamically assign
    pri_jobqueue_t* new_pri_job = (pri_jobqueue_t*) malloc(sizeof(pri_jobqueue_t));
    // initialise a pri_jobqueue
    pri_jobqueue_init(new_pri_job);
    return new_pri_job;
}

/* 
 * TODO: you must implement this function.
 */
void pri_jobqueue_init(pri_jobqueue_t* pjq) {
    // i set to 0 to start the loop at first pjq
    int i = 0;
    //Checks that pjq isn't null before setting
    if(pjq!=NULL){
        //buf_size to JOB_BUFFER_SIZE
        pjq->buf_size = JOB_BUFFER_SIZE;
        //size to 0
        pjq->size = 0;
    }
    //each job in the buffer to an initial state defined by job_init
    while(i < pjq->buf_size ){
        job_init(&pjq->jobs[i]);
        i++;
    }

    if(pjq == NULL){
        return;
    }
}

/* 
 * TODO: you must implement this function.
 * Hint:
 *      - if a queue is not empty, and the highest priority job is not in the 
 *      last used slot on the queue, dequeueing a job will result in the 
 *      jobs on the queue having to be re-arranged
 *      - remember that the job returned by this function is a copy of the job
 *      that was on the queue
 */
job_t* pri_jobqueue_dequeue(pri_jobqueue_t* pjq, job_t* dst) {

    if(pjq == NULL || pjq->size == 0){
        return NULL;
    }

    //Code adapted from bubble sort method [https://stackoverflow.com/questions/3893937/sorting-an-array-in-c]
    //Sorts array with the highest priority at index 0
    int i, j;
    job_t temp;
    for(i=0; i < pjq->size - 1; i++){
        for(j = 0; j < pjq->size - 1 - i; j++){
            if(pjq->jobs[j].priority > pjq->jobs[j + 1].priority){
                temp = pjq->jobs[j + 1];
                pjq->jobs[j + 1] = pjq->jobs[j];
                pjq->jobs[j] = temp;
            }
        }
    }

    job_t* highest_priority = &pjq->jobs[0];



    //If dst is not NULL, the dequeued job is copied to the job pointed to by dst. (job with the highest priority)
    if(dst!=NULL){
        if(highest_priority == NULL){
            return NULL;
        }
        memcpy(dst, highest_priority, sizeof(job_t));
        // Copies the contents of the last index  to the index of the high priority job after copying
        pjq->jobs[0] = pjq->jobs[pjq->size - 1];
        // Removes last index since it was moved to high priorities old index
        pjq->size--;

    }

    //If dst is NULL, the dequeued job is copied to a new, dynamically allocated job. A pointer to the copy is returned.
    if(dst==NULL){
        job_t* newJob = (job_t*) malloc(sizeof(job_t));
        if(newJob == NULL){
            //Handles failed allocation
            return NULL;
        }
        if(highest_priority == NULL){
            return NULL;
        }
        memcpy(newJob, highest_priority, sizeof(job_t));
        // Copies the contents of the last index  to the index of the high priority job after copying
        pjq->jobs[0] = pjq->jobs[pjq->size - 1];
        // Removes last index since it was moved to high priorities old index
        pjq->size--;
        return newJob;
    }

    return NULL;
}

/* 
 * TODO: you must implement this function.
 * Hints:
 * - if a queue is not full, and if you decide to store the jobs in 
 *      priority order on the queue, enqueuing a job will result in the jobs 
 *      on the queue having to be re-arranged. However, it is not essential to
 *      store jobs in priority order (it simplifies implementation of dequeue
 *      at the expense of extra work in enqueue). It is your choice how 
 *      you implement dequeue (and enqueue) to ensure that jobs are dequeued
 *      by highest priority job first (see pri_jobqueue.h)
 * - remember that the job passed to this function is copied to the 
 *      queue
 */
void pri_jobqueue_enqueue(pri_jobqueue_t* pjq, job_t* job) {

    if(pjq->size >= pjq->buf_size){
        return;
    }
    if(job == NULL || pjq == NULL){
        return;
    }
    if(job->priority==0){
        return;
    }

    // Adds new job to the end of the queue
    //Doesn't matter where job is in the queue as the dequeue sort it
    if(pri_jobqueue_is_full(pjq) == false){
        pjq->jobs[pjq->size] = *job;
        pjq->size++;
        return;
    }
}
   
/* 
 * TODO: you must implement this function.
 */
bool pri_jobqueue_is_empty(pri_jobqueue_t* pjq) {
    if(pjq == NULL || pjq->size == 0){
        return true;
    }

    return false;
}

/* 
 * TODO: you must implement this function.
 */
bool pri_jobqueue_is_full(pri_jobqueue_t* pjq) {
    // Checks if the queue is already full
    if(pjq->size >= pjq->buf_size){
        //returns if case is true
        return true;
    }
    return false;
}

/* 
 * TODO: you must implement this function.
 * Hints:
 *      - remember that the job returned by this function is a copy of the 
 *      highest priority job on the queue.
 *      - both pri_jobqueue_peek and pri_jobqueue_dequeue require copying of 
 *      the highest priority job on the queue
 */
job_t* pri_jobqueue_peek(pri_jobqueue_t* pjq, job_t* dst) {
    if(pjq != NULL || pjq->size!=0){
        return NULL;
    }
    //Code adapted from bubble sort method [https://stackoverflow.com/questions/3893937/sorting-an-array-in-c]
    //Sorts array with the highest priority at index 0
    int i, j;
    job_t temp;
    for(i=0; i < pjq->size - 1; i++){
        for(j = 0; j < pjq->size - 1 - i; j++){
            if(pjq->jobs[j].priority > pjq->jobs[j + 1].priority){
                temp = pjq->jobs[j + 1];
                pjq->jobs[j + 1] = pjq->jobs[j];
                pjq->jobs[j] = temp;
            }
        }
    }

    job_t* highest_priority = &pjq->jobs[0];



    if(dst!=NULL){
        //Copy the highest priority to dst
        memcpy(dst, highest_priority, sizeof(job_t));
    }
    if(dst==NULL){
        //allocate a new job
        dst = (job_t*) malloc(sizeof (job_t));
        if(dst==NULL){
            return NULL;
        }
    }
    //Copy the highest priority to the new job
    memcpy(dst, highest_priority, sizeof (job_t));
    return dst;
}

/* 
 * TODO: you must implement this function.
 */
int pri_jobqueue_size(pri_jobqueue_t* pjq) {
    if(pjq == NULL){
        return 0;
    }
    if(pjq->size != 0 || pjq !=NULL){
        return pjq->size;
    }
    return 0;
}

/* 
 * TODO: you must implement this function.
 */
int pri_jobqueue_space(pri_jobqueue_t* pjq) {


    return pjq->buf_size - pjq->size;
}

/* 
 * TODO: you must implement this function.
 *  Hint:
 *      - see pri_jobqeue_new
 */
void pri_jobqueue_delete(pri_jobqueue_t* pjq) {
    if(pjq!=NULL){
        free(pjq);
    }
}
