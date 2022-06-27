#define MAX_SIZE 256
#define PRECISION 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "myftoa.h"

//Presents a match between sign and its position in polish notation
struct Sign {
	char *sign;
	int pos;
};

//Records a data to stack
char *RecordData(char *_expression, int _size ,int _i) {
	char *tmp = calloc(sizeof(char),_size);
	strncpy(tmp, _expression + _i, _size - 1);
	return tmp;
}

//Gets a number's length (it's needed in case of numbers like these: 345435, 312)
int GetNumberLength(char *_expression, int _i) {
	int i = _i;
	while((_expression[i] >= '0' && _expression[i] <= '9') || _expression[i] == '.')
		i++;
	return i - _i + 1;
}

//Gets sign's priority
int GetPriority(char _sign) {
	if(_sign == '*' || _sign == '/')
		return 2;
	if(_sign == '+' || _sign == '-')
		return 1;
	return -1;
}

//"56.5+(43.43-23.5)*43.8"
//Gets a polish notation
struct Stack* GetPolishNotation(char *_expression) {
    struct Stack *st_signs = NULL;
    struct Stack *st_out = NULL;
    char* tmp = calloc(sizeof(char), 1);
    struct PopResult *res;

    int i = 0; //counter
    while(i < strlen(_expression)) {
    	//if the symbol of left bracket was met
    	if(_expression[i] == '(') {
        	st_signs = Push(st_signs, RecordData(_expression, 2, i), 0);
            i++;
            continue;
        }

    	//if the current symbol is a number/
    	if(_expression[i] >= '0' && _expression[i] <= '9') {
    		int numberLength = GetNumberLength(_expression, i);
    		st_out = Push(st_out, RecordData(_expression, numberLength, i), 0);
    		i += (numberLength - 1);
    		continue;
    	}

    	//If we've met a sign '*' or '/'
    	if(strchr("+-*/", _expression[i]) != NULL){
    		if(st_signs == NULL) {
    			st_signs = Push(st_signs, RecordData(_expression, 2, i), 0);
    	  		i++;
    	   		continue;
    	   	}
    	    st_signs = Peek(st_signs);
   	    	char topSign = st_signs->data->record[0];
   	    	char currentSign = (RecordData(_expression, 2, i))[0];
   	    	int priorityStackSign = GetPriority(topSign);
   	    	int priorityCurrentSign = GetPriority(currentSign);
    	    if(priorityCurrentSign > priorityStackSign) {
   	    		st_signs = Push(st_signs, RecordData(_expression, 2, i), 0);
   	    	}
   	    	else {
   	    		//printf("ST_SIGNS Data --- %s\n", st_signs->data);
   	    		while((st_signs->data->record)[0] != '(' && st_signs != NULL) {
   	    			res = Pop(st_signs);
   	    			st_signs = res->pointer;
   	    			tmp = res->data;
   	    			st_out = Push(st_out, tmp, 0);
   	    		}
   	    		st_signs = Push(st_signs, RecordData(_expression, 2, i), 0);
   	    	}
   	    	i++;
   	    	continue;
    	}

    	//in occasion of the met symbol is closed bracket
    	if(_expression[i] == ')') {
    		while((st_signs->data->record)[0] != '(') {
    			res = Pop(st_signs);
    			st_signs = res->pointer;
    			tmp = res->data;
    			st_out = Push(st_out, tmp, 0);
    		}

    		res = Pop(st_signs);
    		st_signs = res->pointer;
    		//_expression[i] = *res->data;
    		_expression[i] = ((struct Data*)res->data)->record;

    		i++;
    		continue;
    	}
    }

    //out remain elements from the stack st_signs to the stack "st_out"
    while(st_signs != NULL) {
    	res = Pop(st_signs);
    	st_signs = res->pointer;
    	tmp = res->data;
      	st_out = Push(st_out, tmp, 0);
    	tmp = NULL;
    }

    //free a memory from unused variables
    //free(tmp);
    //if(res != NULL)
    //	free(res);
    st_signs = Clear(st_signs);

    PrintStack(st_out);

    return st_out;
}

//Find a minimum in the array
int GetArraysMinimum(struct Sign *_signs, int _length) {
	int i, min = _signs[0].pos;

	for(i = 0; i < _length; i++) {
		if(min < 0 && _signs[i].pos > 0)
			min = _signs[i].pos;
		if(min > _signs[i].pos && _signs[i].pos != -2)
			min = _signs[i].pos;
	}

	return min;
}

//Defines a sign and returns conformed number
int GetSignsCode(char *_sign) {
	if(strcmp(_sign, "+") == 0)
		return 0;
	if(strcmp(_sign, "-") == 0)
		return 1;
	if(strcmp(_sign, "*") == 0)
		return 2;
	if(strcmp(_sign, "/") == 0)
		return 3;

	return -1;
}

