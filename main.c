#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "LCD/LCD.h"
#include "TouchPanel/TouchPanel.h"
#include "semihosting/semihosting.h"

#include "interface_calculator.h"
#include "grammar.h"

//Otzberg

/*IBM Raphsody*/

//static char array[][2000];

int main(void) {
	TP_Init();
	LCD_Initializtion();
	LCD_Clear(Black);
	TouchPanel_Calibrate();
	IRF_DrawInterface(Blue, Red, Black);

	char *expression = calloc(sizeof(char), 256);

	/*
	expression = "-(5+3)+(5-4*(3-2))*(((3+2*(+5-5*3*4+9)*4)))+4*(5-(-(-(-5))))";
	expression = "+60-65+(-(-2+54))+()*()";
	expression = "5^(-3)";
	expression = "-(-1)+1+5*10^(-10+12*(-1)*(-1)+(+1)-(-1))";
	expression = "2^2^2";
	expression = ComputeExpression(expression);

	printf("%s\n", expression);
	*/

	while(1) {
		getDisplayPoint(&display, Read_Ads7846(), &matrix);
		IRF_Work(expression);
		//printf("!\n");
	}

	return 0;
}

/*
 * Баги:
 * 0. ...
 */
