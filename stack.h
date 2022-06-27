#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

/*Structures*/
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
struct Stack *CreateStack(char *);
/*Adds a new element to stack*/
struct Stack *Push(struct Stack *, char *, int );
/*Gets last input element from Stack without its removing*/
struct Stack *Peek(struct Stack *);
/*Gets last input element and removes this one*/
struct PopResult *Pop(struct Stack *);
/*Prints elements of stack(Order: from top to low item)*/
void PrintStack(struct Stack *);
/*Gets a count of stack's items*/
int GetItemsCount(struct Stack *);
/*Gets the last element of regarded stack*/
struct Stack *Bottom(struct Stack *);
/*Produces movements among stack's items*/
struct Stack *MoveTo(struct Stack *, unsigned int );
/*Pushes a new element on concrete position*/
struct Stack* PushInside(struct Stack *, unsigned int , char *, int );
/*Gets the first position of required data*/
int FindValue(struct Stack *, char *);
/*Removes a concrete item via its position number*/
struct Stack* RemoveAt(struct Stack *, int );
/*Deletes all items and frees memory*/
struct Stack* Clear(struct Stack *);


#endif // STACK_H_INCLUDED
