#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Data {
	char *record;
	int type;
};

struct Stack {
	struct Data *data;
    struct Stack *next;
    struct Stack *prev;
};


/*Return data structure (especially for Pop() function)*/
struct PopResult {
	struct Stack *pointer;
	struct Data *data;
};

/*Pseudo-constructor (Creates a new stack with one item)*/
struct Stack *CreateStack(char *_firstElData, int _firstElType) {
    struct Stack *stack = (struct Stack*)malloc(sizeof(struct Stack*));
    struct Data *d = (struct Data*)malloc(sizeof(struct Data*));
    d->record = _firstElData;
    d->type = _firstElType;

    stack->data = d;

    stack->prev = NULL;
    stack->next = NULL;
    return stack;
}

/*Adds a new element to stack*/
struct Stack *Push(struct Stack *_stack, char *_inputData, int _inputType) {
    /*in case when the stack would has a one member*/
	if(_stack != NULL) {
        struct Stack *newItem = (struct Stack*)malloc(sizeof(struct Stack*));
        struct Data *d = (struct Data*)malloc(sizeof(struct Data*));
        //d->data = (char*)calloc(sizeof(char), 256);
        d->record = _inputData;
        d->type = _inputType;

        newItem->data = d; /*set our data*/
        newItem->prev = _stack;
        newItem->next = NULL;

        _stack->next = newItem;

        return newItem;
    }
    /*in case when the stack is empty*/
    else {
        _stack = CreateStack(_inputData, _inputType);
        return _stack;
    }
}

/*Gets last input element from Stack without its removing*/
struct Stack *Peek(struct Stack *_stack) {
	if(_stack != NULL) {
        while(_stack->next != NULL) {
        	_stack = _stack->next;
        	printf("!\n");
        }
        return _stack;
    }
    else
        return NULL;
}

/*Gets top stack's member and removes it*/
struct PopResult *Pop(struct Stack *_stack) {
    if(_stack == NULL)
        return NULL;

    if(_stack->prev != NULL) {
        _stack = Peek(_stack); /*get the first stack's item*/

        struct PopResult *result = (struct PopResult*)malloc(sizeof(struct PopResult*));
        result = NULL;
        printf("%s\n", _stack->data->record);
        result->data = _stack->data;
        /*result->data->record = _stack->data->record;
        result->data->type = _stack->data->type;
        */
        /*delete a connection and free memory*/
        _stack = _stack->prev;
        struct Stack *tmp = _stack->next;
        _stack->next = NULL;

        //free(tmp->data);
        //free(tmp);
        //tmp = NULL;
        _stack->next = NULL;

        result->pointer = Peek(_stack);
        //_stack = NULL;

        return result;
    }
    else {
        struct PopResult *result = (struct PopResult*)malloc(sizeof(struct PopResult*));
        result = NULL;

        result->data = _stack->data;
        /*
        result->data->record = _stack->data->record;
        result->data->type = _stack->data->type;
         */
    	result->pointer = NULL;
    	//free(_stack);
    	//_stack = NULL;

    	return result;
    }
}

/*
struct PopResult *Pop(struct Stack *_stack) {
    if(_stack == NULL)
        return NULL;

    if(_stack->prev != NULL) {
        _stack = Peek(_stack); /*get the first stack's item

        struct PopResult *result = (struct PopResult*)malloc(sizeof(struct PopResult*));
        result->data = _stack->data;

        /*delete a connection and free memory
        _stack = _stack->prev;
        struct Stack *tmp = _stack->next;
        _stack->next = NULL;

        free(tmp);
        result->pointer = Peek(_stack);

        return result;
    }
    else {
        struct PopResult *result = (struct PopResult*)malloc(sizeof(struct PopResult*));
    	result->data = _stack->data;
    	result->pointer = NULL;
    	free(_stack);
        return result;
    }
}
*/

/*Deletes all items and frees memory*/
struct Stack* Clear(struct Stack *_stack) {
    if(_stack != NULL) {
		_stack = Peek(_stack);
		struct Stack *tmp;

		while(_stack->prev != NULL) {
			tmp = _stack;
			_stack = _stack->prev;
			free(tmp);
		}

		/*frees last item*/
		tmp = _stack;
		free(tmp);
    }
    else
    	free(_stack);
    _stack = NULL;

    return _stack;
}

