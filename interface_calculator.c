#define KEYBOARD_WIDTH 6
#define KEYBOARD_HEIGHT 4
#define EXPRESSION_LENGTH 35

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "LCD/LCD.h"
#include "TouchPanel/TouchPanel.h"

#include "grammar.h"


struct Key {
	uint16_t x_pos;
	uint16_t y_pos;
	uint16_t width;
	uint16_t height;
	uint16_t gap_left;
	uint16_t gap_top;
	const char *text;
	const char *destination;
};

//===========Global variables=============
struct Key keys[4][6] = {
		{
			{
				6, 20, 47, 50, 4, 8, "1", "input"
			},
			{
				58, 20, 47, 50, 4, 8, "2", "input"
			},
			{
				110, 20, 47, 50, 4, 8, "3", "input"
			},
			{
				162, 20, 47, 50, 4, 8, "+", "input"
			},
			{
				214, 20, 47, 50, 4, 8, "<-", "backspace"
			},
			{
				266, 20, 47, 50, 4, 8, "C", "erase"
			},
		},
		{
			{
				6, 75, 47, 50, 4, 8, "4", "input"
			},
			{
				58, 75, 47, 50, 4, 8, "5", "input"
			},
			{
				110, 75, 47, 50, 4, 8, "6", "input"
			},
			{
				162, 75, 47, 50, 4, 8, "-", "input"
			},
			{
				214, 75, 47, 50, 4, 8, "(", "input"
			},
			{
				266, 75, 47, 50, 4, 8, ")", "input"
			}
		},
		{
			{
				6, 130, 47, 50, 4, 8, "7", "input"
			},
			{
				58, 130, 47, 50, 4, 8, "8", "input"
			},
			{
				110, 130, 47, 50, 4, 8, "9", "input"
			},
			{
				162, 130, 47, 50, 4, 8, "*", "input"
			},
			{
				214, 130, 99, 105, 4, 8, "=", "compute"
			},
			{
				214, 130, 99, 105, 6, 8, "=", "empty"
			},
		},
		{
			{
				6, 185, 47, 50, 4, 8, "0", "input"
			},
			{
				58, 185, 47, 50, 4, 8, ".", "input"
			},
			{
				110, 185, 47, 50, 4, 8, "^", "input"
			},
			{
				162, 185, 47, 50, 4, 8, "/", "input"
			},
			{
				214, 130, 99, 105, 6, 8, "=", "empty"
			},
			{
				214, 130, 99, 105, 6, 8, "=", "empty"
			},
		},
};

int f_finished = -1; //excludes the input on the place of an answer
//========================================

//Set a delay for pressing buttons
void IRF_Delay_BTN(uint16_t _ms)
{
	uint16_t i,j;
	for( i = 0; i < _ms; i++ )
	{
		for( j = 0; j < 1141; j++);
	}
}

//=========================This part of code in charge of drawing elements=========================
void IRF_DrawButton(struct Key key, uint16_t colorButton, uint16_t colorText, uint16_t colorBG) {
	LCD_DrawRectangle(key.x_pos, key.y_pos, key.width, key.height, colorButton);
	GUI_Text(key.x_pos + (key.width)/2 - key.gap_left, key.y_pos + (key.height)/2 - key.gap_top, key.text, colorText, colorBG);
}

void IRF_DrawInterface(uint16_t colorButton, uint16_t colorText, uint16_t colorBG) {
	int i,j;

	for(i = 0; i < KEYBOARD_HEIGHT; i++) {
		for(j = 0; j < KEYBOARD_WIDTH; j++) {
			if(strcmp(keys[i][j].destination, "empty") != 0)
				IRF_DrawButton(keys[i][j], colorButton, colorText, colorBG);
		}
	}
}
//==================================================================================================

