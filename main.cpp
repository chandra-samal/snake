#include <raylib.h>
#include <raymath.h>
#include <iostream>
// #include <rshapes.h>

using namespace std;

int player_score = 0;
int cpu_score = 0;

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};


class Ball{
    public:
    float x, y;
    int speed_x, speed_y;
    int radius;
    
    void Draw(){
        DrawCircle(x, y, radius, Yellow);
    }

    void Update(){
        x += speed_x;
        y += speed_y;

        if (y+radius >= GetScreenHeight() || y - radius <= 0) 
        speed_y *= -1;

        if (x+radius >= GetScreenWidth()){
            cpu_score++;
            Reset();
        }
        if (x - radius <= 0) {
            player_score++;
            Reset();
        }
    }

    void Reset(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speedChoices[2] = {-1, 1};
        speed_x *= speedChoices[GetRandomValue(0, 1)];
        speed_y *= speedChoices[GetRandomValue(0, 1)];
    }
};

class Paddle{
    protected:
    void LimitMovement(){
        if (y<=0) y = 0;
        if (y>=GetScreenHeight() - height) y = GetScreenHeight() - height;
    }

    public:
    float x, y;
    float width, height;
    int speed;

    void Draw(){
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update(){
        if(IsKeyDown(KEY_UP)) y -= speed;
        if(IsKeyDown(KEY_DOWN)) y += speed; 
        LimitMovement();
    }
};

class CPUpaddle: public Paddle{
    public:

    void update(int ball_y){
        if (y + height/2 > ball_y) y -= speed;
        if (y + height/2 < ball_y) y += speed;
        LimitMovement();
    }
};

Ball ball;
Paddle player;
CPUpaddle cpu;

int main(void){
    const int width = 640;
    const int height = 640;
    InitWindow(width, height, "game");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = width/2;
    ball.y = height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 32;
    player.height = 128;
    player.x = width - player.width - 10;
    player.y = height/2 - player.height/2;
    player.speed = 6;

    cpu.width = player.width;
    cpu.height = player.height;
    cpu.x = 10;
    cpu.y = height/2 - cpu.height/2;
    cpu.speed = 6;

    while (!WindowShouldClose()){
        BeginDrawing();

        //Updating
        ball.Update();
        player.Update();
        cpu.update(ball.y);

        //Checking for collision
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})){
            ball.speed_x *= -1;
        }
        
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
            ball.speed_x *= -1;
        }

        // Drawing
        ClearBackground(Dark_Green);
        DrawRectangle(width/2, 0, width/2, height, Green);
        DrawCircle(width/2,height/2, 125, Light_Green);
        DrawLine(width/2, 0, width/2, height, WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", cpu_score), width/4, 20, 30, WHITE);
        DrawText(TextFormat("%i", player_score), 3*width/4, 20, 30, WHITE);


        EndDrawing();
    }


    CloseWindow(); 
    return 0;
}