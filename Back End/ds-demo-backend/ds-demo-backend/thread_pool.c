#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>

static DWORD WINAPI worker_thread(LPVOID arg) {
    ThreadPool* pool = (ThreadPool*)arg;

    for (;;) {
        EnterCriticalSection(&pool->queue_lock);

        // Wait while there's no task and we're not stopping
        while (!pool->stop && (pool->task_head == NULL)) {
            SleepConditionVariableCS(&pool->queue_cond, &pool->queue_lock, INFINITE);
        }

        // If stop is set and no more tasks, exit the loop
        if (pool->stop && (pool->task_head == NULL)) {
            LeaveCriticalSection(&pool->queue_lock);
            break;
        }

        // Dequeue a task
        ThreadTask* task = pool->task_head;
        if (task) {
            pool->task_head = task->next;
            if (pool->task_head == NULL) {
                pool->task_tail = NULL;
            }
        }

        LeaveCriticalSection(&pool->queue_lock);

        // Execute the task if we got one
        if (task) {
            task->func(task->arg);
            free(task);
        }
    }

    return 0;
}

int thread_pool_init(ThreadPool* pool, size_t num_threads) {
    if (!pool || num_threads == 0) {
        return -1;
    }

    pool->thread_count = num_threads;
    pool->stop = FALSE;
    pool->task_head = NULL;
    pool->task_tail = NULL;

    InitializeCriticalSection(&pool->queue_lock);
    InitializeConditionVariable(&pool->queue_cond);

    pool->threads = (HANDLE*)malloc(sizeof(HANDLE) * num_threads);
    if (!pool->threads) {
        fprintf(stderr, "Error: Could not allocate thread handles.\n");
        DeleteCriticalSection(&pool->queue_lock);
        return -1;
    }

    // Create worker threads
    for (size_t i = 0; i < num_threads; i++) {
        pool->threads[i] = CreateThread(
            NULL,
            0,
            worker_thread,
            pool,  // pass the ThreadPool* as arg
            0,
            NULL
        );
        if (!pool->threads[i]) {
            fprintf(stderr, "Error: Failed to create thread %zu.\n", i);
            // For a robust implementation, you would handle partial creation
            return -1;
        }
    }

    return 0;
}

int thread_pool_add_task(ThreadPool* pool, thread_task_func_t func, void* arg) {
    if (!pool || !func) return -1;

    // Create a new ThreadTask
    ThreadTask* new_task = (ThreadTask*)malloc(sizeof(ThreadTask));
    if (!new_task) return -1;

    new_task->func = func;
    new_task->arg = arg;
    new_task->next = NULL;

    // Lock and enqueue the new task
    EnterCriticalSection(&pool->queue_lock);

    if (pool->task_tail == NULL) {
        pool->task_head = new_task;
        pool->task_tail = new_task;
    }
    else {
        pool->task_tail->next = new_task;
        pool->task_tail = new_task;
    }

    // Wake one waiting thread
    WakeConditionVariable(&pool->queue_cond);
    LeaveCriticalSection(&pool->queue_lock);

    return 0;
}

void thread_pool_destroy(ThreadPool* pool) {
    if (!pool) return;

    // Signal all threads to stop
    EnterCriticalSection(&pool->queue_lock);
    pool->stop = TRUE;
    WakeAllConditionVariable(&pool->queue_cond);
    LeaveCriticalSection(&pool->queue_lock);

    // Wait for threads to finish
    for (size_t i = 0; i < pool->thread_count; i++) {
        WaitForSingleObject(pool->threads[i], INFINITE);
        CloseHandle(pool->threads[i]);
    }

    free(pool->threads);

    // Clean up any leftover tasks
    ThreadTask* current = pool->task_head;
    while (current) {
        ThreadTask* temp = current;
        current = current->next;
        free(temp);
    }

    DeleteCriticalSection(&pool->queue_lock);
}
