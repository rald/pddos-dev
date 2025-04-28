#ifndef MOUSE_H
#define MOUSE_H

#include <dos.h>

void Mouse_Init(word *mouse_on,word *num_buttons);
void Mouse_Status(word *x,word *y,word *buttons);
void Mouse_Motion(int *ox,int *oy);
word Mouse_Pressed(word button);
word Mouse_Released(word button);
void Mouse_Set(word x,word y);

#ifdef MOUSE_IMPLEMENTATION

void Mouse_Init(word *mouse_on,word *num_buttons) {
	union REGS regs;
	regs.x.ax=0;
	int86(0x33,&regs,&regs);
	*mouse_on=regs.x.ax;
	*num_buttons=regs.x.bx;
}

void Mouse_Status(word *x,word *y,word *buttons) {
	union REGS regs;
	regs.x.ax=3;
	int86(0x33,&regs,&regs);
	*x=regs.x.cx;
	*y=regs.x.dx;
	*buttons=regs.x.bx;
}

void Mouse_Motion(int *dx,int *dy) {
	union REGS regs;
	regs.x.ax=0x0B;
	int86(0x33,&regs,&regs);
	*dx=(int)regs.x.cx;
	*dy=(int)regs.x.dx;
}

word Mouse_Pressed(word button) {
	union REGS regs;
	regs.x.ax=5;
	regs.x.bx=button;
	int86(0x33,&regs,&regs);
	return regs.x.bx;
}

word Mouse_Released(word button) {
	union REGS regs;
	regs.x.ax=6;
	regs.x.bx=button;
	int86(0x33,&regs,&regs);
	return regs.x.bx;
}

void Mouse_Set(word x,word y) {
	union REGS regs;
	regs.x.ax=4;
	regs.x.cx=x;
	regs.x.dx=y;
	int86(0x33,&regs,&regs);
}


#endif /* MOUSE_IMPLEMENTATION */



#endif /* MOUSE_H */
