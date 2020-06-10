// basicsearch.c: Search for a pattern in a text string.  
//                Uses a single thread with a brute force algorithm.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

int searchPosition(int i);
void * threadSearch(void * arg);

#define TEXT_LEN 1000000
#define MAX_THREADS 10

// The pattern and text strings are global variables
char *pattern;
char text[TEXT_LEN];

// global lock
pthread_mutex_t lock;

// used to keep track of where to start our threads
int nextAvailableLocation = 0;
int result = -1;

int currThread;

void * threadSearcher(void * arg){
    // locks put around variable assignments to ensure that correct value is held at the end
    // for example, we don't want to find the right position then have another thread update nextAvailableLocation before we set this 
    // as the right location  
    while(nextAvailableLocation < TEXT_LEN && result == -1){
        if(searchPosition(nextAvailableLocation)){
            pthread_mutex_lock(&lock);
            result = nextAvailableLocation;
            pthread_mutex_unlock(&lock);
            return; 
        }
        // we want to make sure threads don't inadvertantly start at the same spot
        pthread_mutex_lock(&lock);
        nextAvailableLocation++;
        pthread_mutex_unlock(&lock);
    }
    //    printf("Thread %d looking at location %d\n", currThread, nextAvailableLocation);
    //    if (searchPosition(nextAvailableLocation) == 1) {
    //     printf("found a match\n");
    //       result = nextAvailableLocation;
    //       return;
    //    }
    //    nextAvailableLocation++;

    // we are going to have to pass this function to pthread_create
    // implement searching mechanism
    // use global position variable and increment by 1 each time 

    // use a lock and unlock to update global position variable 
    // we also need to include lock and unlock in the main 

    // every time, we need to update "result" at the top of the program
    // in order to let each thread know to stop 
    return;
}


int main(int argc, char *argv[]) {

   // Step 1: Extract the pattern string from the command line.   

   // save an array of threads; I used 10 threads for my max
   // I chose 10 because I realized that at around this point, it would be redundant to have too many more threads
   pthread_t thread[MAX_THREADS];
   pattern = argv[1];
   int patlen = strlen(pattern);

   // Step 2: Create and populate the text string.
   // Note that it isn't a proper "string" in the C sense,
   // because it doesn't end in '\0'.

   int textlen = TEXT_LEN; // will be overridden by the actual length
   int count = 0;
   while (count < TEXT_LEN) {
       int status = scanf("%c", &text[count]);
       count++;
       if (status == EOF) {
          textlen = count;
          break;
       }
   }

   // Step 3: Search for the pattern in the text string

   int pos;
   int i;

   for(i = 0; i < MAX_THREADS; i++){
        pthread_create(&thread[i], NULL, threadSearcher, NULL);
    }

   for (i=0; i < MAX_THREADS; i++){
        pthread_join(thread[i], NULL);
    }

//    if(result != -1){
//        printf("Position: %d\n", nextAvailableLocation);
//    }

//    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//    for (pos=0; pos<=textlen-patlen; pos++) {
//        pthread_t currentThread;

//        pthread_mutex_lock(&lock);
//        if(result != -1){
//            printf("Position: %d\n", nextAvailableLocation);
//            break;
//        } 
//        pthread_create(&currentThread, NULL, threadSearcher, NULL);
//        pthread_join(currentThread, NULL);
//        pthread_mutex_unlock(&lock);
    //    if (searchPosition(pos) == 1) {
    //       result = pos; 
    //       break;
    //    }

   // our task: rewrite the above code where threads are responsible for communicating
   // don't simply assign each thread a different portion of the array
   // threads must decide position to search based on the ending pos of the previous thread

   // save a global variable that holds the current location to search

   // Step 4: Determine the result and print it

   if (result == -1)
       printf("Pattern not found\n");
   else
       printf("Pattern begins at character %d\n", result);
} 


int searchPosition(int i) {
   int j;
   for (j=0;j<strlen(pattern); j++) 
       if (text[i+j] != pattern[j])
          return 0;
   return 1;
}
				

