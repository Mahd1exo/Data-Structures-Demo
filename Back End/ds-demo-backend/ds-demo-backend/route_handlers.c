#include "route_handlers.h"
#include "route_globals.h"
#include "route_utils.h"

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

void register_endpoints(struct mg_context* ctx) {
    /* Initialize data structures if needed */
    ll_init(&g_list);
    stk_init(&g_stack);
    que_init(&g_queue);
    tree_init(&g_tree);
    maxheap_init(&g_heap);
    hash_init(&g_hash);
    bst_init(&g_bst);
    avl_init(&g_avl);
    minheap_init(&g_minheap);
    sl_init(&g_sortedlist);
    cl_init(&g_circularlist);

    /* Call sub-registrations */
    register_linkedlist_endpoints(ctx);
    register_stack_endpoints(ctx);
    register_queue_endpoints(ctx);
    register_tree_endpoints(ctx);
    register_heap_endpoints(ctx);
    register_hash_endpoints(ctx);
    register_bst_endpoints(ctx);
    register_avl_endpoints(ctx);
    register_minheap_endpoints(ctx);
    register_sortedlist_endpoints(ctx);
    register_circularlist_endpoints(ctx);
}

/*
 * Internal function to clear all data structures.
 * Uses the generic task queue (from task_queue.h) to enqueue each clear
 * function with its corresponding global variable as context, then executes them.
 */
void clear_all_data_structures(void) {
    TaskQueue queue;
    task_queue_init(&queue);

    /* Enqueue each clear function with its corresponding context.
       Casting each function to task_func_t (i.e. void (*)(void *)).
       Ensure that each clear function accepts a pointer to the respective data structure. */


    task_queue_enqueue(&queue, (task_func_t)ll_clear, &g_list);
    task_queue_enqueue(&queue, (task_func_t)stk_clear, &g_stack);
    task_queue_enqueue(&queue, (task_func_t)que_clear, &g_queue);
    task_queue_enqueue(&queue, (task_func_t)tree_clear, &g_tree);
    task_queue_enqueue(&queue, (task_func_t)maxheap_clear_null, &g_heap);
    task_queue_enqueue(&queue, (task_func_t)hash_clear, &g_hash);
    task_queue_enqueue(&queue, (task_func_t)bst_clear, &g_bst);
    task_queue_enqueue(&queue, (task_func_t)avl_clear, &g_avl);
    task_queue_enqueue(&queue, (task_func_t)minheap_clear_null, &g_minheap);
    task_queue_enqueue(&queue, (task_func_t)sl_clear, &g_sortedlist);
    task_queue_enqueue(&queue, (task_func_t)cl_clear, &g_circularlist);

    /* Dequeue and execute each task */
    void* context = NULL;
    task_func_t task;
    while ((task = task_queue_dequeue(&queue, &context)) != NULL) {
        task(context);
    }

    /*
    * why?
    *  1.Resource Management.
    *  2.Potential for Asynchronous or Multi-threaded Execution.
    *  3.Centralized Error Handling and Monitoring.
    *  4.Dynamic Task Management.
    *  5.Flexibility and Extensibility.
    *  6.Decoupling Task Scheduling from Execution
    */
}
