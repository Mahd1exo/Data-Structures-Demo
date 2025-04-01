#include "route_handlers.h"
#include "route_globals.h"
#include "route_utils.h"  
#include <stdio.h>

/* Forward declarations of registration functions from each route_xxx.c */
void register_linkedlist_endpoints(struct mg_context* ctx);
void register_stack_endpoints(struct mg_context* ctx);
void register_queue_endpoints(struct mg_context* ctx);
void register_tree_endpoints(struct mg_context* ctx);
void register_heap_endpoints(struct mg_context* ctx);
void register_hash_endpoints(struct mg_context* ctx);
void register_bst_endpoints(struct mg_context* ctx);
void register_avl_endpoints(struct mg_context* ctx);
void register_minheap_endpoints(struct mg_context* ctx);
void register_sortedlist_endpoints(struct mg_context* ctx);
void register_circularlist_endpoints(struct mg_context* ctx);


/*
 * Register endpoints by first enqueuing tasks (both data-structure init
 * and endpoint registration) into a TaskQueue, then running them
 * concurrently via a ThreadPool.
 */
void register_endpoints(struct mg_context* ctx) {
    /*
     * 1) Create and initialize a local TaskQueue
     */
    TaskQueue queue;
    task_queue_init(&queue);

    /*
     * 2) Enqueue each data-structure initialization task.
     *    (Casting to (task_func_t) if your init function is void(*)(void*).)
     */
    task_queue_enqueue(&queue, (task_func_t)ll_init, &g_list);
    task_queue_enqueue(&queue, (task_func_t)stk_init, &g_stack);
    task_queue_enqueue(&queue, (task_func_t)que_init, &g_queue);
    task_queue_enqueue(&queue, (task_func_t)tree_init, &g_tree);
    task_queue_enqueue(&queue, (task_func_t)maxheap_init, &g_heap);
    task_queue_enqueue(&queue, (task_func_t)hash_init, &g_hash);
    task_queue_enqueue(&queue, (task_func_t)bst_init, &g_bst);
    task_queue_enqueue(&queue, (task_func_t)avl_init, &g_avl);
    task_queue_enqueue(&queue, (task_func_t)minheap_init, &g_minheap);
    task_queue_enqueue(&queue, (task_func_t)sl_init, &g_sortedlist);
    task_queue_enqueue(&queue, (task_func_t)cl_init, &g_circularlist);

    /*
     * 3) Enqueue each endpoint registration task (pass the mg_context* as arg).
     *    The function signature is usually void register_xxx_endpoints(mg_context*).
     */
    task_queue_enqueue(&queue, (task_func_t)register_linkedlist_endpoints, ctx);
    task_queue_enqueue(&queue, (task_func_t)register_stack_endpoints, ctx);
    task_queue_enqueue(&queue, (task_func_t)register_queue_endpoints, ctx);
    task_queue_enqueue(&queue, (task_func_t)register_tree_endpoints, ctx);
    task_queue_enqueue(&queue, (task_func_t)register_heap_endpoints, ctx);
    task_queue_enqueue(&queue, (task_func_t)register_hash_endpoints, ctx);
    task_queue_enqueue(&queue, (task_func_t)register_bst_endpoints, ctx);
    task_queue_enqueue(&queue, (task_func_t)register_avl_endpoints, ctx);
    task_queue_enqueue(&queue, (task_func_t)register_minheap_endpoints, ctx);
    task_queue_enqueue(&queue, (task_func_t)register_sortedlist_endpoints, ctx);
    task_queue_enqueue(&queue, (task_func_t)register_circularlist_endpoints, ctx);

    /*
     * 4) Initialize a ThreadPool with e.g. 4 worker threads.
     */
    ThreadPool pool;
    if (thread_pool_init(&pool, 4) != 0) {
        fprintf(stderr, "Failed to initialize thread pool.\n");
        return;
    }

    /*
     * 5) Dequeue tasks from the TaskQueue and enqueue them to the ThreadPool.
     *    Each task is then executed by one of the pool’s worker threads.
     */
    void* context = NULL;
    task_func_t task = NULL;
    while ((task = task_queue_dequeue(&queue, &context)) != NULL) {
        thread_pool_add_task(&pool, task, context);
    }

    /*
     * 6) Destroy the ThreadPool, which waits for all enqueued tasks to finish.
     */
    thread_pool_destroy(&pool);

    printf("All data structures initialized and endpoints registered in parallel.\n");
}


/*
 * Internal function to clear all data structures.
 * Uses the generic task queue (from task_queue.h) to enqueue each clear
 * function with its corresponding global variable as context, then executes them.
 */
 /*
  * Clear all data structures by first enqueuing tasks in TaskQueue,
  * then dispatching them to a ThreadPool for concurrent execution.
  */
void clear_all_data_structures(void) {
    // 1) Initialize a thread pool with, say, 4 worker threads.
    ThreadPool pool;
    if (thread_pool_init(&pool, 4) != 0) {
        fprintf(stderr, "Failed to initialize thread pool.\n");
        return;
    }

    // 2) Directly add tasks to the thread pool.
    //    Each clear function takes a pointer to a data structure.
    //    Casting to (thread_task_func_t) if needed (depending on your definitions).
    thread_pool_add_task(&pool, (thread_task_func_t)ll_clear, &g_list);
    thread_pool_add_task(&pool, (thread_task_func_t)stk_clear, &g_stack);
    thread_pool_add_task(&pool, (thread_task_func_t)que_clear, &g_queue);
    thread_pool_add_task(&pool, (thread_task_func_t)tree_clear, &g_tree);
    thread_pool_add_task(&pool, (thread_task_func_t)maxheap_clear_null, &g_heap);
    thread_pool_add_task(&pool, (thread_task_func_t)hash_clear, &g_hash);
    thread_pool_add_task(&pool, (thread_task_func_t)bst_clear, &g_bst);
    thread_pool_add_task(&pool, (thread_task_func_t)avl_clear, &g_avl);
    thread_pool_add_task(&pool, (thread_task_func_t)minheap_clear_null, &g_minheap);
    thread_pool_add_task(&pool, (thread_task_func_t)sl_clear, &g_sortedlist);
    thread_pool_add_task(&pool, (thread_task_func_t)cl_clear, &g_circularlist);

    // 3) Destroy the thread pool, which waits for all enqueued tasks to finish.
    thread_pool_destroy(&pool);

    /*
     * Why go directly to the thread pool (skipping TaskQueue)?
     *  1) Simpler if we don't need to store tasks in an intermediate queue.
     *  2) Less code to maintain (no extra enqueuing/dequeuing).
     *  3) Each clear function is scheduled immediately in the pool.
     *
     * However, using a TaskQueue might still be helpful if:
     *  - You want more advanced scheduling or logging,
     *  - You gather tasks from multiple places before you start them,
     *  - You want to process tasks in some specific order (priority, etc.).
     */
     /*
     * 
     * why?
     * 1.Resource Management.
     * 2.Potential for Asynchronous or Multi-threaded Execution.
     * 3.Centralized Error Handling and Monitoring.
     * 4.Dynamic Task Management.
     * 5.Flexibility and Extensibility.
     * 6.Decoupling Task Scheduling from Execution
     */

}

