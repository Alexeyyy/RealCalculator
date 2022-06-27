#define PRECISION 7

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "myftoa.h"
#include "check_expression.h"
#include "freeMemory.h"

struct Action {
	char *sign;
};

struct Actions {
	struct Action actions[4];
	int length;
};

struct Actions first_priority_actions = {
	{
		{"^"},
		{"/"},
		{"*"}
	}, 3
};

struct Actions second_priority_actions = {
	{
		{"-"},
		{"+"},
	}, 2
};


//-1 --- no brackets, 1 - some brackets exist
int FindBrackets(char *expression) {
	int i;

	for(i = 0; i < strlen(expression); i++) {
		if(expression[i] == '(')
			return 1;
	}

	return -1;
}

//Gets brackets positions
int *GetBracketsMap(char* expression) {
	int *brackets = calloc(sizeof(int), strlen(expression));
	int i, order_opened = 1;

	for(i = 0; i < strlen(expression); i++) {
		if(expression[i] == '(') {
			brackets[i] = order_opened;
			order_opened++;
			continue;
		}
		if(expression[i] == ')') {
			brackets[i] = 0;
			continue;
		}
		brackets[i] = -1;
	}

	return brackets;
}

//-1, if no brackets
//number, differed from -1 - a position
int GetFirstBracketsActionPos(int *brackets_map, int length) {
	int pos = -1, index = -1, i;

	//look for the first bracket
	for(i = 0; i < length; i++) {
		if(brackets_map[i] > 0) {
			pos = i;
			index = brackets_map[i];
			break;
		}
	}

	//look up the maximum
	for(i = pos + 1; i < length; i++) {
		if(brackets_map[i] > 0) {
			if(brackets_map[i] > index) {
				index = brackets_map[i];
				pos = i;
			}
		}
	}

	return pos;
}

//Substrings the part of expression
char *GetPartOfExpression(char *expression, int *brackets_map, int pos_start) {
	//look up for the pos_finished
	int i, pos_finished;
	for(i = pos_start; i < strlen(expression); i++) {
		if(brackets_map[i] == 0) {
			pos_finished = i;
			break;
		}
	}

	int length = pos_finished - pos_start - 1;//-1 - consider the bracket
	char *part = calloc(sizeof(char), length);

	strncpy(part, expression + pos_start + 1, length);
	////printf("To calculate %s\n", part);
	return part;
}

//Gets the left and right remain parts of expression (a calculated part is between them)
char** DistributeExpression(char *expression, int pos_start) {
	char **expression_parts = calloc(sizeof(char*), 2); //reserve only for 2 because we have left, central(part which is calculated at the moment) and right
	int i, pos_closed;

	for(i = 0; i < 2; i++) {
		expression_parts[i] = calloc(sizeof(char), 256);
	}

	//looks for the first sign of ')'
	for(i = pos_start; i < strlen(expression); i++) {
		if(expression[i] == ')') {
			pos_closed = i;
			break;
		}
	}

	//let's copy a parts of expression (of course we don't copy the closed brackets because on that place we'll insert the local result)
	strncpy(expression_parts[0], expression, pos_start);
	strncpy(expression_parts[1], expression + pos_closed + 1, strlen(expression));

	return expression_parts;
}

//Looks for the first action
int GetPositionOfFirstAction(char *part) {
	int i;

	//checks first priority actions (^,*,/)
	for(i = 0; i < first_priority_actions.length; i++) {
		if(strstr(part, first_priority_actions.actions[i].sign) != NULL) {
			return strlen(part) - strlen(strstr(part, first_priority_actions.actions[i].sign));
		}
	}

	//checks second priority actions (+,-)
	for(i = 0; i < second_priority_actions.length; i++) {
		if(strstr(part, second_priority_actions.actions[i].sign) != NULL) {
			return strlen(part) - strlen(strstr(part, second_priority_actions.actions[i].sign));
		}
	}

	//there is no any actions
	return -1;
}

