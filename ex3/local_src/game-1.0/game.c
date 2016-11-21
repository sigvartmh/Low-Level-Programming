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
#include "pong.c"

int gamepad;
uint32_t input;

/* Callback function to be registred on signal */
void getInput() {
	int err = read(gamepad, &input, 4);
	if(err == -1) {
		//printf("%i\n", errno);
		exit(EXIT_FAILURE);
	}
	//printf("%x\n", input);
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

/* Checks the value of which button is pressed */
int isPressed(pos) {
	return (~input >> pos) & 0x1;
}

int main(int argc, char *argv[]) {
	printf("Loading game...\n");
	
	if(initGamepad() < 0) return -1;
	initFramebuffer();

	clear();
	initGame();
	drawGameBoard();
	renderPlayer(&player1,0);
	renderPlayer(&player2,0);
	drawBall(&ball, 0xFFFF);
	input = 0xFFFF;
	
	while(1) {
		if(gamestate.state == STATE_INITIAL){
			if(isPressed(0)){
				gamestate.state = STATE_GAME;
				drawBall(&ball, 0x0000);
			}
		}
		else if(gamestate.state == STATE_RESET){
						
			sleep(3);
			render(ball.x, ball.y, 10, 10, 0x0000);
			resetGame();
			clear();
			drawGameBoard();
			renderPlayer(&player1,0);
			renderPlayer(&player2,0);
			drawBall(&ball, 0xFFFF);
			gamestate.state = STATE_INITIAL;
		}else{
			/* Check which buttons are pressed and move player accordingly */
			if(isPressed(3)) renderPlayer(&player1, 5);
			if(isPressed(1)) renderPlayer(&player1, -5);
			if(isPressed(7)) renderPlayer(&player2, 5);
			if(isPressed(5)) renderPlayer(&player2, -5);
			renderBall(&ball);
			usleep(50000);
		}
		
		if(isPressed(4)) break;
	}
	
	printf("Ending game...\n");

	exit(EXIT_SUCCESS);
}

