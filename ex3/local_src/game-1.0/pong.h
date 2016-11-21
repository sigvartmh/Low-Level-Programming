#ifndef PONG_H
#define PONG_H

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
    uint16_t width;
    uint8_t id;
} player_t;

typedef
struct Board {
    uint16_t max_width;
    uint16_t max_height;
} board_t;

ball_t ball;
player_t player1, player2;

void moveBall(ball_t *ball, player_t player1, player_t player2);

#endif
