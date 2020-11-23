#ifndef _STACK_H
#define _STACK_H
 
/** generic ADT of Stack of integers */
typedef struct stack_t* Stack;
typedef void* Element;
typedef Element (*ElemCopyFunction)(Element);
typedef void (*ElemFreeFunction)(Element);
 
/** possible return values */
typedef enum {
	STACK_SUCCESS, STACK_BAD_ARGUMENT, 	STACK_OPERATION_FAIL, STACK_EMPTY, STACK_FULL
} StackResult;

/**  creates a Stack with maximal capacity of 'maxSize'.
 if fails, returns NULL */
Stack stackCreate(int maxSize, ElemCopyFunction copyFunction,
		ElemFreeFunction freeFunction);
 
/**  releases the memory allocated for the stack */
void stackDestroy(Stack stack);
 
/** inserts an element to the top of the stack.
 Error Codes: STACK_BAD_ARGUMENT if stack is NULL
 STACK_FULL if the stack is full and STACK_OPERATION_ FAIL if
 the supplied copy function fails. */
StackResult stackPush(Stack stack, Element element);

/**  removes the element at the top of the stack.
 Error codes: STACK_BAD_ARGUMENT if stack is NULL
 STACK_EMPTY if the stack is empty  */
StackResult stackPop(Stack stack);
 
/**  writes to 'element' a copy of the last element that was pushed
 Error codes: STACK_BAD_ARGUMENT if stack or number are NULL
 STACK_EMPTY if the stack is empty and STACK_FAIL_OPERATION if the 
 supplied copy function fails */
StackResult stackTop(Stack stack, Element* element); 

/**  returns a flag indicating whether the stack is full (meaning elements cannot be pushed)
 stack must not be NULL */
bool stackIsFull(Stack stack); 
/**  returns a flag indicating whether the stack is empty (meaning elements cannot be popped)
 stack must not be NULL */
bool stackIsEmpty(Stack stack); 
/**  returns the number of elements in the stack.
 stack must not be NULL */
int stackSize(Stack stack); 
#endif
