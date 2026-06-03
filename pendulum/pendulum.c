#include <raylib.h>
#include <math.h>
#include <stdio.h>

#define WIDTH  1200
#define HEIGHT  1200
#define TITLE  "Pendulum"
#define FPS 40 

#define g 9.81    // gravity force 
#define FONT_SIZE 36

typedef struct String_coord {
   float pivot_x;
   float pivot_y;
} string_t;

typedef struct Circle_coord {
   float pos_x;
   float pos_y;
   float radius;  
} circle_t;

float dispersion = 0.005f;
float initial_angle = 0.0f; 
float time = 0.0f;
float dt = 0.2f;
float T = 0.0f; 
float length  = 0.0f;

void init_pendulum(circle_t *circle, string_t *string){
    float dx = circle->pos_x - string->pivot_x;
    float dy = circle->pos_y - string->pivot_y;

    // Rope length = distance from pivot to bob
    length = sqrtf(dx * dx + dy * dy);

    // Angle from vertical (downward Y axis)
    initial_angle = atan2f(dx, dy);
}

void calculating_coords_circle(circle_t *circle, string_t *string){
    float sign = (initial_angle >= 0.0f) ? 1.0f : -1.0f;
    float amplitude = fabsf(initial_angle) - dispersion * time;
    if (amplitude < 0.0f){
        amplitude = 0.0f;
    }
    float theta = sign * amplitude * cosf((2.0f * PI / T) * time);
    circle->pos_x = string->pivot_x + length * sinf(theta);
    circle->pos_y = string->pivot_y + length * cosf(theta);

    time += dt;
}

void draw_pendulum(circle_t *circle, string_t *string){
    calculating_coords_circle(circle, string);
    DrawLine(string->pivot_x, string->pivot_y,
             circle->pos_x,   circle->pos_y, WHITE);
    DrawCircle(circle->pos_x, circle->pos_y, circle->radius, WHITE);
}

void draw_telemetry(char* string, circle_t *circle){
    sprintf(string, "X:%.2f\nY:%.2f", circle->pos_x, circle->pos_y);
    DrawText(string, WIDTH - 160.0f, HEIGHT - FONT_SIZE * 2, FONT_SIZE, WHITE);     
}

int main(int argc, char *argv[]) {
    SetTargetFPS(FPS);
    InitWindow(WIDTH, HEIGHT,TITLE);
    circle_t circle = { .pos_x = 100.0f, 
                        .pos_y = 600.0f,
                        .radius = 50.0f};
    string_t string = { .pivot_x = 600.0f,
                        .pivot_y = 0.0f};

    init_pendulum(&circle, &string);
    T = 2 * PI * sqrt(length / g); 
    char str[30];
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        draw_pendulum(&circle, &string);
        draw_telemetry(str, &circle);
        EndDrawing();
    }   
    return 1;
}
