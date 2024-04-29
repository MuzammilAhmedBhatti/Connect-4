#include <iostream>
#include <vector>
#include <climits>

#include "raylib.h"

using namespace std;

const int EMPTY = 0;
const int AI = 1;
const int HUMAN = -1;
int count = 0;

class GameBoard {
private:
    const int ROWS = 6;
    const int COLS = 7;
    Texture empty_texture;
public:
    int* row = new int[COLS];
    Texture** Tex_arr;
    GameBoard() {
        // Load empty texture
        empty_texture = LoadTexture("empty.png");

        Tex_arr = new Texture * [ROWS];
        for (int i = 0; i < ROWS; i++) {
            Tex_arr[i] = new Texture[COLS];
        }

        for (int i = 0; i < COLS; i++) {
            row[i] = ROWS - 1;
        }

        InitializeGameBoard();
    }
    //Encapsulation
    int getRows() {
        return ROWS;
    }

    int getCols() {
        return COLS;
    }

    Texture getEmptyTexture() {
        return empty_texture;
    }

    Texture** getTexture2DArray() {
        return Tex_arr;
    }

    // Destructor to deallocate memory
    ~GameBoard() {
        for (int i = 0; i < ROWS; i++) {
            delete[] Tex_arr[i];
        }
        delete[] Tex_arr;
        delete[] row;
    }

    // Function to initialize game board with empty textures
    void InitializeGameBoard() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                Tex_arr[i][j] = empty_texture;
            }
        }
    }

    Texture yellowTex = LoadTexture("yellow.png");
    Texture redTex = LoadTexture("red.png");

    // Function to draw the game board
    void Draw() {
        float texture_width = GetScreenWidth() / static_cast<float>(COLS);//Width of each Texture
        float texture_height = GetScreenHeight() / static_cast<float>(ROWS);//Height of each Texture
        float texture_size = min(texture_width, texture_height);

        float texture_position_x = (GetScreenWidth() / 2 - ((texture_size / 2) * COLS));
        float texture_position_y = (GetScreenHeight() / 2 - ((texture_size / 2) * ROWS));

        Rectangle source_rec = { 0, 0, static_cast<float>(empty_texture.width), static_cast<float>(empty_texture.height) };

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                Rectangle dest_rec = { texture_position_x + (j * texture_size),texture_position_y + (i * texture_size) , texture_size,texture_size };
                DrawTexturePro(Tex_arr[i][j], source_rec, dest_rec, Vector2{ 0.0f, 0.0f }, 0.0f, WHITE);
            }
        }
    }
};

class twoPlayer : public GameBoard {
private:
    vector<vector<int>> grid;
    int column_num = 0;
public:
    twoPlayer() : GameBoard() {
        // Initialize grid with zeros
        grid = vector<vector<int>>(getRows(), vector<int>(getCols(), 0));
    }

    int evaluate(const std::vector<std::vector<int>>& board);
    int minimax(std::vector<std::vector<int>> board, int depth, bool maximizingPlayer, int alpha, int beta);
    int findBestMove(const std::vector<std::vector<int>>& board);
    bool isFull(const std::vector<std::vector<int>>& board);

    void click(Vector2 mousePos) {
        int column_number = -1;

        float texture_width = GetScreenWidth() / static_cast<float>(getCols());//Width of each Texture
        float texture_height = GetScreenHeight() / static_cast<float>(getRows());//Height of each Texture
        float texture_size = min(texture_width, texture_height);

        float texture_position_x = (GetScreenWidth() / 2 - ((texture_size / 2) * getCols()));

        column_number = static_cast<int>((mousePos.x - texture_position_x) / texture_size);

        if (mousePos.x < ((GetScreenWidth() / 2 - ((texture_size / 2) * getCols())))) column_number = -1;
        else if (mousePos.x > ((GetScreenWidth() / 2 + ((texture_size / 2) * getCols()))))column_number = -1;

        cout << "\ncolumn number = " << column_number << endl;

        column_num = column_number;
        turn(column_num);
    }

