#ifndef GRAMMAR_H_INCLUDED
#define GRAMMAR_H_INCLUDED

struct Action {
	char *sign;
};

struct Actions {
	struct Action actions[2];
	int length;
};

//-1 --- no brackets, 1 - some brackets exist
int FindBrackets(char *);
//Gets brackets positions
int *GetBracketsMap(char* );
//-1, if no brackets
//number, differed from -1 - a position
int GetFirstBracketsActionPos(int *, int );
//Substrings the part of expression
char *GetPartOfExpression(char *, int *, int );
//Gets the left and right remain parts of expression (a calculated part is between them)
char** DistributeExpression(char *, int );
//Looks for the first action
int GetPositionOfFirstAction(char *);
//Get operands
char** GetOperands(char *, int );
//Change the sign '-' on '$' and '+' on '@' before all negative numbers and positive participants
char *DefineNegativePositiveParticipants(char *);
//Represent "nnpn" into one letter meaning
char* ReduceOperand(char *);
//if our operand is negative then change '$' to '-' and for positive occasion too
char** CheckEditOperands(char **);
//If we got negative result then change the first member from '-' to 'n'
char* EditResult(char *);
//Compounds the expression from three parts(left, result and right)
char* CompoundExpression(char **, char *, char *);
//Computes the result of action
char* ComputePart(char** , char *);
//Checks the next statement: "Have we got a result in part or expression?"
int IsComputed(char *);
//Computes offered expression
char* ComputeExpression(char *);


#endif // GRAMMAR_H_INCLUDED