//Get operands
char** GetOperands(char *part, int st_pos) {
	char **operands = calloc(sizeof(char*), 5); //1 - first operand, 2 - second operand, 3 - sign, 4 - remaining left part, 5 - remaining right part
	int i, left, right;
	//reserve memory
	for(i = 0; i < 5; i++)
		operands[i] = calloc(sizeof(char), 256);

	//in left side
	i = st_pos - 1;
	left = st_pos - 1;
	while(i > 0 && strchr("+-*/^", part[i]) == NULL) {
		left--;
		i--;
	}

	if(left != 0)
		left++;

	//in right side
	i = st_pos + 1;
	right = st_pos + 1;
	while(i < strlen(part) && strchr("+-*/^", part[i]) == NULL) {
		right++;
		i++;
	}

	if(right != strlen(part))
		right--;

	strncpy(operands[0], part + left, st_pos - left);
	strncpy(operands[1], part + st_pos + 1, right - st_pos);
	strncpy(operands[2], part + st_pos, 1); //save sign
	strncpy(operands[3], part, left); //save remaining left part
	strncpy(operands[4], part + right + 1, strlen(part)); //save remaining right part

	return operands;
}

//Change the sign '-' on '$' and '+' on '@' before all negative numbers and positive participants
char *DefineNegativePositiveParticipants(char *part) {
	int i;

	for(i = 0; i < strlen(part); i++) {
		if(part[i] == '-' || part[i] == '+') {
			if(strchr("1234567890", part[i - 1]) == NULL || i == 0) {
				if(part[i] == '-')
					part[i] = 'n';
				if(part[i] == '+')
					part[i] = 'p';
			}
		}
	}

	return part;
}

//Represent "nnpn" into one letter meaning
char* ReduceOperand(char *operand) {
	int i, n_count = 0, p_count = 0;

	for(i = 0; i < strlen(operand); i++) {
		if(operand[i] == 'n')
			n_count++;
		if(operand[i] == 'p')
			p_count++;
	}

	if(n_count + p_count > 0) {
		//if number of minuses before the operand is even
		if(n_count % 2 == 0) {
			char *tmp = calloc(sizeof(char), 256);
			strncpy(tmp, operand + n_count + p_count, strlen(operand));
			operand = tmp;
		}
		else {
			char *tmp1 = calloc(sizeof(char), 256);
			char *tmp2 = calloc(sizeof(char), 256);
			strcat(tmp1, "n");
			strncpy(tmp2, operand + n_count + p_count, strlen(operand));
			strcat(tmp1, tmp2);
			operand = tmp1;
		}
	}

	return operand;
}

//if our operand is negative then change '$' to '-' and for positive occasion too
char** CheckEditOperands(char **operands) {
	int i;

	operands[0] = ReduceOperand(operands[0]);
	operands[1] = ReduceOperand(operands[1]);


	//restriction for 2 because we're checking only operands
	for(i = 0; i < 2; i++) {
		if(operands[i][0] == 'n') //for negative occasion
			operands[i][0] = '-';
		if(operands[i][0] == 'p') //for positive one
			strncpy(operands[i], operands[i] + 1, strlen(operands[i]));
	}


	return operands;
}

//If we got negative result then change the first member from '-' to 'n'
char* EditResult(char *result) {
	int i;

	for(i = 1; i < strlen(result); i++) {
		if(result[i] == '-')
			return result;
	}

	if(result[0] == '-')
		result[0] = 'n';

	return result;
}

//Compounds the expression from three parts(left, result and right)
char* CompoundExpression(char **expression_parts, char *result, char *expression) {
	expression = expression_parts[0];
	strcat(expression, result);
	strcat(expression, expression_parts[1]);

	return expression;
}


//Computes the result of action
char* ComputePart(char** operands, char *part) {
	double operand_1 = atof(operands[0]);
	double operand_2 = atof(operands[1]);
	char operation_sign = operands[2][0];

	//////printf("%f ---- %f\n", operand_1, operand_2);

	switch(operation_sign) {
		case '+': operand_1 += operand_2; break;
		case '-': operand_1 -= operand_2; break;
		case '*': operand_1 *= operand_2; break;
		case '/': {
			if (operand_2 == 0)
				return "Division by zero!";
			else {
				operand_1 /= operand_2;
				break;
			}
		}
		case '^': {
			operand_1 = pow(operand_1, operand_2);
			if(isnan(operand_1) > 0) {
				return "Degree is incorrect!";
			}
			else
				break;
		}
		default: /*printf("DEF\n");*/ break;
	}

	char *insert = MyFtoa(operand_1, PRECISION);

	//gather remain expression
	part = operands[3];
	strcat(part, insert);
	strcat(part, operands[4]);

	return part;
}