/*Prints elements of stack(Order: from top to low item)*/
void PrintStack(struct Stack *_stack) {
    _stack = Peek(_stack); //get the first stack's element

    if (_stack != NULL) {
        while(_stack->prev != NULL) {
        	printf("%s --- %d, type is %d\n", _stack->data->record, strlen(_stack->data->record), _stack->data->type);
            _stack = _stack->prev;
        }
        printf("%s --- %d, type is %d\n", _stack->data->record, strlen(_stack->data->record), _stack->data->type);
    }
    else
        printf("List is empty!\n");
}

/*Gets a count of stack's items*/
int GetItemsCount(struct Stack *_stack) {
	_stack = Peek(_stack); /*get the first stack's element*/
    int count = 0;

    if (_stack != NULL) {
        while(_stack->prev != NULL) {
            _stack = _stack->prev;
            count++;
        }
        count++;
    }
    _stack = Peek(_stack);

    return count;
}

/*Gets the last element of regarded stack*/
struct Stack *Bottom(struct Stack *_stack) {
	if(_stack != NULL) {
		while(_stack->prev != NULL)
			_stack = _stack->prev;
		return _stack;
	}
	else
		return NULL;
}

/*Provides movements among stack's items*/
struct Stack *MoveTo(struct Stack *_stack, unsigned int _pos) {
	/*in case when the _stack equals NULL*/
	if(_stack == NULL)
		return NULL;

	/*in case when the parameter _pos more than length of the _stack*/
	if(GetItemsCount(_stack) < _pos)
		return _stack;

	/*in case when the _pos is a negative*/
	if(_pos < 0)
		return _stack;

	_stack = Bottom(_stack);
	int counter = 0;

	while(counter != _pos) {
		_stack = _stack->next;
		counter++;
	}
	return _stack;
}

/*Pushes a new element on concrete position*/
struct Stack* PushInside(struct Stack *_stack, unsigned int _pos, char *_inputData, int _inputType) {
	/*in case when the _stack equals NULL*/
	if(_stack == NULL)
		return NULL;

	/*in case when the parameter _pos more than length of the _stack*/
	if(GetItemsCount(_stack) < _pos)
		return _stack;

	_stack = MoveTo(_stack, _pos);
	struct Stack *newItem = (struct Stack*)malloc(sizeof(struct Stack*));
	struct Data *d = (struct Data*)malloc(sizeof(struct Data*));
	d->record = _inputData;
	d->type = _inputType;

	newItem->data = d;

	/*if required position belongs to the last element*/
	if(_pos == 0) {
		newItem->next = _stack;
		newItem->prev = NULL;
		_stack->prev = newItem;

		return newItem;
	}

	/*if required position belongs to the first element*/
	if(_pos == GetItemsCount(_stack) - 1) {
		newItem->prev = _stack;
		newItem->next = NULL;
		_stack->next = newItem;

		return newItem;
	}

	/*in other cases (1 --- 2(between) --- 3)*/
	struct Stack *tmp = _stack->next;
	//1---2
	_stack->next = newItem;
	newItem->prev = _stack;
	//2---3
	tmp->prev = newItem;
	newItem->next = tmp;

	return newItem;
}

/*Gets the first position of required data*/
int FindValue(struct Stack *_stack, char *_searchData) {
	_stack = Bottom(_stack);

	/*in case when the _stack is empty*/
	if(_stack == NULL)
		return -1;

	int pos = 0;
	while(_stack != NULL) {
		if(strcmp(_stack->data->record,_searchData) == 0)
			return pos;
		pos++;
		_stack = _stack->next;
	}

	/*in case when there is no any matches*/
	return -2;
}

/*Removes a concrete item via its position number*/
struct Stack* RemoveAt(struct Stack *_stack, int _pos) {
	/*in case when the parameter _pos more than length of the _stack*/
	if(GetItemsCount(_stack) < _pos)
		return _stack;

	_stack = MoveTo(_stack, _pos);
	struct Stack* tmp;
	tmp = _stack;

	/*Consider a case when we'd like to remove the bottom item of the _stack*/
	if(_pos == 0) {
		_stack = _stack->next;
		_stack->prev = NULL;

		tmp->next = NULL;
		free(tmp);

		return _stack;
	}

	/*Consider a case when we'd like to remove a top item of the _stack*/
	if(_pos == GetItemsCount(_stack) - 1) {
		_stack = _stack->prev;
		_stack->next = NULL;

		tmp->prev = NULL;
		free(tmp);

		return _stack;
	}

	/*in other cases*/
	_stack = _stack->prev;
	_stack->next = tmp->next;
	tmp->next->prev = _stack;

	tmp->next = NULL;
	tmp->prev = NULL;
	free(tmp);

	return _stack;
}








