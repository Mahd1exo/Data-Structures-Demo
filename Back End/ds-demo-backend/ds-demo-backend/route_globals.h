#ifndef ROUTE_GLOBALS_H
#define ROUTE_GLOBALS_H

#include "ds_bst.h"
#include "ds_linkedlist.h"
#include "ds_stack.h"
#include "ds_queue.h"
#include "ds_tree.h"
#include "ds_maxheap.h"
#include "ds_hashtable.h"
#include "ds_avl.h"
#include "ds_minheap.h"
#include "route_globals.h"
#include "ds_sortedlist.h"
#include "ds_circularlist.h"

/* Declare globals as extern */
extern LinkedList g_list;
extern Stack g_stack;
extern Queue g_queue;
extern SimpleTree g_tree;
extern SimpleHashTable g_hash;
extern SimpleBST g_bst;
extern SimpleAVL g_avl;

extern MinHeap g_minheap;
extern MaxHeap g_heap;
extern SortedList g_sortedlist;
extern CircularList g_circularlist;


#endif /* ROUTE_GLOBALS_H */
