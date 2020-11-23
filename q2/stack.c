#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define MAX_INPUT_SIZE 100
#define UNDO_COMMAND "UNDO"
#define MAX_COMMAND_SIZE 80
 
struct stack_t {
	Element* array;
	int nextIndex;
	int maxSize;
	ElemCopyFunction copyElement;
	ElemFreeFunction freeElement;
};

Stack stackCreate(int maxSize, ElemCopyFunction copyFunction, ElemFreeFunction freeFunction) 
{
	if (maxSize <= 0 || !copyFunction || !freeFunction) {
		return NULL;	
	}

	Stack stack = malloc(sizeof(*stack));
	
	if (stack == NULL) {
		return NULL;	
	}
		
	stack->array = malloc(sizeof(Element) * maxSize);
	if (stack->array == NULL) {
		free(stack);
		return NULL;	
        }
	stack->nextIndex = 0;
	stack->maxSize = maxSize;
	stack->copyElement = copyFunction;
	stack->freeElement = freeFunction;
	return stack;
}

StackResult stackPush(Stack stack, Element element) {
	if (stack == NULL) {
		return STACK_BAD_ARGUMENT;
	}
	if (stack->nextIndex >= stack->maxSize) {
		return STACK_FULL;
	}
 
	Element newElement = stack->copyElement(element);
	if (newElement == NULL) {
		return STACK_OPERATION_FAIL;
	}
	assert(stack->nextIndex >= 0 && stack->nextIndex < stack->maxSize);
	stack->array[stack->nextIndex++] = newElement;
	return STACK_SUCCESS;
}

StackResult stackPop(Stack stack) {
	if (stack == NULL) {
		return STACK_BAD_ARGUMENT;
	}
	if (stack->nextIndex < 1) {
		return STACK_EMPTY;
	}
 
	assert(stack->nextIndex > 0 && stack->nextIndex <= stack->maxSize);
	stack->freeElement(stack->array[stack->nextIndex - 1]);
	stack->nextIndex--;
	return STACK_SUCCESS;
}

StackResult stackTop(Stack stack, Element* element) {
	if (stack == NULL || element == NULL) {
		return STACK_BAD_ARGUMENT;
	}
	if (stack->nextIndex < 1) {
		return STACK_EMPTY;
	}
	assert(stack->nextIndex > 0 && stack->nextIndex <= stack->maxSize);
	Element copy = stack->copyElement(stack->array[stack->nextIndex - 1]);
	if (copy == NULL) {
		return STACK_OPERATION_FAIL;
	}
	*element = copy;
	return STACK_SUCCESS;
}

int stackSize(Stack stack) {
	assert(stack);
	return stack->nextIndex;
}

bool stackIsEmpty(Stack stack) {
	assert(stack);
	return stackSize(stack) == 0;
}

bool stackIsFull(Stack stack) {
	assert(stack);
	return stackSize(stack) == stack->maxSize;
}

void stackDestroy(Stack stack) {
	if (stack == NULL) {
		return;
	}
 
	while (!stackIsEmpty(stack)) {
		StackResult result = stackPop(stack);
		assert(result == STACK_SUCCESS);
	}
 
	free(stack->array);
	free(stack);
}