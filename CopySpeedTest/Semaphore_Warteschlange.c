sem_t mutex;

void* thread(void* arg)
{
    sem_wait(&mutex);

    char name[16];
    pthread_getname_np(pthread_self(), name,16);

    printf("\nThread %s Entered..\n", name);
    //critical section

    sleep(6);

    //signal
    printf("\nThread %s Just Exiting...\n", name);
    sem_post(&mutex);
    return nullptr;
}

int main(){
//semaphore_scheduling(){
    sem_init(&mutex, 0, 1);
    pthread_t t1,t2,t3,t4;

    //Thread 1 sperrt Semaphore zuerst
    pthread_create(&t1,NULL,thread,NULL);
    pthread_setname_np(t1,"T1");
    sleep(1);

    //Teste ob Warteschlange existiert
    pthread_create(&t2,NULL,thread,NULL);
    pthread_setname_np(t2,"T2");
    sleep(1);

    pthread_create(&t3,NULL,thread,NULL);
    pthread_setname_np(t3,"T3");
    sleep(1);

    pthread_create(&t4,NULL,thread,NULL);
    pthread_setname_np(t4,"T4");

    //Main soll auf Theads warten
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
    pthread_join(t4,NULL);
    printf("Ende\n");

    return 0;
}
