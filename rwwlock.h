#include <pthread.h>
#include <stdbool.h>

typedef struct _rwwlock_t {
    pthread_mutex_t lock;
    pthread_cond_t readers_cond;
    pthread_cond_t writers_cond;
    int readers;
    int waiting_writers;
    bool writer_active;
} rwwlock_t;

void rwwlock_init(rwwlock_t *rww) {
    pthread_mutex_init(&rww->lock, NULL);
    pthread_cond_init(&rww->readers_cond, NULL);
    pthread_cond_init(&rww->writers_cond, NULL);
    rww->readers = 0;
    rww->waiting_writers = 0;
    rww->writer_active = false;
}

void rwwlock_acquire_writelock(rwwlock_t *rww) {
    pthread_mutex_lock(&rww->lock);
    rww->waiting_writers++;
    while (rww->writer_active || rww->readers > 0) {
        pthread_cond_wait(&rww->writers_cond, &rww->lock);
    }
    rww->writer_active = true;
    rww->waiting_writers--;
    pthread_mutex_unlock(&rww->lock);
}

void rwwlock_release_writelock(rwwlock_t *rww) {
    pthread_mutex_lock(&rww->lock);
    rww->writer_active = false;
    if (rww->waiting_writers > 0) {
        pthread_cond_signal(&rww->writers_cond);
    } else {
        pthread_cond_broadcast(&rww->readers_cond);
    }
    pthread_mutex_unlock(&rww->lock);
}

void rwwlock_acquire_readlock(rwwlock_t *rww) {
    pthread_mutex_lock(&rww->lock);
    while (rww->writer_active || rww->waiting_writers > 0) {
        pthread_cond_wait(&rww->readers_cond, &rww->lock);
    }
    rww->readers++;
    pthread_mutex_unlock(&rww->lock);
}

void rwwlock_release_readlock(rwwlock_t *rww) {
    pthread_mutex_lock(&rww->lock);
    rww->readers--;
    if (rww->readers == 0 && rww->waiting_writers > 0) {
        pthread_cond_signal(&rww->writers_cond);
    }
    pthread_mutex_unlock(&rww->lock);
}
