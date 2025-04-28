#define GRAPHICS_IMPLEMENTATION
#include "graphics.h"

#define MOUSE_IMPLEMENTATION
#include "mouse.h"

#define CANVAS_IMPLEMENTATION
#include "canvas.h"

#define PALETTE_IMPLEMENTATION
#include "palette.h"

#define INPUT_FILE "input.cvs"
#define OUTPUT_FILE "input.cvs"



dword pal[] = {
	0x1A1C2CL,
	0x5D275DL,
	0xB13E53L,
	0xEF7D57L,
	0xFFCD75L,
	0xA7F070L,
	0x38B764L,
	0x257179L,
	0x29366FL,
	0x3B5DC9L,
	0x41A676L,
	0x73EFF7L,
	0xF4F4F4L,
	0x94B0C2L,
	0x566C86L,
	0x333C57L
};



void DrawText(byte* srf,Canvas *font,int x,int y,int z,char *text) {
	int i;
	int xc=x,yc=y;
	for(i=0;i<strlen(text);i++) {
		Canvas_Draw(srf,font,xc,yc,text[i],z);
		xc+=font->w;
		if(xc>=SCREEN_WIDTH) {
			xc=0;
			yc+=font->h;
			if(yc>=SCREEN_HEIGHT) {
				break;
			}
		}
	}
}




int main() {

	int i,j;

	Canvas *canvas;

	byte *buf=calloc(SCREEN_SIZE,sizeof(byte));

	int f=0;
	int z=8;
	int c=12;
	bool g=true;

	word start;

	Canvas *mouse=NULL;

	word mouse_on=0;
	word num_buttons=0;

	word mouse_x=0,mouse_y=0;
	word mouse_buttons=0;

	bool quit=false;

	int key;

	Palette *palette=NULL;

	Canvas *font=NULL;

	Canvas *icons=NULL;

	char msg[16];

	unsigned long frame=0;

	bool hold=false;



	srand(time(NULL));



	SetMode(0x13);

	for(i=0;i<16;i++) {
		int r=(int)((pal[i] & 0xFF0000L) >> 16);
		int g=(int)((pal[i] & 0x00FF00L) >> 8);
		int b=(int)((pal[i] & 0x0000FFL));
		r=(int)((double)r/255*63);
		g=(int)((double)g/255*63);
		b=(int)((double)b/255*63);

		SetPalette(i,r,g,b);
	}



	Mouse_Init(&mouse_on,&num_buttons);

	if(!mouse_on) {
		printf("Error: cannot initialize mouse\n");
		return 1;
	}

	palette=Palette_New(0,SCREEN_HEIGHT-16,16,1,16,12);

	canvas=Canvas_LoadCVS(INPUT_FILE);

	mouse=Canvas_LoadCVS("mouse.cvs");

	icons=Canvas_LoadCVS("icons.cvs");

	font=Canvas_LoadCVS("font-00.cvs");


	start=*myclock;

	while(!quit) {

		Mouse_Status(&mouse_x,&mouse_y,&mouse_buttons);

		if(kbhit()) {
			key=getch();
			switch(key) {
			case 27: quit=true; break;
			case 'w':
				mouse_y=(mouse_y-z)/z*z+z/2;
				if(mouse_y<z/2) mouse_y=z/2;
				Mouse_Set(mouse_x,mouse_y);
				break;
			case 's':
				mouse_y=(mouse_y+z)/z*z+z/2;
				if(mouse_y>199-z/2) mouse_y=199-z/2;
				Mouse_Set(mouse_x,mouse_y);
				break;
			case 'a':
				mouse_x=(mouse_x-z*2)/z*z+z/2+2;
				if(mouse_x<z/2) mouse_x=z/2;
				Mouse_Set(mouse_x,mouse_y);
				break;
			case 'd':
				mouse_x=(mouse_x+z*2)/z*z+z/2+2;
				if(mouse_x>639-z/2) mouse_x=639-z/2;
				Mouse_Set(mouse_x,mouse_y);
				break;
			case '1':
				if(canvas) {
					Canvas_Free(canvas);
					canvas=NULL;
				}
				canvas=Canvas_LoadCVS(INPUT_FILE);
				break;
			case '2':
				Canvas_SaveCVS(OUTPUT_FILE,canvas);
				break;
			default: break;
			}
		}

		if(mouse_x<0) mouse_x=0;
		if(mouse_y<0) mouse_y=0;
		if(mouse_x>639) mouse_x=639;
		if(mouse_y>199) mouse_y=199;

		memset(buf,0,SCREEN_SIZE);


		if(mouse_buttons & 0x01) {
			if(inrect(mouse_x>>1,mouse_y,0,0,canvas->w*z,canvas->h*z))  {
				int x=(mouse_x>>1)/z;
				int y=mouse_y/z;
				if(x>=0 && x<canvas->w && y>=0 && y<canvas->h) {
					canvas->pixels[f*canvas->w*canvas->h+y*canvas->w+x]=palette->c;
				}
			}
		}


		if(!hold) {

			if((mouse_buttons & 0x01) == 1) {

				if(inrect(mouse_x>>1,mouse_y,0,canvas->h*z+z,16,16))  {
					FillRect(buf,0,canvas->h*z+z,16,16,15);
					f--;
					if(f<0) f=canvas->frames-1;
					hold=true;
				}

				if(inrect(mouse_x>>1,mouse_y,16+z,canvas->h*z+z,16,16))  {
					FillRect(buf,16+z,canvas->h*z+z,16,16,15);
					f++;
					if(f>=canvas->frames) f=0;
					hold=true;
				}
			}

		} else {

			if((mouse_buttons & 0x01) == 0) {
				hold=false;
			}

		}

		sprintf(msg,"%2d",f);
		DrawText(buf,font,32+z+z,canvas->h*z+z+z/2,1,msg);



		Canvas_Draw(buf,canvas,0,0,f,z);

		Canvas_Draw(buf,canvas,SCREEN_WIDTH-canvas->w,0,f,1);

		if(g) {
			for(i=0;i<=canvas->w;i++) {
				DrawLine(buf,i*z,0,i*z,canvas->h*z,15);
			}

			for(j=0;j<=canvas->h;j++) {
				DrawLine(buf,0,j*z,canvas->w*z,j*z,15);
			}
		}


		Palette_Update(palette,mouse_x>>1,mouse_y,mouse_buttons);

		Palette_Draw(buf,palette);


		Canvas_Draw(buf,icons,0,canvas->h*z+z,0,1);
		Canvas_Draw(buf,icons,16+z,canvas->h*z+z,1,1);


		sprintf(msg,"%7lu",frame);
		DrawText(buf,font,160,0,1,msg);

		Canvas_Draw(buf,mouse,mouse_x>>1,mouse_y,0,1);

		vsync();
		memcpy(VGA,buf,SCREEN_SIZE);

		frame++;

	}

	Canvas_Free(canvas);
	canvas=NULL;

	SetMode(0x03);

	return 0;
}
