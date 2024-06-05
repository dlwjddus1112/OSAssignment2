#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

typedef struct _seqlock_t {
    volatile int counter;
    pthread_mutex_t lock;
} seqlock_t;

void seqlock_init(seqlock_t *seq){
    seq->counter = 0;
    pthread_mutex_init(&seq->lock, NULL);
}

void seqlock_write_lock(seqlock_t *seq){
    pthread_mutex_lock(&seq->lock);
    seq->counter++; 
} 

void seqlock_write_unlock(seqlock_t *seq){
    seq->counter++; 
    pthread_mutex_unlock(&seq->lock);
}

unsigned seqlock_read_begin(seqlock_t *seq){
    unsigned cnt;
    
    do {
        cnt = seq->counter; 
    } while (cnt & 1); 
    return cnt;
}

bool seqlock_read_retry(seqlock_t *seq, unsigned cnt){
    return seq->counter != cnt;
}
