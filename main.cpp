#include<raylib.h>
#include<iostream>
using namespace std;
int main()
{
    const Vector2 screen = { 800,450 };
    InitWindow(int(screen.x), int(screen.y), "Hello, World !");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}