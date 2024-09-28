// Caso 1:
/*
Thread 1:
S1: if (thd->proc_info) {
    S2: fputs(thd->proc_info,...); ...
}

Thread 2:
S3: thd->proc_info=NULL;
*/

// a) Se acontecer S1, S3, S2 então temos um erro, pois fputs vai receber NULL
// aonde não se deseja (comprovado pelo NULL check).

// b) Uma solução é usar um mutex para lock:

/*
pthread_mutex_t mutex; // isso é inicializado na main antes das threads

Thread 1:

pthread_mutex_lock(&mutex);
S1: if (thd->proc_info) {
    S2: fputs(thd->proc_info,...); ...
}
pthread_mutex_unlock(&mutex);

Thread 2:
pthread_mutex_lock(&mutex);
S3: thd->proc_info=NULL;
pthread_mutex_unlock(&mutex);
*/

/*----------------------------------------------------------------------*/

// Caso 2:
/*
Thread 1:
void init (...) {
    mThread=PR_CreateThread(mMain,...);
    ...
}

Thread 2:
void mMain(...) {
    mState=mThread->State;
    ...
*/

// c) A solução seria aguardar pela condição de inicialização, e na
// inicialização sinalizar essa condição.

/*
pthread_mutex_t mutex; // ambos são inicializados antes das threads na main
pthread_cond_t cond;

Thread 1:
void init (...) {
    pthread_mutex_lock(&mutex);
    mThread=PR_CreateThread(mMain,...);
    ...
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

Thread 2:
void mMain(...) {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond,&mutex);
    mState=mThread->State;
    ...
    pthread_mutex_unlock(&mutex);
*/
