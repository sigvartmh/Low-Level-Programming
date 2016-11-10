#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<string.h>

//#include <SDL2/SDL.h>

#define BALL_INIT_SPEED 4
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define UINT16_MAX 65535

typedef
struct _state {
    enum {
        STATE_INITIAL = 0,
        STATE_GAME = 1,
        STATE_MISSED_LEFT,
        STATE_MISSED_RIGHT,
        STATE_NEXT_ROUND,
        STATE_RESET,
        STATE_RESTART,
        STATE_FAILURE = -1
    } state;
} state_t;

typedef
struct Ball {
    uint16_t x;
    uint16_t y;
    int16_t dx;
    int16_t dy;
    int16_t speed;
} ball_t;

typedef
struct Player{
    uint16_t score;
    uint16_t x;
    uint16_t y;
    uint16_t len;
    uint8_t id;
} player_t;

typedef
struct Board {
    uint16_t max_width;
    uint16_t max_height;
} board_t;

void initBall(ball_t *ball){

    ball->x = SCREEN_WIDTH/2;
    ball->y = SCREEN_HEIGHT/2;

    ball->speed = BALL_INIT_SPEED;

    ball->dx = (rand() % 2 == 0) ? BALL_INIT_SPEED : -1*BALL_INIT_SPEED;
    ball->dy = (rand() % 2 == 0) ? BALL_INIT_SPEED : -1*BALL_INIT_SPEED;

}

uint16_t maskBit(uint8_t startBit, uint8_t endBit)
{
       uint16_t r = 0;
          for (uint8_t i=startBit; i<=endBit; i++)
                     r |= 1 << i;

             return r;
}

void hitWall(ball_t *ball, player_t player1, player_t player2){
    if(ball->x == player1.x){
        if(ball->y > player1.y + player1.len || ball->y < player1.y - player1.len){
            printf("\nPlayer 1 loose\n");
            player2.score++;
        }else{
            ball->dx = -1*(ball->dx); //maybe add speed to ball
            printf("\nPlayer 1 hit ball\n");
        }
    }else{
        if(ball->y > player2.y + player2.len || ball->y < player2.y - player2.len){
            printf("\nplayer 2 loose\n");
            player1.score++;
        }else{
            ball->dx = -1*(ball->dx); //maybe add speed to ball and change angle?
            printf("\nPlayer 2 hit ball\n");
        }
    }
}

void moveBall(ball_t *ball, player_t player1, player_t player2){

    if(((ball->x + ball->dx) <= SCREEN_WIDTH) && ((ball->x + ball->dx) >= 0)){
        ball->x = ball->x + ball->dx;
    }else{
        hitWall(ball,player1, player2);
    }

    if(((ball->y + ball->dy) <= SCREEN_HEIGHT) && ((ball->y + ball->dy) >= 0)){
        ball->y = ball->y + ball->dy;
    }else{
        ball->dy = -1*ball->dy;
        //Calculate bounce angel
    }
}


void movePlayer(player_t *player, uint16_t offset){
    if((player->y + player->len/2 + offset) > SCREEN_HEIGHT
        || (player->y - player->len/2 + offset) < 0){
        //do nothing
    }else{
        player->y = player->y + offset;
    }
}

int main(){
    /*
    SDL_Window* window;
    SDL_Init(SDL_INIT_EVERYTHING);
    window= SDL_CreateWindow("Pong",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640,
            480,
            SDL_WINDOW_OPENGL);
    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
i
    SDL_Delay(3000);
    SDL_DestroyWindow(window);
    SDL_Quit();
    */
    uint16_t red = maskBit(11,15);
    uint16_t green = maskBit(5,10);
    uint16_t blue = maskBit(0,4);

    uint16_t screen[SCREEN_WIDTH][SCREEN_HEIGHT];
    memset(screen, 'Z', sizeof(screen[0][0])*SCREEN_WIDTH*SCREEN_HEIGHT);
    srand(time(NULL));

    ball_t ball;
    initBall(&ball);

    player_t player1 = {
        .id = 1,
        .score = 0,
        .x = 0,
        .y = SCREEN_HEIGHT/2,
        .len = 100
    };

    player_t player2 = {
        .id = 2,
        .score = 0,
        .x = SCREEN_WIDTH,
        .y = SCREEN_HEIGHT/2,
        .len = 100
    };

    printf("Hello world\n");

    for(int i = 0; i< 50; i++){
        printf("Ball {\n Pos: %d x, %d y\n Dx: %d, Dy %d\n Speed: %d\n}\n", ball.x, ball.y, ball.dx, ball.dy, ball.speed);
        moveBall(&ball, player1, player2);
        movePlayer(&player1,1);
    }

        printf("Player {\n Id: %d\n Pos: %d x, %d y\n Len: %d\n Score: %d\n}\n", player1.id, player1.x, player1.y, player1.len, player1.score);
        printf("Player {\n Id: %d\n Pos: %d x, %d y\n Len: %d\n Score: %d\n}\n", player2.id, player2.x, player2.y, player2.len, player2.score);
    /*
    for(uint16_t i = 0; i < SCREEN_WIDTH; i++){
        for(uint16_t j = 0; j < SCREEN_HEIGHT; j++){
        printf("[r:%d, g:%d, b:%d]",  (screen[i][j] & red) >> 11 , screen[i][j] & green >> 5, screen[i][j] & blue);
        }
        printf("\n");
    }*/

    //printf("test: %d", screen[0][0]);
    //printf("test: %d", screen[0][1]);
    return 0;
}