    void turn(int column_num) {
        if (row[column_num] == -1) { return; }

        if (count != -1) count++;

        if (column_num >= 0 && column_num < getCols()) {
            cout << "count = " << count;
        }

        if (count % 2 == 0) {
            int animation = 0;
            while (animation <= row[column_num]) {
                ClearBackground(RAYWHITE);
                Tex_arr[animation][column_num] = yellowTex;
                if (animation > 0) { Tex_arr[animation - 1][column_num] = getEmptyTexture(); }
                Draw();
                EndDrawing();
                WaitTime(0.03);
                animation++;
            }
            grid[row[column_num]][column_num] = 8;
            row[column_num]--;
        }

        else if (count % 2 != 0) {
            int animation = 0;
            while (animation <= row[column_num]) {
                ClearBackground(RAYWHITE);
                Tex_arr[animation][column_num] = redTex;
                if (animation > 0) { Tex_arr[animation - 1][column_num] = getEmptyTexture(); }
                Draw();
                EndDrawing();
                WaitTime(0.03);
                animation++;
            }
            grid[row[column_num]][column_num] = 7;
            row[column_num]--;
        }

        if (win_check(grid) == 1) {
            cout << "Player 1 wins";
        }

        else if (win_check(grid) == 2) {
            cout << "Player 2 wins";
        }
    }

    int win_check(int** grid) {
        int mark;
        if (count % 2 == 0) mark = 8;
        else mark = 7;

        bool win = false;

        //Rows
        for (int i = 0; i < getRows(); i++) {
            for (int j = 0; j < getCols() - 3; j++) {
                if (grid[i][j] == mark && grid[i][j + 1] == mark && grid[i][j + 2] == mark && grid[i][j + 3] == mark) {
                    win = true;
                }
            }
        }

        //Columns
        for (int i = 0; i < getRows() - 3; i++) {
            for (int j = 0; j < getCols(); j++) {
                if (grid[i][j] == mark && grid[i + 1][j] == mark && grid[i + 2][j] == mark && grid[i + 3][j] == mark) {
                    win = true;
                }
            }
        }

        //Diagonals
        for (int i = 0; i < getRows() - 3; i++) {
            for (int j = 0; j < getCols() - 3; j++) {
                if (grid[i][j] == mark && grid[i + 1][j + 1] == mark && grid[i + 2][j + 2] == mark && grid[i + 3][j + 3] == mark) {
                    win = true;
                }
            }
        }

        for (int i = 3; i < getRows(); i++) {
            for (int j = 0; j < getCols() - 3; j++) {
                if (grid[i][j] == mark && grid[i - 1][j + 1] == mark && grid[i - 2][j + 2] == mark && grid[i - 3][j + 3] == mark) {
                    win = true;
                }
            }
        }

        if (win) {
            if (count % 2 == 0) return 1;
            else return 2;
        }
        return -5;
    }

    ~twoPlayer() {
        for (int i = 0; i < getRows(); i++) {
            delete[]grid[i];
        }
        delete[]grid;
    }

};

