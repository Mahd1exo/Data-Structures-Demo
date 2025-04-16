
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef void (*thread_task_func_t)(void*);
    typedef struct ThreadTask {
        thread_task_func_t func;
        void* arg;
        struct ThreadTask* next;
    } ThreadTask;

    /* The thread pool structure  */
    typedef struct ThreadPool {
        HANDLE* threads;             
        size_t thread_count;        
        ThreadTask* task_head;       
        ThreadTask* task_tail;      

        CRITICAL_SECTION queue_lock; 
        CONDITION_VARIABLE queue_cond; 
        BOOL stop;                   
    } ThreadPool;

    int thread_pool_init(ThreadPool* pool, size_t num_threads);
    int thread_pool_add_task(ThreadPool* pool, thread_task_func_t func, void* arg);
    void thread_pool_destroy(ThreadPool* pool);

#ifdef __cplusplus
}
#endif

#endif // THREAD_POOL_H
