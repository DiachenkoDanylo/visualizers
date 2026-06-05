#include <raylib.h>
#include <math.h>
#include <stdio.h>

#define WIDTH  800
#define HEIGHT  800
#define TITLE  "Double-Pendulum"
#define FPS 40 
#define g 9.81 
#define FONT_SIZE 18 
#define dt 0.16
#define DISPERSION 0.995f

typedef struct Coords {
   float x1;
   float x2;
   float y1;
   float y2;
} coords_t;

typedef struct Double_pendulum {
    float theta1;
    float omega1;
    float theta2;
    float omega2;
    float l1;
    float l2;
    float m1;
    float m2;
    float pivot_x;
    float pivot_y;
    coords_t coords;
} double_pendulum_t;

void update_double_pendulum(double_pendulum_t *p)
{
    float t1 = p->theta1;
    float t2 = p->theta2;

    float w1 = p->omega1;
    float w2 = p->omega2;

    float m1 = p->m1;
    float m2 = p->m2;

    float l1 = p->l1;
    float l2 = p->l2;

    float delta = t2 - t1;

    float den1 =
        (m1 + m2) * l1 -
        m2 * l1 * cosf(delta) * cosf(delta);

    float alpha1 =
        (m2 * l1 * w1 * w1 * sinf(delta) * cosf(delta)
        + m2 * g * sinf(t2) * cosf(delta)
        + m2 * l2 * w2 * w2 * sinf(delta)
        - (m1 + m2) * g * sinf(t1))
        / den1;

    float den2 = (l2 / l1) * den1;

    float alpha2 =
        (-m2 * l2 * w2 * w2 * sinf(delta) * cosf(delta)
        + (m1 + m2) *
          (g * sinf(t1) * cosf(delta)
          - l1 * w1 * w1 * sinf(delta)
          - g * sinf(t2)))
        / den2;

    p->omega1 += alpha1 * dt;
    p->omega2 += alpha2 * dt;

    p->theta1 += p->omega1 * dt;
    p->theta2 += p->omega2 * dt;

    p->omega1 *= DISPERSION; 
    p->omega2 *= DISPERSION; 
}

void calculate_positions(double_pendulum_t *dp)
{
    dp->coords.x1 = dp->pivot_x + dp->l1 * sinf(dp->theta1);
    dp->coords.y1 = dp->pivot_y + dp->l1 * cosf(dp->theta1);

    dp->coords.x2 = dp->coords.x1 + dp->l2 * sinf(dp->theta2);
    dp->coords.y2 = dp->coords.y1 + dp->l2 * cosf(dp->theta2);
}

void draw_telemetry(char* string, char* string2,  double_pendulum_t *dp){
    sprintf(string, "DISPESION: %.4f\nMass1: %1.1f\nMass2: %1.1f",DISPERSION, dp->m1, dp->m2);
    DrawText(string, 50,HEIGHT - (FONT_SIZE * 4), FONT_SIZE * 1.2, WHITE);
    sprintf(string2, "X1: %3.2f\nY1: %3.2f\nX2: %3.2f\nY2: %3.2f", dp->coords.x1, dp->coords.y1, dp->coords.x2, dp->coords.y2);
    DrawText(string2, WIDTH - 200.0f, HEIGHT - (FONT_SIZE * 2 * 3) - 10, FONT_SIZE * 1.5, WHITE);     
}

double_pendulum_t init_double_pendulum(){  
    double_pendulum_t dp = {
        .l1 = 200, // LENGTH OF ROPE 
        .l2 = 200, // LENGTH OF ROPE
        .m1 = 1, // MASS1
        .m2 = 1, // MASS2
        .theta1 = 1.5f, //INITIAL ANGLES IN RAD. 1 RAD = 57
        .theta2 = 0.0f,
        .omega1 = 0, //INITIAL VELOCITY ( SET 0 to SIMULATE FREE FALL) 
        .omega2 = 0.5,
        .pivot_x = 400,
        .pivot_y = 0
    };
    return dp;
}

void draw_pendulum(float *x, float *y, float pivot_x, float pivot_y, float *radius){
    DrawLine(pivot_x, pivot_y, *x,* y, WHITE);
    DrawCircle(*x, *y, *radius, WHITE);
}

int main(int argc, char *argv[]) {
    SetTargetFPS(FPS);
    InitWindow(WIDTH, HEIGHT,TITLE);
    char str[30];
    char str2[30];
    float radius = 25.0f; // RADIUS OF CIRCLE
    double_pendulum_t dp = init_double_pendulum();
    while(!WindowShouldClose()){
        BeginDrawing();
        update_double_pendulum(&dp);
        calculate_positions(&dp);      
        draw_pendulum(&dp.coords.x1, &dp.coords.y1, dp.pivot_x, dp.pivot_y, &radius); 
        draw_pendulum(&dp.coords.x2, &dp.coords.y2, dp.coords.x1, dp.coords.y1, &radius); 
        draw_telemetry(str, str2, &dp);
        ClearBackground(BLACK);
        EndDrawing();
    }   
    return 1;
}