//Reduces a polish notation and gives a result
char* GetResult(struct Stack *_polishNotation) {
	struct Sign *signs = (struct Sign*)malloc(sizeof(struct Sign*) * 4); //because there are only four signs for arithmetical operations
	char *tmp = calloc(sizeof(char), MAX_SIZE);
	int nearest_sign_pos;
	struct Stack *first_operand;
	struct Stack *second_operand;
	double operand_1, operand_2;
	int operation_sign_code;

	if(GetItemsCount(_polishNotation) == 0)
		return "You didn't type an expression!";

	signs[0].sign = "+";
	signs[1].sign = "-";
	signs[2].sign = "*";
	signs[3].sign = "/";



	while(GetItemsCount(_polishNotation) != 1) {
		signs[0].pos = FindValue(_polishNotation, "+");
		signs[1].pos = FindValue(_polishNotation, "-");
		signs[2].pos = FindValue(_polishNotation, "*");
		signs[3].pos = FindValue(_polishNotation, "/");

		nearest_sign_pos = GetArraysMinimum(signs, 4);

		//Checks the occasion when polish notation is non-computing. E.g. 5+3+(). Polish notation is 5 3 + + - that is wrong!
		if( nearest_sign_pos <= 1) {
			return "Your input was wrong!(1)";
		}
		_polishNotation = MoveTo(_polishNotation, nearest_sign_pos);

		//The two operands were received and they must be a numbers
		first_operand = _polishNotation->prev;
		second_operand = _polishNotation->prev->prev;
		operand_1 = atof(first_operand->data);
		operand_2 = atof(second_operand->data);
		//Get an operation's sign
		operation_sign_code = GetSignsCode(_polishNotation->data);

		switch(operation_sign_code) {
			case 0: {
				operand_2 += operand_1;
				break;
			}
			case 1: {
				operand_2 -= operand_1;
				break;
			}
			case 2: {
				operand_2 *= operand_1;
				break;
			}
			case 3: {
				if(operand_1 == 0)
					return "Division by zero!";
				operand_2 /= operand_1;
				break;
			}
			default:
				return "Your input was wrong!(2)";
		}

		tmp = MyFtoa(operand_2, PRECISION);
		//Set a received value and delete two items from stack(an item with sign and previous ones to it)
		_polishNotation->prev->prev->data = tmp;
		_polishNotation = RemoveAt(_polishNotation, nearest_sign_pos);
		_polishNotation = RemoveAt(_polishNotation, nearest_sign_pos - 1);
	}

	tmp = _polishNotation->data;
	Clear(_polishNotation);

	return tmp;
}

struct Stack* Merging(struct Stack*_polish_notation) {

}

//Reduces a polish notation and gives a result
char* GetResult2(struct Stack *_polishNotation) {
	struct Sign *signs = (struct Sign*)malloc(sizeof(struct Sign*) * 4); //because there are only four signs for arithmetical operations
	char *tmp = calloc(sizeof(char), MAX_SIZE);
	int nearest_sign_pos;
	struct Stack *first_operand;
	struct Stack *second_operand;
	double operand_1, operand_2;
	int operation_sign_code;

	if(GetItemsCount(_polishNotation) == 0)
		return "You didn't type an expression!";

	signs[0].sign = "+";
	signs[1].sign = "-";
	signs[2].sign = "*";
	signs[3].sign = "/";

	while(GetItemsCount(_polishNotation) != 1) {
		//Merge signs and numbers

		signs[0].pos = FindValue(_polishNotation, "+");
		signs[1].pos = FindValue(_polishNotation, "-");
		signs[2].pos = FindValue(_polishNotation, "*");
		signs[3].pos = FindValue(_polishNotation, "/");

		nearest_sign_pos = GetArraysMinimum(signs, 4);

		//Checks the occasion when polish notation is non-computing. E.g. 5+3+(). Polish notation is 5 3 + + - that is wrong!
		_polishNotation = MoveTo(_polishNotation, nearest_sign_pos);

		//The two operands were received and they must be a numbers
		first_operand = _polishNotation->prev;
		second_operand = _polishNotation->prev->prev;
		operation_sign_code = GetSignsCode(_polishNotation->data);

		operand_1 = atof(first_operand->data);
		operand_2 = atof(second_operand->data);
		//Get an operation's sign


		switch(operation_sign_code) {
			case 0: {
				operand_2 += operand_1;
				break;
			}
			case 1: {
				operand_2 -= operand_1;
				break;
			}
			case 2: {
				operand_2 *= operand_1;
				break;
			}
			case 3: {
				if(operand_1 == 0)
					return "Division by zero!";
				operand_2 /= operand_1;
				break;
			}
			default:
				return "Your input was wrong!(2)";
		}

		tmp = MyFtoa(operand_2, PRECISION);
		//Set a received value and delete two items from stack(an item with sign and previous ones to it)
		_polishNotation->prev->prev->data = tmp;
		_polishNotation = RemoveAt(_polishNotation, nearest_sign_pos);
		_polishNotation = RemoveAt(_polishNotation, nearest_sign_pos - 1);
	}

	tmp = _polishNotation->data;
	Clear(_polishNotation);

	return tmp;
}


















