#include <semaphore.h>
typedef struct _seqlock_t {
    int counter;
    sem_t readlock;
    sem_t writelock;
} seqlock_t;
void seqlock_init(seqlock_t *seq);
void seqlock_write_lock(seqlock_t *seq);
void seqlock_write_unlock(seqlock_t *seq);
unsigned seqlock_read_begin(seqlock_t *seq);
unsigned seqlock_read_retry(seqlock_t *seq, unsigned cnt);

void seqlock_init(seqlock_t *seq){
    seq->counter = 0;
    sem_init(&seq->readlock,0,1);
    sem_init(&seq->writelock,0,1);
}
void seqlock_write_lock(seqlock_t *seq){
    sem_wait(&seq->writelock);
    seq->counter++;
}
void seqlock_write_unlock(seqlock_t *seq){
    sem_post(&seq->writelock);
    seq->counter++;
}
unsigned seqlock_read_begin(seqlock_t *seq){ // counter를 읽는다.
    unsigned cnt;
    do{
        cnt = seq->counter;
        if(cnt % 2 == 1){
            sem_wait(&seq->readlock);
            sem_post(&seq->readlock);
        }

    } while(cnt % 2 == 1);
}
unsigned seqlock_read_retry(seqlock_t *seq, unsigned cnt){ // counter값 비교
    return seq->counter != cnt;
}

// Writer는 critical section을 진입할 때, 나갈 때 한번씩 counter를 증가시킨다.
// <Reader> counter 값을 읽는다. 그리고 벗어날 때 한번 더 읽어서 두 값이 같으면 중간에 writer가 진입하지 않았다는 것을 의미. 다르면 retry. 
//counter 값이 같은데 홀수다 ? writer가 critical section에 진입한 상태에서 reader가 진입한 것.