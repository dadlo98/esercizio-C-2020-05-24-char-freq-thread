#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> //per tolower

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int counter[256];

void * thread1_function(void * arg) {
    char * text = (char *) arg;
    int length = strlen(text);
    for(int i=0; i<length; i+=2) {
        if(pthread_mutex_lock(&mutex) != 0){
            perror("lock");
            exit(EXIT_FAILURE);
        }
        counter[(int)text[i]]++;
        if(pthread_mutex_unlock(&mutex) != 0){
            perror("lock");
            exit(EXIT_FAILURE);
        }
    }

    return NULL;
}

void * thread2_function(void * arg) {
    char * text = (char *) arg;
    int length = strlen(text);
    for(int i=1; i<length; i+=2) {
        if(pthread_mutex_lock(&mutex) != 0){
            perror("lock");
            exit(EXIT_FAILURE);
        }
        counter[(int)text[i]]++;
        if(pthread_mutex_unlock(&mutex) != 0){
            perror("lock");
            exit(EXIT_FAILURE);
        }
    }

    return NULL;
}

int main(int argc, char * argv[]) {
    pthread_t t1,t2;
    int res;

    char * content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Mattis rhoncus urna neque viverra justo nec ultrices. Pretium quam vulputate dignissim suspendisse in est ante. Vitae congue mauris rhoncus aenean. Blandit cursus risus at ultrices mi. Ut lectus arcu bibendum at varius vel pharetra vel. Etiam non quam lacus suspendisse faucibus interdum posuere. Eget sit amet tellus cras adipiscing enim eu turpis egestas. Lectus magna fringilla urna porttitor rhoncus dolor purus non. Sit amet consectetur adipiscing elit duis tristique sollicitudin nibh. Nec tincidunt praesent semper feugiat nibh. Sapien pellentesque habitant morbi tristique senectus et netus et malesuada.";

    //creazione threads
    res = pthread_create(&t1, NULL, thread1_function, content);
    if(res != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&t2, NULL, thread2_function, content);
    if(res != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    res = pthread_join(t1, NULL);
    if(res != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    res = pthread_join(t2, NULL);
    if(res != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<256; i++) {
        printf("frequenza carattere %d = %d\n", i, counter[i]);
    }

    return 0;
}
