#include <stdint.h>

#include <linux/fb.h>

#include <sys/ioctl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>

#define SCREEN_WIDTH 	320
#define SCREEN_HEIGHT 	240
#define BITS_PER_PIXEL	2

int fbfd;
uint16_t *pixels;
struct fb_copyarea rect;
int xRect, yRect, wRect, hRect;

void initFramebuffer() {
	fbfd = open("/dev/fb0", O_RDWR);
	pixels = mmap(0, SCREEN_WIDTH*SCREEN_HEIGHT*BITS_PER_PIXEL, PROT_WRITE, MAP_SHARED, fbfd, 0);
	
	printf("Successfully init fb\n");
}

void clear() {
	int i;
	for(i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) {pixels[i] = 0x0;}
	rect.dx 		= 0;
	rect.dy 		= 0;
	rect.width 		= SCREEN_WIDTH;
	rect.height 	= SCREEN_HEIGHT;
	ioctl(fbfd, 0x4680, &rect);
}

void updateRect(int x, int y, int w, int h) {
	if(xRect == -1) {
		xRect		= x;
		yRect		= y;
		wRect		= w;
		hRect		= h;
	} else {
		if(x < xRect) 						{wRect += xRect - x; xRect = x;}
		if(y < yRect) 						{hRect += yRect - y; yRect = y;}
		
		if(x + w > xRect + wRect) 			wRect = (x + w) - xRect;
		if(y + h > yRect + hRect)			hRect = (y + h) - yRect;
	}
	
	rect.dx		= xRect;
	rect.dy		= yRect;
	rect.width	= wRect;
	rect.height	= hRect;
}

void render(int x, int y, int w, int h, uint16_t col) {
	int i = 0;
	int i2 = 0;
	for(i = 0; i < w; i++) {
		int xDraw = x + i;
		for(i2 = 0; i2 < h; i2++) {
			int yDraw = y + i2;
			pixels[xDraw + yDraw*SCREEN_WIDTH] = col;
		}
	}
	
	updateRect(x, y, w, h);
}

void renderAll() {

	printf("%i,\t%i,\t%i,\t%i\n", rect.dx, rect.dy, rect.width, rect.height);
	ioctl(fbfd, 0x4680, &rect);
	xRect = -1;
	yRect = -1;
	wRect = -1;
	hRect = -1;
}
