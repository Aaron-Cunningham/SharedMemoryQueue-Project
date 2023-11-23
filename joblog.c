/*
 * Replace the following string of 0s with your student number
 * 220605069
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include "joblog.h"

/* 
 * DO NOT EDIT the new_log_name function. It is a private helper 
 * function provided for you to create a log name from a process 
 * descriptor for use when reading, writing and deleting a log file.
 * 
 * You must work out what the function does in order to use it properly
 * and to clean up after use.
 */
static char* new_log_name(proc_t* proc) {
    static char* joblog_name_fmt = "%s/%.31s%07d.txt";
                                // string format for the name of a log file
                                // declared static to have only one instance

    if (!proc)
        return NULL;

    char* log_name;
            
    asprintf(&log_name, joblog_name_fmt, JOBLOG_PATH, proc->type_label,
        proc->id);

    return log_name;
}

/* 
 * DO NOT EDIT the joblog_init function that sets up the log directory 
 * if it does not already exist.
 */
int joblog_init(proc_t* proc) {
    if (!proc) {
        errno = EINVAL;
        return -1;
    }
        
    int r = 0;
    if (proc->is_init) {
        struct stat sb;
    
        if (stat(JOBLOG_PATH, &sb) != 0) {
            errno = 0;
            r = mkdir(JOBLOG_PATH, 0777);
        }  else if (!S_ISDIR(sb.st_mode)) {
            unlink(JOBLOG_PATH);
            errno = 0;
            r = mkdir(JOBLOG_PATH, 0777);
        }
    }

    joblog_delete(proc);    // in case log exists for proc
    
    return r;
}

/* 
 * TODO: you must implement this function.
 * Hints:
 * - you have to go to the beginning of the line represented
 *      by entry_num to read the required entry
 * - see job.h for a function to create a job from its string representation
 */
job_t* joblog_read(proc_t* proc, int entry_num, job_t* job) {
    //error no set to the value it had on entry
    int entry_error_no = errno;
    if(proc == NULL || entry_num < 0){
        return NULL;
    }
    char* filename = new_log_name(proc);

    if(filename == NULL){
        return NULL;
    }

    FILE* logfile = fopen(filename, "r");

    if(logfile == NULL){
        errno = entry_error_no;
        return NULL;
    }

    //Code adapted from [https://stackoverflow.com/questions/12733105/c-function-that-counts-lines-in-file]
    int lines_in_logfile =0;
    while(!feof(logfile)){
        int ch = fgetc(logfile);
        if(ch == '\n'){
            lines_in_logfile++;
        }
    }
    // Returns the file position indicator to the beginning of the file
    rewind(logfile);

    // A valid entry_num will be in range 0 to N - 1
    if(entry_num > lines_in_logfile - 1){
        return NULL;
    }
    int count = 0;
    char line[512];
    //Code adapted from [https://stackoverflow.com/questions/21114591/c-programming-read-specific-line-from-text-file]
    while (fgets(line, sizeof line, logfile) != NULL){
        if (count == entry_num){
            if(job==NULL){
                // Allocate new memory for job
                job = (job_t*)malloc(sizeof(job_t));
                if (job == NULL) {
                    fclose(logfile);
                    free(job);
                    return NULL;
                }
            }
            str_to_job(line, job);
        }
        count++;
    }
    fclose(logfile);
    return job;

}

/* 
 * TODO: you must implement this function.
 * Hints:
 * - remember new entries are appended to a log file
 * - see the hint for joblog_read
 */
void joblog_write(proc_t* proc, job_t* job) {
    int entry_no = errno;

    if(proc ==NULL || job == NULL){
        errno = entry_no;
        return;
    }


    char* filename = new_log_name(proc);
    if(filename == NULL){
        errno = entry_no;
        return;
    }

    FILE* logfile = fopen(filename, "a");
    if(logfile == NULL){
        errno = entry_no;
        return;
    }


    char* log_entry = malloc(JOB_STR_SIZE);
    if (job_to_str(job, log_entry) == NULL) {
        errno = entry_no;
    }

    //Adds new line character after each input
    fprintf(logfile, "%s\n", log_entry);

    fclose(logfile);
}

/* 
 * TODO: you must implement this function.
 */
void joblog_delete(proc_t* proc) {

    if (proc != NULL) {
        remove(new_log_name(proc));
    }
}