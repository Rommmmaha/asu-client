#include<raylib.h>
#include<string>
#include<vector>
#include<iostream>
//#include<filesystem>
using namespace std;
string sum(string a, string b)
{
    a += b;
    return a;
}
bool containsSubstring(const char* str, const char* substring)
{
    return strstr(str, substring) != nullptr;
}
class video
{
public:
    int fps;
    string path;
    Texture2D tmp;
    void _genFrames()
    {

    }
    Texture2D _getFrame(unsigned int time)
    {
        string frames = path + "frames\\";
        if (tmp.id > 0) { UnloadTexture(tmp); }
        tmp = LoadTexture(path.c_str());
        return tmp;
    }
};
class element
{
public:
    int type = 0;
    // 0-text | 1-button | 2-image

    string name;
    Vector2 pos, size;
    Color c_text = WHITE, c_backgound = { 255,255,255,100 };
    Texture2D texture;

    element(string _name, Vector2 _pos, Vector2 _size, int _type)
    {
        name = _name;
        pos = _pos;
        size = _size;
        type = _type;
        if (_type == 2) { _LoadTexture(); }
    }
    void _LoadTexture()
    {
        if (texture.id > 0)
        {
            UnloadTexture(texture);
        }
        texture = LoadTexture(name.c_str());
    }
};
class user_interface
{
public:
    int layout = 1;
    Vector2 mouse = { 0,0 };
    int screen = GetCurrentMonitor(), screenWidth = GetMonitorWidth(screen), screenHeight = GetMonitorHeight(screen);
    vector<vector<element>> layouts = {};
    /*
        0   nothing
        1   main_menu
        2   play_menu
        3   create_menu
    */
    int c_x(float a) const { return int(screenWidth / 100.0 * a); }
    int c_y(float a) const { return int(screenHeight / 100.0 * a); }
    string _MousePress()
    {
        Vector2 a = mouse;
        for (auto& a : layouts[layout])
        {
            if (a.type == 1)
            {
                if (c_x(a.pos.x) < mouse.x && mouse.x < c_x(a.pos.x + a.size.x))
                {
                    if (c_y(a.pos.y) < mouse.y && mouse.y < c_y(a.pos.y + a.size.y))
                    {
                        return a.name;
                    }
                }
            }
        }
        return "";
    }
    void _DrawLayout() { for (auto& i : layouts[layout]) { _DrawElement(i); } }
    void _DrawElement(element a) const
    {
        if (a.type == 1)
        {
            Color tmp = a.c_backgound;
            if (c_x(a.pos.x) < mouse.x && mouse.x < c_x(a.pos.x + a.size.x))
            {
                if (c_y(a.pos.y) < mouse.y && mouse.y < c_y(a.pos.y + a.size.y))
                {
                    tmp.a = 200;
                }
            }
            DrawRectangleV({ 1.0f * c_x(a.pos.x), 1.0f * c_y(a.pos.y) }, { 1.0f * c_x(a.size.x), 1.0f * c_y(a.size.y) }, tmp);
        }
        if (a.type == 0 || a.type == 1)
        {
            _DrawText(a.name.c_str(), a.pos.x, a.pos.y, a.size.y, a.c_text);
        }
        if (a.type == 2)
        {
            DrawTexturePro(a.texture, { 0,0,float(a.texture.width),float(a.texture.height) }, { 0,0,float(screenWidth),float(screenHeight) }, { 0,0 }, 0, WHITE);
        }
    }
    void _DrawText(const char* text, float posX, float posY, float fontSize, Color color) const
    {
        DrawText(text, c_x(posX), c_y(posY), c_y(fontSize), color);
    }
    void _GetLevels()
    {
        layouts[2].clear();
        layouts[2].push_back(layouts[1][0]);
        layouts[2].push_back(element("ASU!", { 10,10 }, { 0,20 }, 0));
        FilePathList list1 = LoadDirectoryFiles(sum(GetWorkingDirectory(), "\\levels").c_str());
        for (int i = list1.count; i--;)
        {
            string temp = sum(list1.paths[i], "\\level.dat");
            if (FileExists(temp.c_str()))
            {
                layouts[2].push_back(element(sum(list1.paths[i], "\\music.mp3").c_str(), { 10,20.0f + layouts[2].size() * 5 }, { 20,5 }, 1));
            }
        }
    }
};

int main()
{
    InitWindow(1280, 720, "ASU!");
    InitAudioDevice();
    SetConfigFlags(FLAG_VSYNC_HINT);
    user_interface ui;
    {
        ui.layouts.push_back({});
        ui.layouts.push_back({});
        ui.layouts[1].push_back(element("resources\\image.png", { 0,0 }, { 0,0 }, 2));
        ui.layouts[1].push_back(element("ASU!", { 10,10 }, { 0,20 }, 0));
        ui.layouts[1].push_back(element(" Play", { 10,30 }, { 20,5 }, 1));
        ui.layouts[1].push_back(element(" Create", { 10,35 }, { 20,5 }, 1));
        ui.layouts[1].push_back(element(" Exit", { 10,40 }, { 20,5 }, 1));
        ui.layouts.push_back({});
    }
    ui.screenWidth = 1280; ui.screenHeight = 720;
    //====================================================================================================
    Music mus = LoadMusicStream("levels\\michael\\music.mp3");

    //====================================================================================================
    for (bool exit = 0; !WindowShouldClose() && !exit;)
    {
        ui.mouse = GetMousePosition();
        if (IsMusicReady(mus))
            UpdateMusicStream(mus);
        //====================================================================================================
        /*if (!IsWindowFullscreen())
        {
            SetWindowSize(ui.screenWidth, ui.screenHeight);
            ToggleFullscreen();
        }*/
        //====================================================================================================
        if (IsKeyPressed(KEY_A))
        {
            StopMusicStream(mus);
            PlayMusicStream(mus);
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            string action = ui._MousePress();
            if (action != "")
            {
                cout << action << endl;
                if (action == " Play")
                {
                    ui.layout = 2;
                    ui._GetLevels();
                }
                if (action == " Exit")
                {
                    exit = 1;
                }
                if (action[0] != ' ')
                {
                    if (IsMusicReady(mus))  UnloadMusicStream(mus);
                    mus = LoadMusicStream(action.c_str());
                    PlayMusicStream(mus);
                }
            }
        }
        for (; int k = GetKeyPressed();)
        {
            k -= '0';
            if (0 <= k && k < ui.layouts.size()) { ui.layout = k; }
        }
        //====================================================================================================
        BeginDrawing();
        ClearBackground(BLACK);
        ui._DrawLayout();
        DrawFPS(0, 0);
        EndDrawing();
        //====================================================================================================
    }
    CloseWindow();
    UnloadMusicStream(mus);
    return 0;
}