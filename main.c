#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

//GFX
#define BIRD '>'
#define EMPTY ' '
#define GROUND '_'
#define WALL '#'

//SCREEN MATRIX
#define SCREEN_WIDTH 30
#define SCREEN_HEIGHT 10
char screen[SCREEN_HEIGHT][SCREEN_WIDTH];

//PHYSICS (ALL TIED TO FRAMETIME)
#define FRAMETIME 33

//BIRD FLAPPING
#define FRAMES_FOR_BIRD_FALL 3
#define TILES_PER_FALL 1
#define TILES_PER_FLOP 1
int birdFallFrameCounter = 0;

//WALLS CREATION
#define FRAMES_FOR_WALL_FORMATION 20
#define BLANK_TILES_PER_WALL 3
int wallSpawnFrameCounter = 0;

//SCORING SYSTEM
int score = -2;


void printScreen();
//cleans and prints the state of the whole screen matrix

void initScreen();
//initializes the screen to have EMPTY in all "pixels" except the ground, which is set to GROUND

void birdUpdate(int *birdPosition, int flopped);
//updates the bird position based on the input (flopped or not)

void wallsUpdate(); //updates the positions of the walls

void spawnWall(); //spawns walls at the right when called

int checkCollision(int birdPosition);
//returns 1 if collision between walls and bird found, 0 if not (used for game over), -1 if error

void gameOver();
//executes the game over sequence


int main(void) {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    srand(time(NULL));

    while (1) {
        score = -2; //counter resetting
        birdFallFrameCounter = 0;
        wallSpawnFrameCounter = 0;
        initScreen();

        int birdPosition = 5; //storing bird position (row of matrix)
        int flopped = 0; //var to store flap status, used in bird update per frame

        birdUpdate(&birdPosition, flopped);

        birdPosition = 5;
        printf("press any key to start..., q to quit. Use spacebar to flop!");
        if(getch() == 'q') {
            break;
        }

        while(1) {
            flopped = 0;
            char a = 0;
            if(_kbhit()) {
                a = _getch();
            }

            if(a == 'q') { break; }

            if(a == ' ') { flopped = 1; }

            birdUpdate(&birdPosition, flopped);
            int collision = checkCollision(birdPosition);

            if(collision == 1) {
                gameOver();
                break;
            }
            wallsUpdate();
            printScreen();
            if (birdPosition >= SCREEN_HEIGHT) {
                gameOver();
                break;
            }
            Sleep(FRAMETIME);
        }
    }
    system("cls");
    printf("Thanks for playing!");
    Sleep(1500);
    return 0;
}

void printScreen() {
    system("cls");
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            printf("%c", screen[i][j]);
        }
        printf("\n");
    }
}

void initScreen() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) { //drawing void
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            screen[i][j] = EMPTY;
        }
    }
    for (int i = 0; i < SCREEN_WIDTH; i++) { //drawing ground
        screen[SCREEN_HEIGHT - 1][i] = GROUND;
    }
}

void birdUpdate(int *birdPosition, int flopped) {
    if(flopped) {
        (*birdPosition) -= TILES_PER_FLOP;
    }
    else {
        birdFallFrameCounter++;
        if (birdFallFrameCounter == FRAMES_FOR_BIRD_FALL) {
            (*birdPosition) += TILES_PER_FALL;
            birdFallFrameCounter = 0;
        }
    }

    if(*birdPosition < 0) { *birdPosition = 0; }
    if(*birdPosition >= SCREEN_HEIGHT) { return; }

    for(int i = 0; i < SCREEN_HEIGHT; i++) {
        screen[i][1] = EMPTY;
    }
    screen[*birdPosition][1] = BIRD;
}

void wallsUpdate() {
    for(int i = 0; i < SCREEN_HEIGHT-1; i++) {
        for(int j = 2; j < SCREEN_WIDTH-1; j++) {
            screen[i][j] = screen[i][j+1];
        }
        screen[i][SCREEN_WIDTH - 1] = EMPTY;
    }
    if(screen[0][2] == WALL || screen[0][8] == WALL) {
        score++;
    }
    if(wallSpawnFrameCounter == FRAMES_FOR_WALL_FORMATION) {
        spawnWall();
        wallSpawnFrameCounter = 0;
    }
    wallSpawnFrameCounter++;
}

void spawnWall() {

    int randomGapPosition =  rand() % (SCREEN_HEIGHT - BLANK_TILES_PER_WALL + 1);

    for (int i = 0; i < SCREEN_HEIGHT-1; i++) {
        if (i >= randomGapPosition && i < randomGapPosition + BLANK_TILES_PER_WALL) {
            screen[i][SCREEN_WIDTH - 1] = EMPTY;
        }
        else {
            screen[i][SCREEN_WIDTH - 1] = WALL;
        }

    }
}

int checkCollision(int birdPosition) {
    if(screen[birdPosition][2] == WALL) {
        return 1;
    }
    return 0;
}

void gameOver() {
    printf("GAME OVER! BIRD CRASHED!\n");
    printf("SCORE: %d\n", score > 0 ? score : 0);
    Sleep(2500);
    system("cls");
}
