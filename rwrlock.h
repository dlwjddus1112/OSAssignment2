#include <semaphore.h>
#include <pthread.h>

typedef struct _rwrlock_t {
    sem_t readlock;
    sem_t writelock;
    pthread_mutex_t mutex;
    int readers;

} rwrlock_t;
void rwrlock_init(rwrlock_t *rwr);
void rwrlock_acquire_writelock(rwrlock_t *rwr);
void rwrlock_release_writelock(rwrlock_t *rwr);
void rwrlock_acquire_readlock(rwrlock_t *rwr);
void rwrlock_release_readlock(rwrlock_t *rwr);

void rwrlock_init(rwrlock_t *rwr){
    rwr->readers = 0;
    sem_init(&rwr->readlock,0,1);
    sem_init(&rwr->writelock,0,1);
}
void rwrlock_acquire_writelock(rwrlock_t *rwr){
    sem_wait(&rwr->writelock);
}
void rwrlock_release_writelock(rwrlock_t *rwr){
    sem_post(&rwr->writelock);
}
void rwrlock_acquire_readlock(rwrlock_t *rwr){
    sem_wait(&rwr->readlock); 
    pthread_mutex_lock(&rwr->mutex);  // readers 변수를 보호하기 위해 mutex 사용

    rwr->readers++;
    if(rwr->readers == 1){ // 여태까지 접근한 reader가 없었다.
        sem_wait(&rwr->writelock);
    }
    pthread_mutex_unlock(&rwr->mutex);

    sem_post(&rwr->readlock);
}
void rwrlock_release_readlock(rwrlock_t *rwr){
    sem_wait(&rwr->readlock);
    pthread_mutex_lock(&rwr->mutex);
    rwr->readers--;
    if(rwr->readers == 0){
        sem_post(&rwr->writelock);
    }
    pthread_mutex_unlock(&rwr->mutex);
    sem_post(&rwr->readlock);
}