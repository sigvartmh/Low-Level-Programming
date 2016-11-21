#include <stdint.h>
#include <unistd.h>

#include <linux/fb.h>

#include <sys/ioctl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/mman.h>
#include "pong.h"

#define SCREEN_WIDTH 	320
#define SCREEN_HEIGHT 	240
#define BYTES_PER_PIXEL	2

int fbfd;
uint16_t *pixels;
struct fb_copyarea rect;
int xRect, yRect, wRect, hRect;

void renderAll();
void drawBall(ball_t *ball, uint16_t color);
void render(int x, int y, int w, int h, int col);

void initFramebuffer() {
	fbfd = open("/dev/fb0", O_RDWR);
	pixels = mmap(0, SCREEN_WIDTH*SCREEN_HEIGHT*BYTES_PER_PIXEL, PROT_WRITE | PROT_READ, MAP_SHARED, fbfd, 0);
	if (pixels == NULL) {
		printf("fb mmap failed\n");
	}	
	printf("Successfully init fb\n");
}

/*Clear the screen and remove the fb cursor blink */
void clear() {
	int cursor = open("/sys/class/graphics/fbcon/cursor_blink", O_RDWR);
	write(cursor, NULL, 1);
	int i;
	for(i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) {pixels[i] = 0x0;}
	rect.dx 		= 0;
	rect.dy 		= 0;
	rect.width 		= SCREEN_WIDTH;
	rect.height 	= SCREEN_HEIGHT;
	ioctl(fbfd, 0x4680, &rect);
}

/* Updates the rectangle smallest area needed to render the changes */
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

/* Draws a rectangel in position x, y with widht and height w,h */
void drawRect(int x, int y, int w, int h) {
	updateRect(x, y, w, h);
	int j=0;
	int i=0;
	for(i = y; i <= (y+h); i++){
		for(j = x; j <= (w+x); j++){
			pixels[j + i*SCREEN_WIDTH] = 0xFFFF;
		}
	}
	updateRect(0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
	renderAll();
}

void drawBall(ball_t *ball, uint16_t color){
	render(ball->x-5, ball->y, 10, 10, color);
	renderAll();
}

void drawGameBoard(){
	int i;

	for(i = board.min_width; i <= board.max_width; i++){
		pixels[i + 0*SCREEN_WIDTH] = 0xFFFF;
		pixels[i + 1*SCREEN_WIDTH] = 0xFFFF;
		
		pixels[i + (SCREEN_HEIGHT-3)*SCREEN_WIDTH] = 0xFFFF;
		pixels[i + (SCREEN_HEIGHT-4)*SCREEN_WIDTH] = 0xFFFF;
	}
	
	for(i = 0; i < SCREEN_HEIGHT; i++) {
		if(i % 10 > 5) continue;
		pixels[SCREEN_WIDTH/2 + i*SCREEN_WIDTH] = 0xFFFF;
	}

	updateRect(0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
	renderAll();
}

void render(int x, int y, int w, int h, int col) {
	uint16_t i, i2;
	for(i = x; i <= x + w; i++){
		if(i < 0 || i >= SCREEN_WIDTH) continue;
		for(i2 = y; i2 < y + h; i2++) {
			if(i2 < 4 || i2 >= SCREEN_HEIGHT - 4) continue;
			if(i == SCREEN_WIDTH/2 && i2 % 10 < 5) continue;
			pixels[i + i2*SCREEN_WIDTH] = col;
		}
	}
	updateRect(x, y, w, h);
}



// Renders and move the ball onto the screen
void renderBall(ball_t *ball){
	uint16_t start_x = ball->x;
	uint16_t start_y = ball->y;
	
	uint16_t width = 10;
	uint16_t height = 10;
	
	render(start_x, start_y, width, height, 0x00);
	renderAll();

	moveBall(ball, player1, player2);
	
	render(ball->x, ball->y, width, height, rand());
	renderAll();
}

void renderPlayer(player_t *player, int16_t offset){

	render(player->x - player->width, player->y - player->len/2, player->width, player->len, 0x00);
	movePlayer(player, offset);
	render(player->x - player->width, player->y - player->len/2, player->width, player->len, 0xFFFF);
	updateRect(player->x-4, 0, 4, SCREEN_HEIGHT);
	renderAll();
}

void renderAll() {
	//thun
	//printf("Rect\n:x:%i,\ty:%i,\tw:%i,\th:%i\n", rect.dx, rect.dy, rect.width, rect.height);
	ioctl(fbfd, 0x4680, &rect);
	xRect = -1;
	yRect = -1;
	wRect = -1;
	hRect = -1;
}

