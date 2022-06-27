#ifndef INTERFACE_CALCULATOR_H_INCLUDED
#define INTERFACE_CALCULATOR_H_INCLUDED

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

void IRF_DrawButton(struct Key key, uint16_t colorButton, uint16_t colorText, uint16_t colorBG);
void IRF_DrawInterface(uint16_t colorButton, uint16_t colorText, uint16_t colorBG);
void IRF_InfButtonPressed(struct Key key, char *expression);
void IRF_Work(char *expression);

#endif // INTERFACE_CALCULATOR_H_INCLUDED
