#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*Get number of digits from which number consists of*/
int GetNumberDigits(unsigned long number) {
	int counter = 0;
	double tmp = number;

	while(tmp >= 1) {
		tmp /= 10;
		counter++;
	}

	return counter;
}

/*Get multiplier for further extraction of digits*/
unsigned long GetMultiplier(int digits_count) {
	unsigned long long multiplier = 1;
	int counter = 1;

	while(counter < digits_count) {
		multiplier *= 10;
		counter++;
	}

	return multiplier;
}

/*Casts number to char* */
char* BuildString(unsigned long long number, int part_code, int nulls_after_comma) {
	char *string = calloc(sizeof(char), 256);

	if(number == 0) {
		string = strcat(string, "0");
		return string;
	}

	char *digit = calloc(sizeof(char), 2);
	unsigned long multiplier;
	double dividend;
	int casted_divident;
	int digits_count = GetNumberDigits(number);

	//if we've a deal with a fraction part do it
	if(part_code == -1) {
		while(nulls_after_comma > 0) {
			string = strcat(string, "0");
			nulls_after_comma --;
		}
	}

	while(digits_count > 0) {
		multiplier = GetMultiplier(digits_count);
		dividend = number/multiplier;
		casted_divident = (int)dividend;
		sprintf(digit, "%d", casted_divident);
		string = strcat(string, digit);
		number = number - multiplier * casted_divident;

		digits_count --;
	}

	return string;
}

/*Converts double number to char* */
char* MyFtoa(double value, int precision) {
	int f_negative = 0; //0 --- positive, -1 --- negative
	char* number = calloc(sizeof(char), 256);

	if(value < 0) {
		f_negative = -1;
		value *= (-1);
	}

	char *whole_part, *fraction_part;
	unsigned long long wp = (unsigned long long)value;
	unsigned long long fp;
	unsigned long long precision_summand = 1;
	unsigned int nulls_after_comma = 0;
	int counter = 0;

	while(counter < precision) {
		precision_summand *= 10;
		counter++;
	}

	fp = (unsigned long long)((value - wp) * precision_summand);
	nulls_after_comma = precision - GetNumberDigits(fp);

	whole_part = BuildString(wp, 1, nulls_after_comma);
	fraction_part = BuildString(fp, -1, nulls_after_comma);

	if(f_negative == -1)
		sprintf(number, "-%s.%s", whole_part, fraction_part);
	else
		sprintf(number, "%s.%s", whole_part, fraction_part);

	return number;
}
