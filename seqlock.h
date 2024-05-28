typedef struct _seqlock_t {
} seqlock_t;
void seqlock_init(seqlock_t *seq);
void seqlock_write_lock(seqlock_t *seq);
void seqlock_write_unlock(seqlock_t *seq);
unsigned seqlock_read_begin(seqlock_t *seq);
unsigned seqlock_read_retry(seqlock_t *seq, unsigned cnt);

void seqlock_init(seqlock_t *seq){

}
void seqlock_write_lock(seqlock_t *seq){

}
void seqlock_write_unlock(seqlock_t *seq){

}
unsigned seqlock_read_begin(seqlock_t *seq){ // counter를 읽는다.

}
unsigned seqlock_read_retry(seqlock_t *seq, unsigned cnt){ // counter값 비교

}

// Writer는 critical section을 진입할 때, 나갈 때 한번씩 counter를 증가시킨다.
// <Reader> counter 값을 읽는다. 그리고 벗어날 때 한번 더 읽어서 두 값이 같으면 중간에 writer가 진입하지 않았다는 것을 의미. 다르면 retry. 
//counter 값이 같은데 홀수다 ? writer가 critical section에 진입한 상태에서 reader가 진입한 것.