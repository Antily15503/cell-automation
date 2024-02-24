#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 100 //width of grid
#define HEIGHT 100 //height of grid
char level[] = " .-+=oxX8#@"; //what each level brightness corresponds to in ASCII symbols.
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

int main(void){
    //set the seed of randomness to be dependent of current time
    srand(time(0));
    set_random_grid();
    display_grid();
    


    return 0;
}