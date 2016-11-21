#ifndef PONG_H
#define PONG_H

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
    uint16_t width;
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
    uint16_t color;
    uint8_t id;
} player_t;

typedef
struct Board {
    uint16_t min_width;
    uint16_t min_height;
    uint16_t max_width;
    uint16_t max_height;
} board_t;

ball_t ball;
board_t board;
state_t gamestate;
player_t player1, player2;

#define PLAYER_WIDTH 4
#define BOARDER_WIDTH 2

void moveBall(ball_t *ball, player_t player1, player_t player2);
void movePlayer(player_t *player, int16_t offset);
int initGame();

#endif

