#include <semaphore.h>
typedef struct _rwwlock_t {
    sem_t readlock;
    sem_t writelock;
    int writers;
} rwwlock_t;
void rwwlock_init(rwwlock_t *rww);
void rwwlock_acquire_writelock(rwwlock_t *rww);
void rwwlock_release_writelock(rwwlock_t *rww);
void rwwlock_acquire_readlock(rwwlock_t *rww);
void rwwlock_release_readlock(rwwlock_t *rww);

void rwwlock_init(rwwlock_t *rww){
    rww->writers = 0;
    sem_init(&rww->readlock,0,1);
    sem_init(&rww->writelock,0,1);
}
void rwwlock_acquire_writelock(rwwlock_t *rww){
    
}
void rwwlock_release_writelock(rwwlock_t *rww){

}
void rwwlock_acquire_readlock(rwwlock_t *rww){
    sem_wait(&rww->readlock);
}
void rwwlock_release_readlock(rwwlock_t *rww){
    sem_post(&rww->readlock);

}
