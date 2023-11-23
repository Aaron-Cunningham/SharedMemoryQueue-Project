/*
 * Replace the following string of 0s with your student number
 * 220605069
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "job.h"

/* 
 * DO NOT EDIT the job_new function.
 */
job_t* job_new(pid_t pid, unsigned int id, unsigned int priority, 
    const char* label) {
    return job_set((job_t*) malloc(sizeof(job_t)), pid, id, priority, label);
}

/* 
 * TODO: you must implement this function
 */
job_t* job_copy(job_t* src, job_t* dst) {

    // If src is NULL, the function returns NULL.
    if (src == NULL) {
        return NULL;
    }
    // If src and dst pointers are identical, i.e. point to the same
    // object in memory, a job pointer is returned without any copying.
    if (src == dst) {
        return dst;
    }

    if (dst == NULL) {
        // Allocate memory for dst
        dst = (job_t *) malloc(sizeof(job_t));
        // checks the memory allocation for dst was successful
        if(dst == NULL){
            return NULL;
        }
        //the job pointed to by src is copied to the new job code apated from [https://www.tutorialspoint.com/c_standard_library/c_function_memcpy.htm]
        memcpy(dst, src, sizeof(job_t));
    }

    if (dst != NULL && dst != src) {
        //If dst is not NULL and is not identical to src, the job pointed to by src is copied to the job pointed to by dst and the dst pointer is returned.
        dst = memcpy(dst, src, sizeof(job_t));
    }
    return dst;
}
/* 
 * TODO: you must implement this function
 */
void job_init(job_t* job) {
    if (job != NULL) {
        job->pid = 0;
        job->id = 0;
        job->priority = 0;
        strcpy(job->label, PAD_STRING);

    }
    if(job == NULL){
        //If job is NULL, this function has no effect.
        return;
    }
}

/* 
 * TODO: you must implement this function
 */
bool job_is_equal(job_t* j1, job_t* j2) {

    if(j1 == NULL && j2 == NULL) {
        return true;
    }

    if(j1 == NULL && j2 != NULL) {
        return false;
    }
    if(j1 != NULL && j2 == NULL){
        return false;
    }


    //strcmp method compares strings, if both strings are same it will result in == 0
    if(j1->id == j2->id && j1->pid == j2->pid && j1->priority == j2->priority && strcmp(j1->label, j2->label)==0) {
        return true;
    }
    return false;
}

/*
 * TODO: you must implement this function.
 * Hint:
 * - read the information in job.h about padding and truncation of job
 *      labels
 */
job_t* job_set(job_t* job, pid_t pid, unsigned int id, unsigned int priority,
               const char* label) {



    if (job == NULL) {
        return NULL;
    }
    if (job->pid == pid && job->id == id && job->priority == priority && strcmp(job->label, label) == 0) {
        return NULL;
    }
    // sets the jobs values
    job->pid = pid;
    job->id = id;
    job->priority = priority;

    //if label is null
    if(label == NULL){
        //copy the pad string
        strcpy(job->label, PAD_STRING);
        return job;
    }

    //copy label to job label
    strncpy(job->label, label, MAX_NAME_SIZE-1);
    //when the length job->label is less than max name size - 1
    for(int i = strlen(job->label); i < MAX_NAME_SIZE - 1; i++ ){
        // add "*" to the end
        strcat(job->label, "*");
    }

    return job;
}

/*
 * TODO: you must implement this function.
 * Hint:
 * - see malloc and calloc system library functions for dynamic allocation, 
 *      and the documentation in job.h for when to do dynamic allocation
 */
char* job_to_str(job_t* job, char* str) {

    //Errors
    if(strlen(job->label) != MAX_NAME_SIZE - 1){
        return NULL;
    }

    //Allocation
    if(str == NULL){
        str = (char*) malloc(JOB_STR_SIZE);
        if(str == NULL){
            //failure of dynamic allocation returns null
            return NULL;
        }
    }

    //Formatting the String Code adapted from [https://stackoverflow.com/questions/804288/creating-c-formatted-strings-not-printing-them]
    snprintf(str, JOB_STR_SIZE, JOB_STR_FMT, job->pid, job->id, job->priority, job->label);
    return str;

}

/*
 * TODO: you must implement this function.
 * Hint:
 * - see the hint for job_to_str
 */
job_t* str_to_job(char* str, job_t* job) {

    //Allocate
    if (job == NULL) {
        //If job is NULL, the memory for the job is dynamically allocated.
        job = (job_t *) malloc(sizeof(job_t));
        if (job == NULL) {
            //failure of dynamic allocation returns null
            return NULL;
        }
    }

    //Errors
    //Code adapted from [https://stackoverflow.com/questions/16013031/ensure-a-string-matches-a-certain-format-in-c]
    if (str == NULL || sscanf(str, JOB_STR_FMT, &(job->pid), &(job->id), &(job->priority), job->label) != 4) {
        // If str is NULL return NULL and any dynamically allocated memory will be deallocated.
        free(job);
        return NULL;
    }

    if (strlen(str) != JOB_STR_SIZE - 1) {
        //if str is not length JOB_STR_SIZE - 1 return NULL and any dynamically allocated memory will be deallocated.
        return NULL;
    }

    if (strlen(job->label) != MAX_NAME_SIZE - 1) {
        // if the label component of the resulting job is not length MAX_NAME_SIZE - 1. return NULL and any dynamically allocated memory will be deallocated.
        return NULL;
    }
    sscanf(str, JOB_STR_FMT, &(job->pid), &(job->id), &(job->priority), job->label);
    return job;
}

/* 
 * TODO: you must implement this function
 * Hint:
 * - look at the allocation of a job in job_new
 */
void job_delete(job_t* job) {

    if(job!=NULL){
        free(job);
    }
}