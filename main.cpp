#include <raylib.h>
#include <iostream>
#include <raymath.h>
#include <deque>


using namespace std;

Color Light_Green = Color{173, 204, 96, 255};
float cellSize = 30;
int cellCount = 30;
// int offset = 75;

double lastUpdateTime = 0;

bool eventTrigger(double interval){
    double currentTime = GetTime();

    if (currentTime-lastUpdateTime>=interval){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool elementInDequeu(Vector2 element, deque<Vector2> deq){
    for (unsigned int i = 0; i<deq.size(); i++){
        if (Vector2Equals(deq[i], element)) return true;
    }
    return false;
}

class Snake{
    public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction {1, 0};

    bool addSegment = false;

    void Draw(){
        for (unsigned i = 0; i<body.size(); i++){
            float x = body[i].x;
            float y = body[i].y;
            DrawRectangleRounded(Rectangle{x * cellSize, y*cellSize, cellSize, cellSize}, 0.5, 6, DARKGRAY);
        }
    }

    void Update(){
        body.push_front(body[0] + direction);
        if (addSegment){
            addSegment = false;
        }
        else {
            body.pop_back();
        }
    }

    void Reset(){
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
    }
};

class Pellet{
    public:
    Vector2 position;
    Texture2D texture;

    Pellet(deque<Vector2> snakeBody){
        texture = LoadTexture("graphics/food.png");
        position = GenerateRandomPos(snakeBody);
    }

    ~Pellet(){
        UnloadTexture(texture);
    }

    void Draw(){
        DrawTexture(texture, position.x*cellSize, position.y*cellSize, WHITE);
    }

    Vector2 GenerateRandomCell(){
        float x = GetRandomValue(0, cellCount-1);
        float y = GetRandomValue(0, cellCount-1);
        return Vector2{x, y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody){
        Vector2 position = GenerateRandomCell();
        while (elementInDequeu(position, snakeBody)){
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Game{
public:
    Snake snake;
    Pellet food = Pellet(snake.body);
    bool running = true;

    void Draw(){
        food.Draw();
        snake.Draw();
    }

    void Update(){
        if (running){
            snake.Update();
            checkCollision();
            checkCollisionWithBorder();
            checkCollisionWithTail();
        }
    }

    void checkCollision(){
        if(Vector2Equals(snake.body[0], food.position)){
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
        }
    }

    void checkCollisionWithBorder(){
        if (snake.body[0].x == cellCount || snake.body[0].x == -1){
            GameOver();
        }
        if (snake.body[0].y == cellCount || snake.body[0].y == -1){
            GameOver();
        }
    }

    void checkCollisionWithTail(){
        deque<Vector2> bodyD = snake.body;
        bodyD.pop_front();
        if (elementInDequeu(snake.body[0], bodyD)) GameOver();
    }

    void GameOver(){
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
    }
};

int main(){
    const int width = cellSize*cellCount;
    const int height = cellSize*cellCount;
    InitWindow(width, height, "Snake");
    SetTargetFPS(60);
    Game game;


    while (!WindowShouldClose()){
        BeginDrawing();

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
            game.snake.direction = {0, -1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.snake.direction = {0, 1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.snake.direction = {1, 0};
            game.running = true;
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
            game.snake.direction = {-1, 0};
            game.running = true;
        }

        if (eventTrigger(0.2)) game.Update();

        ClearBackground(Light_Green);
        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}