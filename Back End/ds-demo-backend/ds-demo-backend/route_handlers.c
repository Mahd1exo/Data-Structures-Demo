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
    /* Initialize DS if needed */
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

void clear_all_data_structures(void) {
    ll_clear(&g_list);
    stk_clear(&g_stack);
    que_clear(&g_queue);
    tree_clear(&g_tree);
    maxheap_clear_null(&g_heap);
    hash_clear(&g_hash);
    bst_clear(&g_bst);
    avl_clear(&g_avl);
    minheap_clear_null(&g_minheap);
    sl_clear(&g_sortedlist);
    cl_clear(&g_circularlist);
}
