#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//url to paper on SmoothLife: https://arxiv.org/pdf/1111.1567.pdf

/**************CONSTANTS THAT YOU CAN CHANGE***************/
#define WIDTH 10 //width of grid
#define HEIGHT 10 //height of grid
float alpha = 0.028; //alphaN
//float alpha = 0.147; //alphaM 
int ra = 21;


char level[] = " .:-=o+wxWX#$@"; //what each level brightness corresponds to in ASCII symbols.
#define level_count (sizeof(level)/sizeof(level[0]) - 1) //how many brightness levels there are defined in this program

float grid[HEIGHT][WIDTH] = {0}; //the global grid of floats that serves as the map of our automation

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

void display_grid(void){ //displays all the values of the grid in terms of levels
    for (size_t i = 0; i < HEIGHT; i++){
        for (size_t j = 0; j < WIDTH; j++){
            char x = level[(int)(grid[i][j]*level_count - 1)]; //set char x (char in array at (i,j)) to be equal to whichever char level the random float returns
            printf("%c ", x);
        }
        printf("\n");
    }
}

int emod(int a, int b){ return (a%b + b)%b;} //modulation for both positive and negative numbers

//computations for the three sigma equations
float sigma_one(float x, float a) {return 1.0f/(1.0f + expf(-(x-a)*(4/alpha)));}
float sigma_two();


int main(void){
    //set the seed of randomness to be dependent of current time
    srand(time(0));
    set_random_grid();

    //initiatlizing enviroment variables
    //initialize center x and y
    int cx = 0;
    int cy = 0;

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
    printf("m = %f, n = %f\n", m, n);

    //display_grid();
    


    return 0;
}