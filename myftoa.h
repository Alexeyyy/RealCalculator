#ifndef MYFTOA_H_INCLUDED
#define MYFTOA_H_INCLUDED

/*Get number of digits from which number consists of*/
int GetNumberDigits(unsigned long );
/*Get multiplier for further extraction of digits*/
unsigned long GetMultiplier(int );
/*Casts number to char* */
char* BuildString(unsigned long long );
/*Converts double number to char* */
char* MyFtoa(double , int );

#endif // MYFTOA_H_INCLUDED
