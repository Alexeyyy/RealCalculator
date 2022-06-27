#include<stdlib.h>
#include<stdio.h>
#include<string.h>

//Checks the brackets of input expression
int CheckBrackets(char *expression) {
	int count_left_brackets = 0, count_right_brackets = 0, i;

	//let's count a number of each type of brackets
	for(i = 0; i < strlen(expression); i++) {
		if(expression[i] == '(')
			count_left_brackets++;
		if(expression[i] == ')')
			count_right_brackets++;
	}

	if(count_right_brackets == count_left_brackets) {
		int total_count_brackets = count_left_brackets + count_right_brackets;

		//exclude occasions for ")("
		if(total_count_brackets != 0) {
			int  check_open_close = 0;

			for(i = 0; i < strlen(expression); i++) {
				if(check_open_close < 0)
					return -1;
				else {
					if(expression[i] == '(')
						check_open_close++;
					if(expression[i] == ')')
						check_open_close--;
				}
			}
		}

		return 1;
	}
	else
		return -1;

	//will never reach this "return"
	return 1;
}

//The function checks comma's right position in the expression
int CheckComma(char *expression) {
	int i;

	if(expression[0] == '.' || expression[strlen(expression) - 1] == '.')
		return -1;

	for(i = 1; i < strlen(expression) - 1; i++) {
		if(expression[i] == '.') {
			if((expression[i - 1] >= '0' && expression[i - 1] <= '9') && (expression[i + 1] >= '0' && expression[i + 1] <= '9'))
				continue;
			else
				return -1;
		}
	}

	return 1;
}

//Checks the correct order of '*' and '/' signs
int CheckFirstActionSigns(char *expression, char sign) {
	int i;

	if(expression[0] == sign || expression[strlen(expression) - 1] == sign)
		return -1;

	for(i = 1; i < strlen(expression) - 1; i++) {
		if(expression[i] == sign) {
			if(((expression[i - 1] >= '0' && expression[i - 1] <= '9') && (expression[i + 1] >= '0' && expression[i + 1] <= '9')) ||
				((expression[i - 1] >= '0' && expression[i - 1] <= '9') && (expression[i + 1] == '(')) ||
				((expression[i + 1] >= '0' && expression[i + 1] <= '9') && (expression[i - 1] == ')')) ||
				((expression[i - 1] == ')') && ((expression[i + 1] == '(')))	)
				continue;
			else
				return -1;
		}
	}
	return 1;
}

//Checks the correct order of '+' and '-'
int CheckSecondActionSigns(char *expression, char sign) {
	int i;

	//if we meet this sign at the end of expression - then stupidly return -1
	if(expression[strlen(expression) - 1] == sign)
		return -1;

	for(i = 1; i < strlen(expression) - 1; i++) {
		if(expression[i] == sign) {
			if((((expression[i - 1] >= '0' && expression[i - 1] <= '9') && (expression[i + 1] >= '0' && expression[i + 1] <= '9'))) ||
				(strchr("+-/*.",expression[i - 1]) == NULL && expression[i + 1] >= '0' && expression[i + 1] <= '9') ||
				(strchr("+-/*.",expression[i + 1]) == NULL && expression[i - 1] >= '0' && expression[i - 1] <= '9') ||
				(expression[i - 1] == ')' || (expression[i + 1] == '('))) {
				continue;
			}
			else
				return -1;
		}
	}
	return 1;
}

//Checks the content of operands in expression(excludes occasions for "3.2.4" numbers)
int CheckOperandsOnCommas(char *expression) {
	int i; //runner through expression
	int dot_counter; //counter for commas

	for(i = 0; i < strlen(expression); i++) {
		if(strchr("1234567890", expression[i]) != NULL) {
			dot_counter = 0;
			while(strchr("1234567890", expression[i]) != NULL || expression[i] == '.') {
				if(expression[i] == '.') {
					dot_counter++;
					if(dot_counter > 1)
						return -1; //in this case operand contains more than 1 comma
				}
				i++;
			}
		}
	}

	return 1;
}

//Probably, some cases will be added with time
int CheckOtherStrangeCases(char *expression) {
	if(strstr(expression, "()"))
		return  -1;
	return 1;
}