int twoPlayer::evaluate(const std::vector<std::vector<int>>& board) {
    int score = 0;

    // Check rows
    for (int i = 0; i < GameBoard::getRows(); ++i) {
        for (int j = 0; j <= GameBoard::getCols() - 4; ++j) {
            int sum = 0;
            for (int k = 0; k < 4; ++k) {
                sum += board[i][j + k];
            }
            if (sum == 4) return INT_MAX; // AI wins
            else if (sum == -4) return INT_MIN; // Human wins
            score += sum;
        }
    }

    // Check columns
    for (int j = 0; j < GameBoard::getCols(); ++j) {
        for (int i = 0; i <= GameBoard::getRows() - 4; ++i) {
            int sum = 0;
            for (int k = 0; k < 4; ++k) {
                sum += board[i + k][j];
            }
            if (sum == 4) return INT_MAX; // AI wins
            else if (sum == -4) return INT_MIN; // Human wins
            score += sum;
        }
    }

    // Check diagonal (top-left to bottom-right)
    for (int i = 0; i <= GameBoard::getRows() - 4; ++i) {
        for (int j = 0; j <= GameBoard::getCols() - 4; ++j) {
            int sum = 0;
            for (int k = 0; k < 4; ++k) {
                sum += board[i + k][j + k];
            }
            if (sum == 4) return INT_MAX; // AI wins
            else if (sum == -4) return INT_MIN; // Human wins
            score += sum;
        }
    }

    // Check diagonal (bottom-left to top-right)
    for (int i = GameBoard::getRows() - 1; i >= 3; --i) {
        for (int j = 0; j <= GameBoard::getCols() - 4; ++j) {
            int sum = 0;
            for (int k = 0; k < 4; ++k) {
                sum += board[i - k][j + k];
            }
            if (sum == 4) return INT_MAX; // AI wins
            else if (sum == -4) return INT_MIN; // Human wins
            score += sum;
        }
    }

    return score;
}

bool twoPlayer::isFull(int** board) {
    // Check if the board is full
    for (int i = 0; i < GameBoard::getRows(); ++i) {
        for (int j = 0; j < GameBoard::getCols(); ++j) {
            if (board[i][j] == EMPTY) return false;
        }
    }
    return true;
}

int twoPlayer::minimax(std::vector<std::vector<int>> board, int depth, bool maximizingPlayer, int alpha, int beta) {
    if (depth == 0 || isFull(board)) {
        return evaluate(board);
    }

    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        for (int col = 0; col < GameBoard::getCols(); ++col) {
            if (board[0][col] == EMPTY) {
                for (int row = GameBoard::getRows() - 1; row >= 0; --row) {
                    if (board[row][col] == EMPTY) {
                        board[row][col] = AI;
                        int eval = minimax(board, depth - 1, false, alpha, beta);
                        board[row][col] = EMPTY;
                        maxEval = std::max(maxEval, eval);
                        alpha = std::max(alpha, eval);
                        if (beta <= alpha)
                            break; // Beta cut-off
                    }
                }
            }
        }
        return maxEval;
    }
    else {
        int minEval = INT_MAX;
        for (int col = 0; col < GameBoard::getCols(); ++col) {
            if (board[0][col] == EMPTY) {
                for (int row = GameBoard::getRows() - 1; row >= 0; --row) {
                    if (board[row][col] == EMPTY) {
                        board[row][col] = HUMAN;
                        int eval = minimax(board, depth - 1, true, alpha, beta);
                        board[row][col] = EMPTY;
                        minEval = std::min(minEval, eval);
                        beta = std::min(beta, eval);
                        if (beta <= alpha)
                            break; // Alpha cut-off
                    }
                }
            }
        }
        return minEval;
    }
}

int twoPlayer::findBestMove(const std::vector<std::vector<int>>& board) {
    // Implementation of the findBestMove function
    // Same as the findBestMove function provided in the AI code
}

int main() {

    cout << "Hello World ! " << endl;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(700, 600, "Connect 4");

    Texture2D textureLoadPage = LoadTexture("coverPage.png");
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
                int clicked = 0;
                float mouseX = GetMouseX();
                float mouseY = GetMouseY();
                Rectangle rect = { (float)(GetScreenWidth() / 2) - (GetScreenWidth() / 11), (float)GetScreenHeight() / 4, (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 10 };

                if (CheckCollisionPointRec({ mouseX, mouseY }, rect)) {

                    cout << "DOUBLE TROUBLE" << endl;

                    twoPlayer twoPlayerGame;

                    while (!WindowShouldClose()) {
                        BeginDrawing();
                        ClearBackground(RAYWHITE);

                        twoPlayerGame.Draw();
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && clicked) {
                            Vector2 mousePos = GetMousePosition();
                            twoPlayerGame.click(mousePos);
                        }
                        clicked++;
                        EndDrawing();
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
