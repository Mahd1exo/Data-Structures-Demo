
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

    /*
     * A function pointer for tasks within the thread pool
     * (similar to task_func_t, but let's keep them separate for clarity).
     */
    typedef void (*thread_task_func_t)(void*);

    /*
     * A single task node in the thread pool queue.
     * Renamed from "Task" to "ThreadTask" to avoid collision.
     */
    typedef struct ThreadTask {
        thread_task_func_t func;
        void* arg;
        struct ThreadTask* next;
    } ThreadTask;

    /*
     * The thread pool structure
     */
    typedef struct ThreadPool {
        HANDLE* threads;             // array of thread handles
        size_t thread_count;         // number of threads
        ThreadTask* task_head;       // head of the task linked list
        ThreadTask* task_tail;       // tail of the task linked list

        CRITICAL_SECTION queue_lock; // protects the task queue
        CONDITION_VARIABLE queue_cond; // signaled when new tasks arrive
        BOOL stop;                   // set to TRUE when shutting down
    } ThreadPool;

    /*
     * Initialize the thread pool with `num_threads`.
     * Returns 0 on success, non-zero on failure.
     */
    int thread_pool_init(ThreadPool* pool, size_t num_threads);

    /*
     * Enqueue a new task (function + argument) to the thread pool.
     * Returns 0 on success, non-zero on failure.
     */
    int thread_pool_add_task(ThreadPool* pool, thread_task_func_t func, void* arg);

    /*
     * Destroy the thread pool:
     *   - Signals all threads to stop
     *   - Waits for them to finish
     *   - Cleans up memory
     */
    void thread_pool_destroy(ThreadPool* pool);

#ifdef __cplusplus
}
#endif

#endif // THREAD_POOL_H
