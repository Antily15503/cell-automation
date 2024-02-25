#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

//url to paper on SmoothLife: https://arxiv.org/pdf/1111.1567.pdf

/**************CONSTANTS THAT YOU CAN CHANGE***************/
#define WIDTH 120 //width of grid
#define HEIGHT 70 //height of grid
float alpha = 0.028; //alphaN
//float alpha = 0.147; //alphaM 

int ra = 20;//outer barrier radius
float dt = 0.08f;//change in time per change in frame

/**************LIFE AND DEATH CONSTANTS yOu CAN CHANGE***************/
//b and d parameters represent the death and birth intervals
float b1 = 0.257;
float b2 = 0.336;
float d1 = 0.365;
float d2 = 0.549;

char level[] = " .:-=o+wxWX#$@"; //what each level brightness corresponds to in ASCII symbols.
#define level_count (sizeof(level)/sizeof(level[0]) - 1) //how many brightness levels there are defined in this program
#define gotoxy(x, y) printf("\033[%d;%dH]", (y), (x));

float grid[HEIGHT][WIDTH] = {0}; //the global grid of floats that serves as the map of our automation
float gridNext[HEIGHT][WIDTH] = {0}; //what the grid of the next frame looks like

float rand_float(void){  //returns a float somewhere between the value 0 and 1
    return (float) rand() / (float)RAND_MAX;
}

void set_random_grid(void){ //creates a random grid of variables

    /****GENERATE A RANDOM CANVAS OF RANDOMLY GENERATED VALUES OF DOTS****
    for (size_t i = 0; i < HEIGHT; i++){
        for (size_t j = 0; j < WIDTH; j++){
            if(rand_float() > 0.3){
                grid[i][j] = rand_float();
            }
        }
    }*/
    /****GENERATE A RANDOM CANVAS OF RANDOMLY GENERATED VALUE IN A small circle****/
    size_t w = WIDTH/3;
    size_t h = HEIGHT/2;
    for(size_t dy = 0; dy < h; dy++){
        for(size_t dx = 0; dx < w; dx++){
            size_t x = dx + WIDTH/2 - w/2;
            size_t y = dy + HEIGHT/2 - h/2;
            grid[y][x] = rand_float();
        }
    }
}

void display_grid(float grid[HEIGHT][WIDTH]){ //displays all the values of the grid in terms of levels
    gotoxy(0, 0); //move cursor to top-left corner of terminal
    for (size_t i = 0; i < HEIGHT; i++){
        for (size_t j = 0; j < WIDTH; j++){
            char x = level[(int)(grid[i][j]*(level_count - 1))]; //set char x (char in array at (i,j)) to be equal to whichever char level the random float returns
            fputc(x, stdout);
        }
        fputc('\n', stdout);
    }
    fputc('\r', stdout);
}

int emod(int a, int b){ return (a%b + b)%b;} //modulation for both positive and negative numbers

void clamp(float *x, float l, float h){ //clamp operation that forces whatever values of x to be between h and l
    if (*x < l) *x = l;
    if (*x > h) *x = h;
}

//computations for the three sigma equations and s equation
float sigma_one(float x, float a) {return 1.0f/(1.0f + expf(-(x-a)*(4/alpha)));}
float sigma_two(float x, float a, float b){return (sigma_one(x, a) * (1 - sigma_one(x, b)));}
float sigma_m(float x, float y, float m){return x*(1 - sigma_one(m, 0.5f)) + y*sigma_one(m, 0.5);}
float s(float n, float m){
    //b1, b2 and d1, d2 are the birth and death of cells modifier
    return sigma_two(n, sigma_m(b1, d1, m), sigma_m(b2, d2, m)); 
}

//function that computes the SmoothLife equations for all parts of the grid
void compute_next_grid(void){
    //initiatlizing enviroment variables
    //initialize center x and y with cy and cx and looping it for each grid
    for (int cy = 0; cy < HEIGHT; cy++){
        for (int cx = 0; cx < WIDTH; cx++){
            float m = 0, M = 0;
            float n = 0, N = 0;

            //establishing radius of checking
            float ra = 9; //NOTE: THIS VALUE CAN BE CHANGED TO ALTER CHECKING RADIUS

            float ri = ra/3; //inner circle radius

            //doing integration for the area around center point (x, y) given a radius ra
            for (int dy = -(ra-1); dy <= (ra - 1); dy++){
                for (int dx = -(ra - 1); dx <= (ra - 1); dx++){
                    int x = emod(cx + dx, WIDTH);
                    int y = emod(cy + dy, HEIGHT);

                    //check if the grid is within the inner or outer circle
                    if (dx*dx + dy*dy <= ri*ri) {
                        m += grid[y][x];
                        M += 1;
                    }
                    else if (dx*dx + dy*dy <= ra*ra) {
                        n += grid[y][x];
                        N += 1;
                    }
                }
            }
            m /= M; n /= N;
            float q = s(n, m); //temp val
            gridNext[cy][cx] = (2*q - 1);
        }
    }
}

int main(void) {
    struct timespec tim, rem;
    tim.tv_sec = 0;              // Sleep for 0 seconds initially
    tim.tv_nsec = 10;     // Sleep for 700 million nanoseconds (0.7 seconds)
    srand(time(0));
    set_random_grid();

    display_grid(grid);

    for (;;) {
        compute_next_grid();
        for (size_t y = 0; y < HEIGHT; y++) {
            for (size_t x = 0; x < WIDTH; x++) {
                grid[y][x] += dt * gridNext[y][x];
                clamp(&grid[y][x], 0, 1);
            }
        }
        display_grid(grid);
        fflush(stdout);
        nanosleep(&tim, &rem);
    }

    return 0;
}