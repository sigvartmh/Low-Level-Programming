#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include "framebuffer.c"
#include "main.c"

int x, y;
int gamepad;
uint32_t input;


void getInput() {
	int err = read(gamepad, &input, 4);
	if(err == -1) {
		printf("%i\n", errno);
	}
	printf("%x\n", input);
}

int initGamepad() {
	gamepad = open("/dev/Gamepad", O_RDWR);
	if(gamepad == -1) {
		printf("Failed to load gamepad!\n");
		printf("ERRNO: %i\n", errno);
		return -1;
	}
	
	if(signal(SIGIO, &getInput) == SIG_ERR) {
		printf("Failed to register signal handler!\n");
		return -1;
	}
	
	int err = fcntl(gamepad, F_SETOWN, getpid());
	if(err == -1) {
		printf("Failed to set PID\n");
		return -1;
	}
	
	long oflags = fcntl(gamepad, F_GETFL);
	if(fcntl(gamepad, F_SETFL, oflags | FASYNC) == -1) {
		printf("Error setting ut fasync");
		return -1;
	}
	
	return 1;
}

int isPressed(pos) {
	return (~input >> pos) & 0x1;
}

int main(int argc, char *argv[]) {
	printf("Loading game...\n");
	
	if(initGamepad() < 0) return -1;
	
	x = 0;
	y = 0;
	
	initFramebuffer();
	clear();
	gameMain();
	drawGameBoard();
	renderPlayer(&player1,0);
	renderPlayer(&player2,0);
	//drawRect(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,40,40);
	
	input = 0xFFFF;
	int counter = 0;
	while(1) {
		//counter = (counter + 1) % 4;
		//render(ball.x, ball.y, 4, 4, 0x0);
		//moveBall(&ball, player1, player2);
		//render(ball.x, ball.y, 4, 4, 0xFFFF);
		//renderAll();
		renderBall(&ball);
		
		if(counter == 0) {
			if(isPressed(3)) renderPlayer(&player1, 5);
			if(isPressed(1)) renderPlayer(&player1, -5);
			if(isPressed(5)) renderPlayer(&player2, 5);
			if(isPressed(7)) renderPlayer(&player2, -5);
			
			//if(isPresse)
		}
		
		if(isPressed(4)) break;
		
		if(x + 16 > SCREEN_WIDTH) x = SCREEN_WIDTH - 16;
		if(y + 16 > SCREEN_HEIGHT) y = SCREEN_HEIGHT - 16;
		if(x < 0) x = 0;
		if(y < 0) y = 0;
		
		//renderAll();
	}
	
	printf("Ending game...\n");

	exit(EXIT_SUCCESS);
}
