
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "stack.h"

int main() {
	Stack stack = stackCreate(MAX_INPUT_SIZE, copyInt, freeInt);
	if (stack == NULL) {
		fprintf(stderr, "failed to create stack\n");
		return -1;
	}
 
	int input;
	while (!stackIsFull (stack) && scanf("%d", &input) == 1) {
		if (input != UNDO_LAST_COMMAND) {
			StackResult result = stackPush(stack, &input);
			assert(result == STACK_SUCCESS);
			continue;
		}
 
		StackResult result = stackPop(stack);
		if (result == STACK_EMPTY) {
			printf("Cannot undo\n");
		} else {
			assert(result == STACK_SUCCESS);
			printf("undo\n");
		}
	}

	while (!stackEmpty(stack)) {
		int* number = NULL;
		StackResult result = stackTop(stack, (Element*)&number);
		StackResult result2 = stackPop(stack);
		assert(result == STACK_SUCCESS && result2 == STACK_SUCCESS);
		printf("%d\n", *number);
		freeInt(number);
	}
	stackDestroy(stack);
	return 0;
}

