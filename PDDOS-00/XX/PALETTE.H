#ifndef PALETTE_H
#define PALETTE_H



typedef struct Palette Palette;



struct Palette {
	int x,y;
	int w,h;
	int z;
	int c;
	int cx,cy;
};



#ifdef PALETTE_IMPLEMENTATION



Palette *Palette_New(int x,int y,int w,int h,int z,int c) {
	Palette *palette=(Palette*)malloc(sizeof(Palette));

	if(palette) {
		palette->x=x;
		palette->y=y;
		palette->w=w;
		palette->h=h;
		palette->z=z;
		palette->c=c;
		palette->cx=0;
		palette->cy=0;
	}

	return palette;
}

void Palette_Free(Palette *palette) {
	free(palette);
}

void Palette_Draw(byte *srf,Palette *palette) {
	int i,j;
	for(j=0;j<palette->h;j++) {
		for(i=0;i<palette->w;i++) {
			FillRect(srf,i*palette->z+palette->x,j*palette->z+palette->y,palette->z,palette->z,palette->w*j+i);
		}
	}

	DrawRect(srf,palette->c*palette->z+palette->x,palette->y,palette->z,palette->z,12);
	DrawRect(srf,palette->c*palette->z+palette->x+1,palette->y+1,palette->z-2,palette->z-2,0);

}


void Palette_Update(Palette *palette,int mouse_x,int mouse_y,int mouse_buttons) {
	if(mouse_buttons & 0x01) {
		if(inrect(mouse_x,mouse_y,palette->x,palette->y,palette->w*palette->z,palette->h*palette->z)) {
			int x=mouse_x/palette->z;
			if(x>=0 && x<=palette->w) {
				palette->c=x;
			}
		}
  }
}



#endif /* PALETTE_IMPLEMENTATION */



#endif /* PALETTE_H */