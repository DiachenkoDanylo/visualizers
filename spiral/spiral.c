#include <raylib.h>
#include <math.h>

#define WIDTH  800
#define HEIGHT  800
#define TITLE  "MAIN"
#define FPS 200 

float start_pos_x = (WIDTH/2.0);
float start_pos_y = 0;
const float STEP = 5.0f;
const float RADIUS_STEP = 0.2f;
float starting_radius = WIDTH/2.0;
float angle = 0.0;
const int OFFSET = WIDTH/2;

void calculation_coord(float *x, float *y,
                       float *radius,
                       float *angle)
{
    if(*radius < 1){
        return;
    }
    *x = *radius * cosf(*angle);
    *y = *radius * sinf(*angle);
    float delta = STEP / *radius;
    *angle += delta;
    *radius -= RADIUS_STEP;
}

void draw_spiral(float *x, float *y)
{
    calculation_coord(
        x,
        y,
        &starting_radius,
        &angle
    );

    DrawRectangle(
        *x + OFFSET,
        *y + OFFSET,
        2,
        2,
        WHITE
    );
}

int main(int argc, char *argv[]) {

    SetTargetFPS(FPS);
    InitWindow(WIDTH, HEIGHT,TITLE);
    while(!WindowShouldClose()){
        BeginDrawing();
        draw_spiral(&start_pos_x,&start_pos_y);
   //     ClearBackground(BLACK);
        EndDrawing();
    }   
    return 1;
}
