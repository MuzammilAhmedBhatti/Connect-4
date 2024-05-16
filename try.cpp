// #include <iostream>
// #include <limits.h>
// #include "raylib.h"
// //#include<cstdlib>
// using namespace std;

// Rectangle textureStartPage_math(Texture textureLoadPage) {
//     float destinationWidth = 0, destinationHeight = 0;

//     //Calculating ratio so that we can scale the image accordingly
//     float textureAspectRatio = static_cast<float>(textureLoadPage.width) / textureLoadPage.height;

//     // Calculate destination rectangle size while maintaining aspect ratio
//     if (textureAspectRatio > 1) {
//         // Landscape orientation
//         destinationWidth = GetScreenWidth();
//         destinationHeight = destinationWidth / textureAspectRatio;
//     }
//     else {
//         // Portrait or square orientation
//         destinationHeight = GetScreenHeight();
//         destinationWidth = destinationHeight * textureAspectRatio;
//     }

//     if (destinationWidth > GetScreenWidth()) {
//         destinationWidth = GetScreenWidth();
//         destinationHeight = destinationWidth / textureAspectRatio;
//     }
//     if (destinationHeight > GetScreenHeight()) {
//         destinationHeight = GetScreenHeight();
//         destinationWidth = destinationHeight * textureAspectRatio;
//     }

//     Rectangle destination_Start{
//         (GetScreenWidth() - destinationWidth) / 2,
//         (GetScreenHeight() - destinationHeight) / 2,
//         destinationWidth,
//         destinationHeight
//     };

//     return destination_Start;
// }
// int main() {
//     cout << "HELLO WORLD ! " << endl;
//     SetConfigFlags(FLAG_WINDOW_RESIZABLE);
//     InitWindow(840, 720, "Connect 4");
//     Texture load_page = LoadTexture("My project.png");
//     SetTargetFPS(60);
//     Rectangle load = textureStartPage_math(load_page);

//     while (!WindowShouldClose()) {
//         BeginDrawing();
//         DrawTexturePro(load_page,
//             { 0, 0, static_cast<float>(load_page.width), static_cast<float>(load_page.height) },
//             load,
//             { 0,0 },
//             0,
//             WHITE); // Destination


//         if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
//             float mouseX = GetMouseX();
//             float mouseY = GetMouseY();
//             cout << "mouseX = " << mouseX << " mouseY = " << mouseY << endl;
//         }
//         EndDrawing();

//     }

// }
