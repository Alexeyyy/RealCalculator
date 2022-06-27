#ifndef POLISH_NOTATION_H_INCLUDED
#define POLISH_NOTATION_H_INCLUDED

//Presents a match between sign and its position in polish notation
struct Sign {
	char *sign;
	int pos;
};

//Records a data to stack
char *RecordData(char *, int  , int );
//Gets a number's length (it's needed in case of numbers like these: 345435, 312)
int GetNumberLength(char *, int );
//Gets sign's priority
int GetPriority(char );
//Find a minimum in the array
int GetArraysMinimum(struct Sign *, int );
//Gets a polish notation
struct Stack* GetPolishNotation(char *);
//Reduces a polish notation and gives a result
char* GetResult(struct Stack *);
//Checks the brackets of input expression
/*int CheckBrackets(char *_expression);
//Checks the correct order of '*' and '/' signs
int CheckFirstActionSigns(char *_expression, char _sign);
//Checks the correct order of '+' and '-'
int CheckSecondActionSigns(char *_expression, char _sign);*/

#endif // POLISH_NOTATION_H_INCLUDED
