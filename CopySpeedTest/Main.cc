#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <iostream>
#include <chrono>
#include <pthread.h>
#include <string.h>

char FILENAME_5MB[16] = "testfiles/5mb";
char FILENAME_10MB[16] = "testfiles/10mb";
char FILENAME_50MB[16] = "testfiles/50mb";
char FILENAME_100MB[16] = "testfiles/100mb";
char COPIED_FILENAME[16] = "copied";

int *workaround; //name is self explanatory
int fd; //fd stands for file descriptor
int nread = 1; //this variable represents the return value of the io-thread
int start = 1;
char buf[10];
char sourcePath[PATH_MAX];
char targetPath[PATH_MAX];


void* copyFileSpeed( void* help){
    FILE *fileSource, *fileTarget;
    char ch;
    std::chrono::high_resolution_clock::time_point start, end;
    std::chrono::high_resolution_clock::duration difference;
    int read, write, open, close, writeraw, readraw;

    start = std::chrono::high_resolution_clock::now();
    fileSource = fopen(sourcePath, "r");
    end = std::chrono::high_resolution_clock::now();
    if(fileSource == NULL) {
        std::cout<<"\nError Occurred!"; return 0;
    }
    difference = end - start;
    open = std::chrono::duration_cast<std::chrono::microseconds>(difference).count();


    start = std::chrono::high_resolution_clock::now();
    fclose(fileSource);
    end = std::chrono::high_resolution_clock::now();
    difference = end - start;
    close = std::chrono::duration_cast<std::chrono::microseconds>(difference).count();

    // Start Time
    start = std::chrono::high_resolution_clock::now();
    fileSource = fopen(sourcePath, "r");

    ch = fgetc(fileSource);
    while(ch != EOF) {
        ch = fgetc(fileSource);
    }
    fclose(fileSource);

    end = std::chrono::high_resolution_clock::now();
    difference = end - start;
    read = std::chrono::duration_cast<std::chrono::milliseconds>(difference).count();

    start = std::chrono::high_resolution_clock::now();

    fileSource = fopen(sourcePath, "r");

    fileTarget = fopen(targetPath, "w");
    if(fileTarget == NULL) {
        std::cout<<"\nError Occurred!";
        return 0;
    }

    ch = fgetc(fileSource);
    while(ch != EOF) {
        fputc(ch, fileTarget);
        ch = fgetc(fileSource);
    }

    fclose(fileSource);
    fclose(fileTarget);
    end = std::chrono::high_resolution_clock::now();
    difference = end - start;
    write = std::chrono::duration_cast<std::chrono::milliseconds>(difference).count();
    readraw = (float)read - (float)open/1000 - (float)close/1000;
    writeraw = write - read;

    std::cout << "Open Took: " << open << "µs." << std::endl;
    std::cout << "Close Took: " << close << "µs.\n" << std::endl;

    std::cout << "Read Took: " << read << "ms." << std::endl;
    std::cout << "Write Took: " << write << "ms.\n" << std::endl;

    std::cout << "Read_raw Took: " << readraw << "ms" << std::endl;
    std::cout << "Write_raw Took: " << writeraw << "ms" << std::endl;

    std::cout << "\nFile copied successfully." << std::endl;

    return nullptr;
}

void handler(int s) {
    printf("\ninterrupt by signal handler\n%d\n", nread);
    return;
}

void *help(void *a) {
    while (start);
    pthread_kill((pthread_t) a, SIGALRM);
    return nullptr;
}

void *copyFile(void *help) {
    workaround = &errno;
    start = 0;
    while (nread > 0)
        nread = write(STDOUT_FILENO, &buf, sizeof(buf));
    return nullptr;
}

int startCopySpeedTest(){
    std::cout << "Copy File Speed Test" << std::endl;
    pthread_t test_thread; int rc_speed;
    copyFileSpeed(nullptr);
    std::cout << "\n\n\nThreads\n" << std::endl;
    rc_speed = pthread_create(&test_thread, NULL, copyFileSpeed, NULL);
    if (rc_speed) {
        printf("Error:unable to create thread, %d\n", rc_speed);
        exit(-1);
    }
    pthread_join(test_thread, NULL);
    return 0;
}

void getCurrentWorkingDirectory(){
    int user_file_size;
    char cwd[PATH_MAX];

    printf("Choose File Size: 5mb(1) 10mb(2) 50mb(3) 100mb(4)\n");
    scanf("%d",&user_file_size);

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);

        strncpy(sourcePath, cwd, strlen(cwd)-13);
        if(user_file_size == 1){
            strcat(sourcePath, FILENAME_5MB);
        }
        if(user_file_size == 2){
            strcat(sourcePath, FILENAME_10MB);
        }
        if(user_file_size == 3){
            strcat(sourcePath, FILENAME_50MB);
        }
        if(user_file_size == 4){
            strcat(sourcePath, FILENAME_100MB);
        }

        strcat(targetPath, sourcePath);
        strcat(targetPath, COPIED_FILENAME);

        printf("File to Copy: %s\n", sourcePath);
        std::cout << "Target Path: " << targetPath << std::endl;

    } else {
        perror("getcwd() error");
    }
}

int main() {

    getCurrentWorkingDirectory();
    startCopySpeedTest();

    fd = open(sourcePath, O_RDONLY);
    pthread_t tt, te, tb;
    int rc, rd, d;
    struct sigaction act;

    sigemptyset(&act.sa_mask);
    act.sa_handler = handler;
    act.sa_flags = 0;

    sigaction(SIGALRM, &act, NULL);

    rc = pthread_create(&tt, NULL, copyFile, NULL);
    if (rc) {
        printf("Error:unable to create thread, %d\n", rc);
        _exit(-1);
    }

    d = pthread_create(&tb, NULL, help, (void *) tt);
    if (d) {
        printf("Error:unable to create thread, %d\n", rd);
        _exit(-1);
    }

    pthread_join(tt, NULL);
    printf("\n%d %d\n", nread, *workaround);
}