//Checks the next statement: "Have we got a result in part or expression?"
int IsComputed(char *expression) {
	int i;

	for(i = 0; i < strlen(expression); i++) {
		if(strchr("+-*/()^", expression[i]) != NULL) //we didn't consider '-',  because it means negative number
			return -1;
	}

	return 1;
}

//Computes offered expression
char* ComputeExpression(char *expression) {
	//expression = DefineNegativePositiveParticipants(expression);
	//printf("Expression == %s\n", expression);
	if(CheckOperandsOnCommas(expression) == 1 && CheckBrackets(expression) == 1 && CheckFirstActionSigns(expression, '/') == 1 && CheckFirstActionSigns(expression, '^') == 1 && CheckFirstActionSigns(expression, '*') == 1 && CheckSecondActionSigns(expression, '+') == 1 && CheckSecondActionSigns(expression, '-') == 1 && CheckOtherStrangeCases(expression) == 1) {
		while(IsComputed(expression) == -1) {
			if(FindBrackets(expression) == 1) {
				/////
				int *brackets_map = GetBracketsMap(expression);
				int pos = GetFirstBracketsActionPos(brackets_map, strlen(expression));

				char *part = GetPartOfExpression(expression, brackets_map, pos);
				char **parts_expression = DistributeExpression(expression, pos);

				////printf("LEFT 1 ----- %s\nRIGHT 2 ----- %s\n", parts_expression[0], parts_expression[1]);

				int i = 0;

				while(IsComputed(part) == -1) {
					part = DefineNegativePositiveParticipants(part);
					pos = GetPositionOfFirstAction(part);

					if(pos == -1)
						break;

					char **operands = GetOperands(part, pos);
					//////printf("OP 1 ==== %s\nOP 2 === %s\nSIGN === %s\n", operands[0], operands[1], operands[2]);
					operands = CheckEditOperands(operands);
					//////printf("AFTER!!!! ===== OP 1 ==== %s\nOP 2 === %s\nSIGN === %s\n", operands[0], operands[1], operands[2]);
					part = ComputePart(operands, part);

					if(strcmp(part, "Division by zero!") == 0)
						return "Division by zero!";
					if(strcmp(part, "Degree is incorrect!") == 0)
						return "Degree is incorrect!";

					//////printf("Step is %d ==== %s\n", i, part);
					i++; //tmp
					part = EditResult(part);

					//FreeDDCArray(operands, 5);
				}

				expression = CompoundExpression(parts_expression, part, expression);
				//////////
				//Free-memory block
				/*FreeODIArray(brackets_map);
				FreeODCArray(part);
				FreeDDCArray(parts_expression, 2);*/
				//Free-memory block
			}
			else {
				////printf("LAST JERK === %s\n", expression);

				while(IsComputed(expression) == -1) {
					expression = DefineNegativePositiveParticipants(expression);
					int pos = GetPositionOfFirstAction(expression);

					if(pos == -1)
						break;

					char **operands = GetOperands(expression, pos);
					////printf("OP 1 ==== %s\nOP 2 === %s\nSIGN === %s\n", operands[0], operands[1], operands[2]);
					operands = CheckEditOperands(operands);
					expression = ComputePart(operands, expression);

					if(strcmp(expression, "Division by zero!") == 0)
						return "Division by zero!";
					if(strcmp(expression, "Degree is incorrect!") == 0)
						return "Degree is incorrect!";

					////printf("Current Condition ==== %s\n", expression);
					expression = EditResult(expression);

					//FreeDDCArray(operands, 5);
				}

				//in case when we received a negative result
				expression = ReduceOperand(expression);

				if(expression[0] == 'n') {
					expression[0] = '-';
					break; //because it won't satisfy loop's condition (we have a '-', but it's a sign)
				}


			}
		}

		////printf("RESULT === %s\n", expression);
	}
	else {
		expression = "Expression is wrong!";
	}

	return expression;
}






