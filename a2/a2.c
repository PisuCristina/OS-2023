#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "a2_helper.h"
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>


pthread_mutex_t mutex_p2_p3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_p2_p5 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_p3_p8 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_p4_p7 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_p1_p6 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_p1_p2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_p1_p4 = PTHREAD_MUTEX_INITIALIZER;

void *p1_function(void *arg);
void *p2_function(void *arg);
void *p3_function(void *arg);
void *p4_function(void *arg);
void *p5_function(void *arg);
void *p6_function(void *arg);
void *p7_function(void *arg);
void *p8_function(void *arg);



int main(int argc, char **argv)
{

    init();

    info(BEGIN, 1, 0);
     
    pid_t pid1 = getpid();
   
        p1_function(NULL);
        exit(0);
    

    waitpid(pid1, NULL, 0);

   

    return 0;
}

void *p1_function(void *arg)
{


    pid_t pid2, pid4, pid6;

 
    pthread_mutex_lock(&mutex_p1_p2);
    pid2 = fork();
    if (pid2 == 0) {
        p2_function(NULL);
        exit(0);
    }
    pthread_mutex_unlock(&mutex_p1_p2);

 
    pthread_mutex_lock(&mutex_p1_p4);
    pid4 = fork();
    if (pid4 == 0) {

        p4_function(NULL);
        exit(0);
    }
    pthread_mutex_unlock(&mutex_p1_p4);

 
    pid6 = fork();
    if (pid6 == 0) {
        p6_function(NULL);
        exit(0);
    }

    waitpid(pid2, NULL, 0);
    waitpid(pid4, NULL, 0);

 
    info(END, 1, 0);
    return NULL;
}


void *p2_function(void *arg)
{
 
    info(BEGIN, 2, 0);

    pid_t pid3, pid5;

    // create P3 process
    pthread_mutex_lock(&mutex_p2_p3);
    pid3 = fork();
    if (pid3 == 0) {

        p3_function(NULL);
        exit(0);
    }
    pthread_mutex_unlock(&mutex_p2_p3);


    pthread_mutex_lock(&mutex_p2_p5);
    pid5 = fork();
    if (pid5 == 0) {

        p5_function(NULL);
        exit(0);
    }
    pthread_mutex_unlock(&mutex_p2_p5);

    waitpid(pid3, NULL, 0);
    waitpid(pid5, NULL, 0);

    info(END, 2, 0);

    return NULL;
}
void *execThreadT3_2(void *arg) {

    info(BEGIN, 3, 2);

    info(END, 3, 2);

    return NULL;
}

void *execThreadT3_1(void *arg) {

    info(BEGIN, 3, 1);

    pthread_t t2;
    int tNb = 2;
    pthread_create(&t2, NULL, execThreadT3_2, &tNb);
    pthread_join(t2,NULL);

    info(END, 3, 1);

    return NULL;
}


void *execThreadT3_3(void *arg) {
   
    info(BEGIN, 3, 3);



    info(END, 3, 3);

    return NULL;
}

void *execThreadT3_4(void *arg) {
 
    info(BEGIN, 3, 4);


    info(END, 3, 4);

    return NULL;
}
void *p3_function(void *arg)
{
   
    info(BEGIN, 3, 0);

       pthread_t t[4];


    pthread_create(&t[0], NULL, execThreadT3_1, NULL);

    pthread_join(t[1], NULL);

    pthread_create(&t[2], NULL, execThreadT3_3, NULL);
    pthread_create(&t[3], NULL, execThreadT3_4, NULL);

    pthread_join(t[0], NULL);
    pthread_join(t[2], NULL);
    pthread_join(t[3], NULL);

    pid_t pid8;

    pthread_mutex_lock(&mutex_p3_p8);
    pid8 = fork();
    if (pid8 == 0) {
        // P8 process
        p8_function(NULL);
        exit(0);
    }
    pthread_mutex_unlock(&mutex_p3_p8);

    waitpid(pid8, NULL, 0);

    info(END, 3, 0);

    return NULL;
}

void *p4_function(void *arg)
{
 
    info(BEGIN, 4, 0);

    pid_t pid7;

    pid7 = fork();
    if (pid7 == 0) {
        p7_function(NULL);
        exit(0);
    }

 
    waitpid(pid7, NULL, 0);

    info(END, 4, 0);

    return NULL;
}

sem_t sem;

void *execThreadT5_n(void *arg) {
    int threadNumber = *(int *)arg;

    
    info(BEGIN, 5, threadNumber);

 
    info(END, 5, threadNumber);

    sem_post(&sem);

    return NULL;
}

void *p5_function(void *arg) {

    info(BEGIN, 5, 0);

    pthread_t t[46];
    sem_init(&sem, 0, 5);
    
     int *threadNumber12 = malloc(sizeof(int));
    *threadNumber12 = 12;
    sem_wait(&sem);
    pthread_create(&t[12],NULL,execThreadT5_n,threadNumber12);
   

    for (int i = 1; i <= 46; i++) {
          
    if(i == 12){
        pthread_join(t[12], NULL);
    continue;
    } 
    
    int *threadNumber = malloc(sizeof(int));
    *threadNumber = i;
    sem_wait(&sem);

    pthread_create(&t[i], NULL, execThreadT5_n, threadNumber);
     
}


    for (int i = 1; i <= 46; i++) {
     
        pthread_join(t[i], NULL);

        
    }

    info(END, 5, 0);

    return NULL;
}


void *p6_function(void *arg)
{
 
    info(BEGIN, 6, 0);
 
    info(END, 6, 0);

    return NULL;
}



pthread_t t[7];
pthread_mutex_t mutex;


void *execThreadT7_n(void *arg) {
    int threadNumber = *(int *)arg;

    info(BEGIN, 7, threadNumber);
     if(threadNumber == 5){
    pthread_mutex_lock(&mutex); 
     }
    info(END, 7, threadNumber);
    if(threadNumber == 5){
    pthread_mutex_unlock(&mutex); 
    }
    return NULL;
  
}

void *execThreadT7_4(void *arg) {
    
    info(BEGIN, 7, 4);
    pthread_mutex_lock(&mutex); 
    info(END, 7, 4);
    pthread_mutex_unlock(&mutex); 
    return NULL;

}


void *p7_function(void *arg) {
    
    info(BEGIN, 7, 0);

    pthread_mutex_init(&mutex, NULL);
    pthread_create(&t[5], NULL, execThreadT7_n, (void *)&(int){5});
    pthread_join(t[5], NULL);
    pthread_create(&t[4], NULL, execThreadT7_4, NULL);
    pthread_join(t[4], NULL);
    pthread_create(&t[2],NULL,execThreadT7_n,(void *)&(int){2});
    pthread_create(&t[1], NULL, execThreadT7_n, (void *)&(int){1});
    pthread_create(&t[3], NULL, execThreadT7_n, (void *)&(int){3});
    pthread_create(&t[6], NULL, execThreadT7_n, (void *)&(int){6});

 
    for (int i = 1; i <= 6; i++) {
        if(i != 5)
        pthread_join(t[i], NULL);
    }

     pthread_mutex_destroy(&mutex); 


    info(END, 7, 0);

    return NULL;
}

void *p8_function(void *arg)
{
    
    info(BEGIN, 8, 0);

    info(END, 8, 0);

    return NULL;
}

