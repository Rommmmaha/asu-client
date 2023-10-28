#include<raylib.h>
#include<string>
#include<vector>
#include<iostream>
using namespace std;
class element
{
public:
    string name;
    Vector2 pos, size;
    Color c_text = WHITE, c_backgound = { 255,255,255,100 };
    bool isButton = 0;
    element(string _name, Vector2 _pos, Vector2 _size, bool _isButton)
    {
        name = _name;
        pos = _pos;
        size = _size;
        isButton = _isButton;
    }
};
class user_interface
{
public:
    int menu = 0;
    Vector2 mouse;
    vector<vector<element>> layouts;
    /*
        0   main_menu
        1   play_menu
        2   create_menu
    */
    const int screen = GetCurrentMonitor(), screenWidth = GetMonitorWidth(screen), screenHeight = GetMonitorHeight(screen);
    int c_x(float a) const { return int(screenWidth / 100.0 * a); }
    int c_y(float a) const { return int(screenHeight / 100.0 * a); }

    void _DrawLayout(int layout) { for (auto i : layouts[layout]) { _DrawElement(i); } }
    void _DrawElement(element a)
    {
        if (a.isButton)
        {
            Color tmp = a.c_backgound;
            bool active = 0;
            if (c_x(a.pos.x) < mouse.x && mouse.x < c_x(a.pos.x + a.size.x))
            {
                if (c_y(a.pos.y) < mouse.y && mouse.y < c_y(a.pos.y + a.size.y))
                {
                    tmp.a = 200;
                }
            }
            DrawRectangleV({ 1.0f * c_x(a.pos.x), 1.0f * c_y(a.pos.y) }, { 1.0f * c_x(a.size.x), 1.0f * c_y(a.size.y) }, tmp);
        }
        _DrawText(a.name.c_str(), a.pos.x, a.pos.y, a.size.y, a.c_text);
    }
    void _DrawText(const char* text, int posX, int posY, int fontSize, Color color)
    {
        DrawText(text, c_x(posX), c_y(posY), c_y(fontSize), color);
    }
};

int main()
{
    InitWindow(100, 100, "ASU!");
    user_interface ui;
    {
        ui.layouts.push_back({});
        ui.layouts[0].push_back(element("ASU!", { 10,10 }, { 0,20 }, 0));
        ui.layouts[0].push_back(element(" Play", { 10,30 }, { 20,5 }, 1));
        ui.layouts[0].push_back(element(" Create", { 10,35 }, { 20,5 }, 1));
        ui.layouts[0].push_back(element(" Exit", { 10,40 }, { 20,5 }, 1));
    }
    while (!WindowShouldClose())
    {
        if (!IsWindowFullscreen())
        {
            SetWindowSize(ui.screenWidth, ui.screenHeight);
            ToggleFullscreen();
        }
        BeginDrawing();
        {
            ui.mouse = GetMousePosition();
            ClearBackground(BLACK);
            if (IsMouseButtonPressed(1))
            {

            }
            ui._DrawLayout(0);
            DrawFPS(0, 0);
        }
        EndDrawing();

    }
    CloseWindow();
    return 0;
}