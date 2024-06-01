#include <semaphore.h>
typedef struct _rwwlock_t {
    sem_t readlock;
    sem_t writelock;
    int writers_waiting;
    int readers;
    pthread_mutex_t lock;
} rwwlock_t;
void rwwlock_init(rwwlock_t *rww);
void rwwlock_acquire_writelock(rwwlock_t *rww);
void rwwlock_release_writelock(rwwlock_t *rww);
void rwwlock_acquire_readlock(rwwlock_t *rww);
void rwwlock_release_readlock(rwwlock_t *rww);

void rwwlock_init(rwwlock_t *rww){
  
    sem_init(&rww->readlock,0,1);
    sem_init(&rww->writelock,0,1);
    rww->readers = 0;
    rww->writers_waiting = 0;
    pthread_mutex_init(&rww->lock, NULL);

}
void rwwlock_acquire_writelock(rwwlock_t *rww){
    pthread_mutex_lock(&rww->lock);
    rww->writers_waiting++;
    pthread_mutex_unlock(&rww->lock);

    sem_wait(&rww->writelock);

    pthread_mutex_lock(&rww->lock);
    rww->writers_waiting--;
    pthread_mutex_unlock(&rww->lock);
}
void rwwlock_release_writelock(rwwlock_t *rww){
    sem_post(&rww->writelock);

}
void rwwlock_acquire_readlock(rwwlock_t *rww){
     pthread_mutex_lock(&rww->lock);
    if (rww->readers == 0 || rww->writers_waiting > 0) {
        pthread_mutex_unlock(&rww->lock);
        sem_wait(&rww->readlock);
    } else {
        pthread_mutex_unlock(&rww->lock);
    }

    pthread_mutex_lock(&rww->lock);
    rww->readers++;
    if (rww->readers == 1) {
        sem_wait(&rww->writelock);
    }
    pthread_mutex_unlock(&rww->lock);
}
void rwwlock_release_readlock(rwwlock_t *rww){
    pthread_mutex_lock(&rww->lock);
    rww->readers--;
    if (rww->readers == 0) {
        sem_post(&rww->writelock);
        if (rww->writers_waiting > 0) {
            sem_post(&rww->readlock);
        }
    }
    pthread_mutex_unlock(&rww->lock);

}
