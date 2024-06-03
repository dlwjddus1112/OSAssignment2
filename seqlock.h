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
    seq->counter++; // 홀수로 설정
}

void seqlock_write_unlock(seqlock_t *seq){
    seq->counter++; // 짝수로 설정
    pthread_mutex_unlock(&seq->lock);
}

unsigned seqlock_read_begin(seqlock_t *seq){
    unsigned cnt;
    do {
        cnt = seq->counter; // atomic read
    } while (cnt & 1); // 홀수면 쓰기 작업 중이므로 반복
    return cnt;
}

bool seqlock_read_retry(seqlock_t *seq, unsigned cnt){
    // 카운터가 변경되었는지 확인
    return seq->counter != cnt;
}
