#ifndef CHECK_EXPRESSION_H_INCLUDED
#define CHECK_EXPRESSION_H_INCLUDED

//Checks the brackets of input expression
int CheckBrackets(char *);
//The function checks comma's right position in the expression
int CheckComma(char *);
//Checks the correct order of '*' and '/' signs
int CheckFirstActionSigns(char *, char );
//Checks the correct order of '+' and '-'
int CheckSecondActionSigns(char *, char );
//Probably, some cases will be added with time
int CheckOtherStrangeCases(char *);

#endif // CHECK_EXPRESSION_H_INCLUDED
