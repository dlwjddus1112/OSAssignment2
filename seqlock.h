#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

typedef struct _seqlock_t {
    volatile int counter;
    sem_t readlock;
    sem_t writelock;
    pthread_mutex_t lock;
} seqlock_t;

void seqlock_init(seqlock_t *seq){
    seq->counter = 0;
    sem_init(&seq->readlock, 0, 1);
    sem_init(&seq->writelock, 0, 1);
    pthread_mutex_init(&seq->lock, NULL);
}

void seqlock_write_lock(seqlock_t *seq){
    
    pthread_mutex_lock(&seq->lock);
    seq->counter++; 
    pthread_mutex_unlock(&seq->lock);
    sem_wait(&seq->writelock);
}

void seqlock_write_unlock(seqlock_t *seq){
    pthread_mutex_lock(&seq->lock);
    seq->counter++; 
    pthread_mutex_unlock(&seq->lock);
    sem_post(&seq->writelock);
}

unsigned seqlock_read_begin(seqlock_t *seq){
    unsigned cnt;
    do {
        cnt = seq->counter; // atomic read
        // 홀수면 쓰기 작업 중이므로 대기
        if (cnt & 1) {
            sem_wait(&seq->readlock);
            sem_post(&seq->readlock);
        }
    } while (cnt & 1); // 홀수면 반복
    return cnt;
}

unsigned seqlock_read_retry(seqlock_t *seq, unsigned cnt){
    // 카운터가 변경되었는지 확인
    return seq->counter = cnt;
}
