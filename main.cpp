#include <iostream>
#include "raylib.h"

using namespace std;

class GameArray {
public:
    int number;
    Texture img;
};

class GameBoard {
private:
    int rows = 6;
    int cols = 7;
    Texture empty_texture;
    Texture** Tex_arr;

public:
    GameBoard() {
        // Load empty texture
        empty_texture = LoadTexture("empty.png");

        Tex_arr = new Texture * [rows];
        for (int i = 0; i < rows; i++) {
            Tex_arr[i] = new Texture[cols];
        }

        InitializeGameBoard();
    }

    // Destructor to deallocate memory
    ~GameBoard() {
        for (int i = 0; i < rows; i++) {
            delete[] Tex_arr[i];
        }
        delete[] Tex_arr;
    }

    // Function to initialize game board with empty textures
    void InitializeGameBoard() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Tex_arr[i][j] = empty_texture;
            }
        }
    }

    // Function to draw the game board
    void Draw() {
        float texture_width = GetScreenWidth() / static_cast<float>(cols);//Width of each Texture
        float texture_height = GetScreenHeight() / static_cast<float>(rows);//Height of each Texture

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Rectangle source_rec = { 0, 0, static_cast<float>(empty_texture.width), static_cast<float>(empty_texture.height) };
                Rectangle dest_rec = { j * texture_width, i * texture_height , texture_width,texture_height };
                DrawTexturePro(Tex_arr[i][j], source_rec, dest_rec, Vector2{ 0.0f, 0.0f }, 0.0f, WHITE);
            }
        }
    }
};


int main() {

    cout << "Hello World ! " << endl;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Connect 4");

    Texture2D textureLoadPage = LoadTexture("conect4.png");
    Texture2D textureStartPage = LoadTexture("StartPage.png");

    //For 3 seconds delay first
    float elapsedTime = 0.0f;
    const float duration = 3.0f;

    int count_ignore_unload_texture = 1;

    Color darkBrightRed = { 200, 0, 0, 255 };

    SetTargetFPS(60);


    while (!WindowShouldClose()) {

        elapsedTime += GetFrameTime();

        //Calculating ratio so that we can scale the image accordingly
        float Scale_X_Load = static_cast<float>(GetScreenWidth()) / textureLoadPage.width;
        float Scale_Y_Load = static_cast<float>(GetScreenHeight()) / textureLoadPage.height;

        float Scale_X_Start = static_cast<float>(GetScreenWidth()) / textureStartPage.width;
        float Scale_Y_Start = static_cast<float>(GetScreenHeight()) / textureStartPage.height;


        //Using Rectangle class to select the position where image needs to be fit
        //Not making object as this class does not have constructor so we use {} to directly assign values 
        Rectangle destination_Load{
            0,
            0,//top left corner (x,y)
            static_cast<float>(textureLoadPage.width) * Scale_X_Load,   // width of the rectangle
            static_cast<float>(textureLoadPage.height) * Scale_Y_Load   // height of the rectangle
        };

        Rectangle destination_Start{
            0,
            0,//top left corner (x,y)
            static_cast<float>(textureStartPage.width) * Scale_X_Start,   // width of the rectangle
            static_cast<float>(textureStartPage.height) * Scale_Y_Start   // height of the rectangle
        };

        BeginDrawing();

        if (elapsedTime < duration) {
            DrawTexturePro(textureLoadPage, { 0,0,static_cast<float>(textureLoadPage.width),static_cast<float>(textureLoadPage.height) }, // Sourse
                destination_Load, { 0,0 }, 0, WHITE); // Here to draw i.e. destination
        }

        if (elapsedTime >= duration) {
            // Unload the texture
            if (count_ignore_unload_texture == 1) {
                UnloadTexture(textureLoadPage);
            }
            count_ignore_unload_texture++;
            DrawTexturePro(textureStartPage, { 0,0,static_cast<float>(textureStartPage.width),static_cast<float>(textureStartPage.height) }, // Source
                destination_Start, { 0,0 }, 0, WHITE); // Here to draw i.e. destination
            //Single Player
            DrawRectangle((GetScreenWidth() / 2) - (GetScreenWidth() / 11), GetScreenHeight() / 8, GetScreenWidth() / 5, GetScreenHeight() / 10, darkBrightRed);
            DrawText("Single Player", (GetScreenWidth() / 2) - (GetScreenWidth() / 11) + 10, GetScreenHeight() / 8 + 10, GetScreenWidth() / 38, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                float mouseX = GetMouseX();
                float mouseY = GetMouseY();
                Rectangle rect = { (float)(GetScreenWidth() / 2) - (GetScreenWidth() / 11), (float)GetScreenHeight() / 8, (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 10 };

                if (CheckCollisionPointRec({ mouseX, mouseY }, rect)) {
                    cout << "Ayein!" << endl;
                }
            }


            //Two Player
            DrawRectangle((GetScreenWidth() / 2) - (GetScreenWidth() / 11), GetScreenHeight() / 4, GetScreenWidth() / 5, GetScreenHeight() / 10, darkBrightRed);
            DrawText("Two Player", (GetScreenWidth() / 2) - (GetScreenWidth() / 11) + 10, GetScreenHeight() / 4 + 10, GetScreenWidth() / 38, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                float mouseX = GetMouseX();
                float mouseY = GetMouseY();
                Rectangle rect = { (float)(GetScreenWidth() / 2) - (GetScreenWidth() / 11), (float)GetScreenHeight() / 4, (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 10 };

                if (CheckCollisionPointRec({ mouseX, mouseY }, rect)) {

                    cout << "DOUBLE TROUBLE" << endl;

                    GameBoard game;

                    while (!WindowShouldClose()) {
                        BeginDrawing();
                        ClearBackground(RAYWHITE);

                        game.Draw();

                        EndDrawing();
                        //hello
                    }

                }
            }


            //Instructions
            DrawRectangle((GetScreenWidth() / 2) - (GetScreenWidth() / 11), GetScreenHeight() / 2.7, GetScreenWidth() / 5, GetScreenHeight() / 10, darkBrightRed);
            DrawText("Instructions", (GetScreenWidth() / 2) - (GetScreenWidth() / 11) + 10, GetScreenHeight() / 2.7 + 10, GetScreenWidth() / 38, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                float mouseX = GetMouseX();
                float mouseY = GetMouseY();
                Rectangle rect = { (float)(GetScreenWidth() / 2) - (GetScreenWidth() / 11), (float)GetScreenHeight() / 2.7f, (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 10 };

                if (CheckCollisionPointRec({ mouseX, mouseY }, rect)) {
                    cout << "Ruk Mai Tere ko Batata hun ! " << endl;
                }
            }


            //Exit
            DrawRectangle((GetScreenWidth() / 2) - (GetScreenWidth() / 11), GetScreenHeight() / 2, GetScreenWidth() / 5, GetScreenHeight() / 10, darkBrightRed);
            DrawText("Exit", (GetScreenWidth() / 2) - (GetScreenWidth() / 11) + 10, GetScreenHeight() / 2 + 10, GetScreenWidth() / 38, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                float mouseX = GetMouseX();
                float mouseY = GetMouseY();
                Rectangle rect = { (float)(GetScreenWidth() / 2) - (GetScreenWidth() / 11), (float)GetScreenHeight() / 2, (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 10 };

                if (CheckCollisionPointRec({ mouseX, mouseY }, rect)) {
                    cout << "Tata goodbye khatam ! " << endl;
                }
            }
        }


        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

