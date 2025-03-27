#ifndef DS_STACK_H
#define DS_STACK_H

#define STK_MAX_DATA_LEN 64

typedef struct StackNode {
    char data[STK_MAX_DATA_LEN];
    struct StackNode* next;
} StackNode;

typedef struct Stack {
    StackNode* top;
} Stack;

#ifdef __cplusplus
extern "C" {
#endif

    void stk_init(Stack* stack);
    void stk_push(Stack* stack, const char* value);
    void stk_pop(Stack* stack);
	void stk_clear(Stack* stack);

    /* Returns array of char* [stack top is array[0]]
       caller frees array, not strings themselves. */
    char** stk_collect_data(const Stack* stack, int* count);

#ifdef __cplusplus
}
#endif

#endif /* DS_STACK_H */