//================================This one is for event handlers====================================
void IRF_InfButtonPressed(struct Key key, char *expression) {
	TP_GetAdXY(display.x, display.y);
	//Look up our button at the screen
	if(display.x >= key.x_pos && display.y >= key.y_pos && display.y <= key.y_pos + key.height && display.x <= key.width + key.x_pos) {
		//Redraw touched button
		IRF_DrawButton(key, Yellow, Yellow, Black);

		if(strcmp(key.destination, "input") == 0 && strlen(expression) <= 39) {
			if(f_finished == 1) {
				LCD_FillRectangle2(0,0,320,20, Black);
				f_finished = -1;
			}
			//automatically add '*' after ')'
			if(expression[strlen(expression) - 1] == ')' && strstr(")+-/*^", key.text) == NULL)
				strcat(expression, "*");

			if(strcmp(key.text, ".") == 0) {
				if(!(expression[strlen(expression) - 1] >= '0' && expression[strlen(expression) - 1] <= '9') || strlen(expression) == 0) {
					strcat(expression, "0.");
				}
				else {
					expression = strcat(expression, key.text);
				}
			}
			else if(strcmp(key.text, "(") == 0) {
				if((strchr("*/-+^(", expression[strlen(expression) - 1]) == NULL) && strlen(expression) != 0) {
					strcat(expression, "*(");
				}
				else
					strcat(expression, key.text);
			}
			else
				strcat(expression, key.text);
			GUI_Text(5,0, expression, Red, Black);
			//printf("Input == %s\n", expression);
		}

		if(f_finished == 1) {
			LCD_FillRectangle(0,0,320,20, Black);
			f_finished = -1;
		}
		if(strcmp(key.destination, "backspace") == 0) {
			expression[strlen(expression) - 1] = 0;
			LCD_FillRectangle(0,0,320,20, Black);
			//printf("Backspace == %s\n", expression);

			//otherwise we'll get smth strange in the output string
			if(strlen(expression) != 0)
				GUI_Text(5,0, expression, Red, Black);
		}

		if(strcmp(key.destination, "erase") == 0) {
			memset(expression, '\0', strlen(expression));
			//printf("Erase == %s\n", expression);
			LCD_FillRectangle(0,0,320,20, Black);
		}

		if(strcmp(key.destination, "compute") == 0 && strlen(expression) != 0) {
			LCD_FillRectangle(0,0,320,20, Black);

			char *result = calloc(sizeof(char), 256);
			result = ComputeExpression(expression);
			f_finished = 1;
			if(strlen(result) < EXPRESSION_LENGTH) {
				GUI_Text(5,0, result, Red, Black);
			}
			else
				GUI_Text(5,0, "The result is out of range", Red, Black);
			//printf("STRLEN ==== %d\n", strlen(result));
			memset(expression, '\0', strlen(expression));
		}
		//printf("%s\n", key.text);
		//here must be delay....
		IRF_Delay_BTN(100);

		//we've finished pressing
		IRF_DrawButton(key, Blue, Red, Black);
	}
}
//==================================================================================================

//Provides a work of interface
void IRF_Work(char *expression) {
	//first line
	IRF_InfButtonPressed(keys[0][0], expression);
	IRF_InfButtonPressed(keys[0][1], expression);
	IRF_InfButtonPressed(keys[0][2], expression);
	IRF_InfButtonPressed(keys[0][3], expression);
	IRF_InfButtonPressed(keys[0][4], expression);
	IRF_InfButtonPressed(keys[0][5], expression);
	//second line
	IRF_InfButtonPressed(keys[1][0], expression);
	IRF_InfButtonPressed(keys[1][1], expression);
	IRF_InfButtonPressed(keys[1][2], expression);
	IRF_InfButtonPressed(keys[1][3], expression);
	IRF_InfButtonPressed(keys[1][4], expression);
	IRF_InfButtonPressed(keys[1][5], expression);
	//third line
	IRF_InfButtonPressed(keys[2][0], expression);
	IRF_InfButtonPressed(keys[2][1], expression);
	IRF_InfButtonPressed(keys[2][2], expression);
	IRF_InfButtonPressed(keys[2][3], expression);
	IRF_InfButtonPressed(keys[2][4], expression);
	//fourth line
	IRF_InfButtonPressed(keys[3][0], expression);
	IRF_InfButtonPressed(keys[3][1], expression);
	IRF_InfButtonPressed(keys[3][2], expression);
	IRF_InfButtonPressed(keys[3][3], expression);
}

//Check our expression on correctness
			/*if(CheckBrackets(_expression) == 1 && CheckSecondActionSigns(_expression,'+') == 1 && CheckSecondActionSigns(_expression,'-') == 1 && CheckFirstActionSigns(_expression,'*') == 1 && CheckFirstActionSigns(_expression,'/') == 1 && CheckComma(_expression) == 1 && CheckOtherStrangeCases(_expression) == 1) {
				struct Stack *polish_stack = GetPolishNotation(_expression);
				char *result = calloc(sizeof(char), 256);
				result = GetResult(polish_stack);
				////printf("Result === %s\n", result);
				if(strlen(result) <= expression_length) {
					GUI_Text(5,0, result, Red,Yellow);
					f_finished = 1;
				}
				else {
					GUI_Text(5,0, "Too long answer for output!", Red,Yellow);
					f_finished = 1;
				}
			}*/
