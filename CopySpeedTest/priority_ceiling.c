#define nullptr ((void *)0)
#define _GNU_SOURCE
#include <sched.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

void initiateptattr(pthread_attr_t *pt_attr, struct sched_param *param)
{
    int rc;
    if ((rc = pthread_attr_init(pt_attr)) != 0)
    {
        printf("failed to initiate pt_attr1 with error %s", strerror(rc));
        exit(-1);
    }
    if ((rc = pthread_attr_setinheritsched(pt_attr, PTHREAD_EXPLICIT_SCHED)) != 0)
    {
        printf("das ist %s\n", strerror(rc));
        exit(-1);
    }
    if ((rc = pthread_attr_setschedpolicy(pt_attr, SCHED_FIFO)) != 0)
    {
        printf("%s\n", strerror(rc));
        exit(-1);
    }
    if ((rc = pthread_attr_setschedparam(pt_attr, param)) != 0)
    {
        printf("%s\n", strerror(rc));
        exit(-1);
    }
}
void initiatecore(pthread_t thread, int kern)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(kern, &cpuset);
    if (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset) != 0)
    {
        printf("pthread_setaffinity_np failed\n");
        exit(-1);
    }
}
void *pthread1(void *input)
{
    initiatecore(pthread_self(), 1);
    int c;
    printf("entered threadroutine 1 running on core %d\n", sched_getcpu());
    fflush(stdout);

    pthread_mutex_lock((pthread_mutex_t *)input);
    for (unsigned int i = 0; i < 0xCCCCCCCC; ++i)
    {
        //printf("i at %d\t", i);
        if (i % 0xCCCCCC == 0)
        {
            printf("\033[34mworking in threadroutine 1 i is at: %u on core %d\033[0m\n", i, sched_getcpu());
            fflush(stdout);
        }
    }
    printf("exiting threadroutine 1\n");
    fflush(stdout);
    pthread_mutex_unlock((pthread_mutex_t *)input);
    return nullptr;
}
void *pthread2(void *input)
{
    initiatecore(pthread_self(), 1);
    printf("help\n");
    printf("entered threadroutine 2 running on core %d\n", sched_getcpu());
    fflush(stdout);

    pthread_mutex_lock((pthread_mutex_t *)input);
    for (unsigned int i = 0; i < 0xCCCCCCCC; ++i)
    {
        //printf("i at %d\t", i);
        if (i % 0xCCCCCC == 0)
        {
            printf("\033[31mworking in threadroutine 2 i is at: %u on core %d\033[0m\n", i, sched_getcpu());
            fflush(stdout);
        }
    }
    printf("exiting threadroutine 2\n");
    fflush(stdout);
    pthread_mutex_unlock((pthread_mutex_t *)input);
    return nullptr;
}
void *pthread3(void *input)
{
    fflush(stdout);
    initiatecore(pthread_self(), 1);
    printf("entered threadroutine 3 running on core %d\n", sched_getcpu());

    int c;

    for (unsigned int i = 0; i < 0xCCCCCCCC; ++i)
    {
        //printf("i at %d\t", i);
        if (i % 0xCCCCCC == 0)
            printf("\033[35mworking in threadroutine 3 i is at: %u on core %d\033[0m\n", i, sched_getcpu());
    }

    printf("exiting threadroutine 3\n");
    fflush(stdout);
    return nullptr;
}
void *dauerthread(void *input)
{
    //pthread_self() nur Initiator?
    initiatecore(pthread_self(), 2);
    pthread_mutex_t common_mutex;
    pthread_mutexattr_t mutexattr_common_mutex;

    pthread_t pt[3];
    pthread_attr_t pt_attr1, pt_attr2, pt_attr3;

    int ptid[3];
    int rc, rd, ret;

    struct sched_param param1, param2, param3;
    param1.sched_priority = 90;
    param2.sched_priority = 60;
    param3.sched_priority = 80;

    initiateptattr(&pt_attr1, &param1);
    initiateptattr(&pt_attr2, &param2);
    initiateptattr(&pt_attr3, &param3);

    rc = pthread_mutexattr_init(&mutexattr_common_mutex);
    if (rc != 0)
    {
        printf("%s\n", strerror(rc));
        exit(-1);
    }
    rc = pthread_mutexattr_setprotocol(&mutexattr_common_mutex, PTHREAD_PRIO_PROTECT);
    if (rc != 0)
    {
        printf("%s\n", strerror(rc));
        exit(-1);
    }
    rc = pthread_mutexattr_setprioceiling(&mutexattr_common_mutex, 99);
    if (rc != 0)
    {
        printf("%s\n", strerror(rc));
        exit(-1);
    }
    pthread_mutexattr_getprioceiling(&mutexattr_common_mutex, &rc);
    printf("%d\n", rc);

    rc = pthread_mutex_init(&common_mutex, &mutexattr_common_mutex);
    if (rc != 0)
    {
        printf("%s\n", strerror(rc));
        exit(-1);
    }
    /* thread 2 wird erzeugt thread1 wird erzeugt thread3 thread3 versucht thread 2 zu verdrängen aber thread3 verdrängt main
2kerne
kern1 -> main main erzeigt ERZEUGER-THREAD & sleep(30)
kern2 -> Erzeuger-Thread (höchste Prio kann nicht verdrängt werden)
kern1 ->Thread2 wird erzeugt
Thread1 wird erzeugt und muss warten 

*/
    /*rc = pthread_create(&pt[3], NULL, dauerthread, NULL);
    if (rc != 0)
    {
        printf("%s\n", strerror(rc));
        exit(-1);
    }*/
    sleep(1);
    printf("im about to create thread 2\n");
    ptid[1] = pthread_create(&pt[1], &pt_attr2, pthread2, (void *)&common_mutex);
    if (ptid[1])
    {
        printf("Error:unable to create thread, %d %s\n", ptid[1], strerror(ptid[1]));
        exit(-1);
    }
    for (unsigned int i = 0; i < 0xCCCCCC; ++i)
        ;
    printf("im about to create thread 3\n");
    ptid[2] = pthread_create(&pt[2], &pt_attr2, pthread3, (void *)&common_mutex);
    if (ptid[2])
    {
        printf("Error:unable to create thread, %d %s\n", ptid[2], strerror(ptid[2]));
        exit(-1);
    }
    for (unsigned int i = 0; i < 0xCCCCCC; ++i)
        ;
    printf("im about to create thread 1\n");
    ptid[0] = pthread_create(&pt[0], &pt_attr1, pthread1, (void *)&common_mutex);
    if (ptid[0])
    {
        printf("Error:unable to create thread, %d %s\n", ptid[0], strerror(ptid[0]));
        exit(-1);
    }
    pthread_join(pt[0], NULL);
    pthread_join(pt[1], NULL);
    pthread_join(pt[2], NULL);
    pthread_mutex_destroy(&common_mutex);
    printf("everything is finished\nbye\n");
    exit(0);
    for (; true;)
    {
        /*printf("\033[37m test\033[m");
        sleep(0.01)*/
        ;
    }
}

int main()
{
    initiatecore(pthread_self(), 3);
    pthread_t pt;
    pthread_attr_t pt_attr;
    struct sched_param param;
    int rc;
    param.sched_priority = 99;
    initiateptattr(&pt_attr, &param);
    rc = pthread_create(&pt, NULL, dauerthread, NULL);
    if (rc != 0)
    {
        printf("%s\n", strerror(rc));
        exit(-1);
    }
    pthread_join(pt, NULL);
    return 0;
}