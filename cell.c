#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//url to paper on SmoothLife: https://arxiv.org/pdf/1111.1567.pdf

/**************CONSTANTS THAT YOU CAN CHANGE***************/
#define WIDTH 10 //width of grid
#define HEIGHT 10 //height of grid
float alpha = 0.028; //alphaN
//float alpha = 0.147; //alphaM 

int ra = 21;//outer barrier radius
float dt = 1.0f;//change in time per change in frame

/**************LIFE AND DEATH CONSTANTS yOu CAN CHANGE***************/
//b and d parameters represent the death and birth intervals
float b1 = 0.278;
float b2 = 0.365;
float d1 = 0.267;
float d2 = 0.445;

char level[] = " .:-=o+wxWX#$@"; //what each level brightness corresponds to in ASCII symbols.
#define level_count (sizeof(level)/sizeof(level[0]) - 1) //how many brightness levels there are defined in this program

float grid[HEIGHT][WIDTH] = {0}; //the global grid of floats that serves as the map of our automation
float gridNext[HEIGHT][WIDTH] = {0}; //what the grid of the next frame looks like

float rand_float(void){  //returns a float somewhere between the value 0 and 1
    return (float) rand() / (float)RAND_MAX;
}

void set_random_grid(void){ //creates a random grid of variables
    for (size_t i = 0; i < HEIGHT; i++){
        for (size_t j = 0; j < WIDTH; j++){
            grid[i][j] = rand_float();
        }
    }
}

void display_grid(float grid[HEIGHT][WIDTH]){ //displays all the values of the grid in terms of levels
    for (size_t i = 0; i < HEIGHT; i++){
        for (size_t j = 0; j < WIDTH; j++){
            char x = level[(int)(grid[i][j]*level_count - 1)]; //set char x (char in array at (i,j)) to be equal to whichever char level the random float returns
            printf("%c ", x);
        }
        printf("\n");
    }
}

int emod(int a, int b){ return (a%b + b)%b;} //modulation for both positive and negative numbers

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



int main(void){
    //set the seed of randomness to be dependent of current time
    srand(time(0));
    set_random_grid();
    //update grid
    compute_next_grid();
    for(size_t y = 0; y < HEIGHT; y++){
        for (size_t x = 0; x < WIDTH; x++){
            grid[y][x] += dt*gridNext[y][x];
        }
    }
    display_grid(grid);
    return 0;
}