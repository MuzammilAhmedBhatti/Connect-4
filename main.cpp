#include <iostream>
#include <limits.h>
#include "raylib.h"
#include<cstdlib>
#include <crtdbg.h>

using namespace std;
bool should_win = false;
int count = 0;
int difficulty = 0;

class GameEnd {};

Rectangle textureStartPage_math(Texture textureLoadPage) {
    float destinationWidth = 0, destinationHeight = 0;

    //Calculating ratio so that we can scale the image accordingly
    float textureAspectRatio = static_cast<float>(textureLoadPage.width) / textureLoadPage.height;

    // Calculate destination rectangle size while maintaining aspect ratio
    if (textureAspectRatio > 1) {
        // Landscape orientation
        destinationWidth = GetScreenWidth();
        destinationHeight = destinationWidth / textureAspectRatio;
    }
    else {
        // Portrait or square orientation
        destinationHeight = GetScreenHeight();
        destinationWidth = destinationHeight * textureAspectRatio;
    }

    if (destinationWidth > GetScreenWidth()) {
        destinationWidth = GetScreenWidth();
        destinationHeight = destinationWidth / textureAspectRatio;
    }
    if (destinationHeight > GetScreenHeight()) {
        destinationHeight = GetScreenHeight();
        destinationWidth = destinationHeight * textureAspectRatio;
    }

    Rectangle destination_Start{
        (GetScreenWidth() - destinationWidth) / 2,
        (GetScreenHeight() - destinationHeight) / 2,
        destinationWidth,
        destinationHeight
    };

    return destination_Start;
}

void select_level(Texture textureStartPage, Rectangle destination_Start, int& clicked) {
    clicked = 0;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        destination_Start = textureStartPage_math(textureStartPage);
        Color darkBrightRed = { 200, 0, 0, 255 };
        DrawTexturePro(textureStartPage, { 0,0,static_cast<float>(textureStartPage.width),static_cast<float>(textureStartPage.height) }, // Source
            destination_Start, { 0,0 }, 0, WHITE); // Here to draw i.e. destination

        DrawRectangle((GetScreenWidth() / 2) - (GetScreenWidth() / 11), GetScreenHeight() / 8, GetScreenWidth() / 5, GetScreenHeight() / 10, darkBrightRed);
        DrawText("Easy", (GetScreenWidth() / 2) - (GetScreenWidth() / 11) + 10, GetScreenHeight() / 8 + 10, GetScreenWidth() / 38, WHITE);
        Rectangle rect_easy = { (float)(GetScreenWidth() / 2) - (GetScreenWidth() / 11), (float)GetScreenHeight() / 8, (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 10 };

        DrawRectangle((GetScreenWidth() / 2) - (GetScreenWidth() / 11), GetScreenHeight() / 4, GetScreenWidth() / 5, GetScreenHeight() / 10, darkBrightRed);
        DrawText("Medium", (GetScreenWidth() / 2) - (GetScreenWidth() / 11) + 10, GetScreenHeight() / 4 + 10, GetScreenWidth() / 38, WHITE);
        Rectangle rect_medium = { (float)(GetScreenWidth() / 2) - (GetScreenWidth() / 11), (float)GetScreenHeight() / 4, (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 10 };

        DrawRectangle((GetScreenWidth() / 2) - (GetScreenWidth() / 11), GetScreenHeight() / 2.7, GetScreenWidth() / 5, GetScreenHeight() / 10, darkBrightRed);
        DrawText("Hard", (GetScreenWidth() / 2) - (GetScreenWidth() / 11) + 10, GetScreenHeight() / 2.7 + 10, GetScreenWidth() / 38, WHITE);
        Rectangle rect_hard = { (float)(GetScreenWidth() / 2) - (GetScreenWidth() / 11), (float)GetScreenHeight() / 2.7f, (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 10 };

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (clicked) {
                float mouseX = GetMouseX();
                float mouseY = GetMouseY();
                if (CheckCollisionPointRec({ mouseX, mouseY }, rect_easy)) difficulty = 1;
                else if (CheckCollisionPointRec({ mouseX, mouseY }, rect_medium)) difficulty = 3;
                else if (CheckCollisionPointRec({ mouseX, mouseY }, rect_hard)) difficulty = 5;
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) clicked++;
        if (difficulty) return;
        EndDrawing();
    }
}

class artificialIntelligence {
public:
    void makeMove(int**& grid, int col_num, int mark, int rows, int cols) {
        for (int i = rows - 1; i >= 0; i--) {
            if (grid[i][col_num] == 0) {
                grid[i][col_num] = mark;
                break;
            }
        }
    }
    int** copyBoard(int** grid, int rows, int cols) {
        int** copied_board = new int* [rows];
        for (int i = 0; i < rows; i++) {
            copied_board[i] = new int[cols];
            for (int j = 0; j < cols; j++) {
                copied_board[i][j] = grid[i][j];
            }
        }
        return copied_board;
    }
    bool winningMove(int**& grid, int mark, int rows, int cols) {
        int winPattern = 0; // to count adjacent pieces
        // for horizontal checks
        for (int column = 0; column < cols - 3; column++) { // for each column
            for (int row = 0; row < rows; row++) { // each row
                for (int i = 0; i < 4; i++) { // recall you need 4 to win
                    if (grid[row][column + i] == mark) { // if not all pieces match
                        winPattern++; // add sequence count
                    }
                    if (winPattern == 4) {
                        return true;
                    } // if 4 in row
                }
                winPattern = 0; // reset counter
            }
        }
        // vertical checks
        for (int column = 0; column < cols; column++) {
            for (int row = rows - 1; row >= 3; row--) {
                for (int i = 0; i < 4; i++) {
                    if (grid[row - i][column] == mark) {
                        winPattern++;
                    }
                    if (winPattern == 4) { return true; }
                }
                winPattern = 0;
            }
        }
        // the below two are diagonal checks
        for (int column = 0; column < cols - 3; column++) {
            for (int row = rows - 1; row >= 3; row--) {
                for (int i = 0; i < 4; i++) {
                    if (grid[row - i][column + i] == mark) {
                        winPattern++;
                    }
                    if (winPattern == 4) { return true; }
                }
                winPattern = 0;
            }
        }
        for (int column = cols - 1; column >= 3; column--) {
            for (int row = rows - 1; row >= 3; row--) {
                for (int i = 0; i < 4; i++) {
                    if (grid[row - i][column - i] == mark) {
                        winPattern++;
                    }
                    if (winPattern == 4) { return true; }
                }
                winPattern = 0;
            }
        }
        return false; // otherwise no winning move
    }
    int heurFunction(int good, int bad, int empty) {
        int score = 0;
        if (good == 4) { score += 500001; } // preference to go for winning move vs. block
        else if (good == 3 && empty == 1) { score += 5000 + should_win; }
        else if (good == 2 && empty == 2) { score += 500 + should_win; }
        else if (bad == 2 && empty == 2) { score -= 500 + !should_win; } // preference to block
        else if (bad == 3 && empty == 1) { score -= 5000 + !should_win; } // preference to block
        else if (bad == 4) { score -= 500000; }
        return score;
    }
    int scoreSet(int* set, int mark) {
        int good = 0; // points in favor of mark
        int bad = 0; // points against mark
        int empty = 0; // neutral spots
        for (int i = 0; i < 4; i++) { // just enumerate how many of each
            if (set[i] == mark) {
                good += 1;
            }
            else {
                good += 0;
            }

            if (set[i] == 1 || set[i] == 2) {
                bad += 1;
            }
            else {
                bad += 0;
            }
            if (set[i] == 0) {
                empty += 1;
            }
            else {
                empty += 0;
            }
        }
        // bad was calculated as (bad + good), so remove good
        bad -= good;
        return heurFunction(good, bad, empty);
    }
    int tabScore(int** grid, int mark, int rows, int cols) {
        int row_arr[rows], column_arr[cols], set[4], score = 0;

        //Horizontal
        for (int row = 0; row < rows; row++) {
            for (int column = 0; column < cols; column++) {
                row_arr[column] = grid[row][column]; // this is a distinct row alone
            }
            for (int column = 0; column < cols - 3; column++) {
                for (int i = 0; i < 4; i++) {
                    set[i] = row_arr[column + i]; // for each possible "set" of 4 spots from that row
                }
                score += scoreSet(set, mark); // find score
            }
        }
        // vertical
        for (int column = 0; column < cols; column++) {
            for (int row = 0; row < rows; row++) {
                column_arr[row] = grid[row][column];
            }
            for (int row = rows - 1; row >= 3; row--) {
                for (int i = 0; i < 4; i++) {
                    set[i] = column_arr[row - i];
                }
                score += scoreSet(set, mark);
            }
        }
        // diagonals
        for (int row = rows - 1; row >= 3; row--) {
            for (int column = 0; column < cols; column++) {
                row_arr[column] = grid[row][column];
            }
            for (int column = 0; column < cols - 3; column++) {
                for (int i = 0; i < 4; i++) {
                    set[i] = grid[row - i][column + i];
                }
                score += scoreSet(set, mark);
            }
        }
        for (int row = rows - 1; row >= 3; row--) {
            for (int column = 0; column < cols; column++) {
                row_arr[column] = grid[row][column];
            }
            for (int column = cols - 1; column >= 3; column--) {
                for (int i = 0; i < 4; i++) {
                    set[i] = grid[row - i][column - i];
                }
                score += scoreSet(set, mark);
            }
        }
        return score;
    }
    int* miniMax(int**& grid, int depth, int alf, int bet, int mark, int rows, int cols) {
        if (depth == 0 || depth >= (rows * cols) - count) {
            int* result = new int[2];
            result[0] = tabScore(grid, 2, rows, cols);
            result[1] = -1;
            return result;
        }

        if (mark == 2) {
            int* movesDoneTillNow = new int[2];
            movesDoneTillNow[0] = INT_MIN;
            movesDoneTillNow[1] = -1;
            if (winningMove(grid, 1, rows, cols)) {
                return movesDoneTillNow;
            }
            for (int column = 0; column < cols; column++) { // for each possible move
                if (grid[0][column] == 0) { // but only if that column is non-full
                    int** updatedBoard = copyBoard(grid, rows, cols); // make a copy of the board
                    makeMove(updatedBoard, column, mark, rows, cols); // try the move
                    int* score = miniMax(updatedBoard, depth - 1, alf, bet, 1, rows, cols); // find move based on that new board state
                    for (int i = 0; i < rows; i++) {
                        delete[] updatedBoard[i];
                    }
                    delete[] updatedBoard;
                    if (score[0] > movesDoneTillNow[0]) { // if better score, replace it, and consider that best move (for now)
                        movesDoneTillNow[0] = score[0];
                        movesDoneTillNow[1] = column;
                    }
                    delete[] score;
                    alf = max(alf, movesDoneTillNow[0]);
                    if (alf >= bet) {
                        break;
                    } // for pruning
                }
            }
            return movesDoneTillNow;
        }
        else {
            int* movesDoneTillNow = new int[2];
            movesDoneTillNow[0] = INT_MAX;
            movesDoneTillNow[1] = -1;
            if (winningMove(grid, 2, rows, cols)) {
                return movesDoneTillNow;
            }
            for (int column = 0; column < cols; column++) { // for each possible move
                if (grid[0][column] == 0) { // but only if that column is non-full
                    int** updatedBoard = copyBoard(grid, rows, cols); // make a copy of the board
                    makeMove(updatedBoard, column, mark, rows, cols); // try the move
                    int* score = miniMax(updatedBoard, depth
                        - 1, alf, bet, 2, rows, cols); // find move based on that new board state
                    for (int i = 0; i < rows; i++) {
                        delete[] updatedBoard[i];
                    }
                    delete[] updatedBoard;
                    if (score[0] < movesDoneTillNow[0]) { // if better score, replace it, and consider that best move (for now)
                        movesDoneTillNow[0] = score[0];
                        movesDoneTillNow[1] = column;
                    }
                    delete[] score;
                    bet = min(bet, movesDoneTillNow[0]);
                    if (alf >= bet) {
                        break;
                    } // for pruning
                }
            }
            return movesDoneTillNow;
        }
    }
    int aiMove(int**& grid, int& depth, int rows, int cols, int player) {
        //cout << "ai move chal raha" << endl;
        int* temp = miniMax(grid, depth, 0 - INT_MAX, INT_MAX, player, rows, cols);
        int temp2 = temp[1];
        delete[] temp;
        return temp2;
    }
};

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
    Texture getRedMagnified() {
        return redTex_magnified;
    }
    Texture** getTexture2DArray() {
        return Tex_arr;
    }


    float GetGridCenterX(int col)
    {
        float texture_width = GetScreenWidth() / static_cast<float>(COLS);//Width of each Texture
        float texture_height = GetScreenHeight() / static_cast<float>(ROWS);//Height of each Texture
        float texture_size = min(texture_width, texture_height);

        float texture_position_x = (GetScreenWidth() / 2 - ((texture_size / 2) * COLS));

        return texture_position_x + (texture_size * (2 * col + 1) / 2);
    }

    float GetGridCenterY(int row)
    {
        float texture_width = GetScreenWidth() / static_cast<float>(COLS);//Width of each Texture
        float texture_height = GetScreenHeight() / static_cast<float>(ROWS);//Height of each Texture
        float texture_size = min(texture_width, texture_height);

        float texture_position_y = (GetScreenHeight() / 2 - ((texture_size / 2) * ROWS));

        return texture_position_y + (texture_size * (2 * row + 1) / 2);
    }

    // Destructor to deallocate memory
    ~GameBoard() {
        for (int i = 0; i < ROWS; i++) {
            delete[] Tex_arr[i];
        }
        delete[] Tex_arr;
        delete[]row;
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
    Texture redTex_magnified = LoadTexture("red_magnified.png");
    Texture yellowTex_magnified = LoadTexture("yellow_magnified.png");

    // Function to draw the game board (Polymorphism Applied)
    virtual void Draw() {
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
    int** grid;
    int yellowTex_number = 2;
    int redTex_number = 1;
    int column_num = 0;
    artificialIntelligence Ai;
public:
    twoPlayer() {
        grid = new int* [getRows()];
        for (int i = 0; i < getRows(); i++) {
            grid[i] = new int[getCols()];
        }
        for (int i = 0; i < getRows(); i++) {
            for (int j = 0; j < getCols(); j++) {
                grid[i][j] = 0;
            }
        }
    }
    bool AI = false;
    twoPlayer(bool AI) {
        grid = new int* [getRows()];
        for (int i = 0; i < getRows(); i++) {
            grid[i] = new int[getCols()];
        }
        for (int i = 0; i < getRows(); i++) {
            for (int j = 0; j < getCols(); j++) {
                grid[i][j] = 0;
            }
        }
        this->AI = AI;
    }

    void click(Vector2 mousePos) {
        int column_number = -1;

        float texture_width = GetScreenWidth() / static_cast<float>(getCols());//Width of each Texture
        float texture_height = GetScreenHeight() / static_cast<float>(getRows());//Height of each Texture
        float texture_size = min(texture_width, texture_height);

        float texture_position_x = (GetScreenWidth() / 2 - ((texture_size / 2) * getCols()));

        column_number = static_cast<int>((mousePos.x - texture_position_x) / texture_size);

        if (mousePos.x < ((GetScreenWidth() / 2 - ((texture_size / 2) * getCols())))) column_number = -1;
        else if (mousePos.x > ((GetScreenWidth() / 2 + ((texture_size / 2) * getCols()))))column_number = -1;

        column_num = column_number;
        turn(column_num);
    }

    void turn(int column_num) {
        if (row[column_num] == -1) { return; }
        if (column_num >= 0 && column_num < getCols()) {
            count++;
            cout << "count = " << count << endl;
        }

        if (count % 2 != 0) {
            int animation = 0;
            // //Checking AI vs AI (only for checking purposes)
            // column_num = aiMove(grid, difficulty, getRows(), getCols(), 2);
            // while (column_num == -1) {
            //     difficulty -= 1;
            //     if (difficulty != 0)column_num = aiMove(grid, difficulty, getRows(), getCols(), 2);
            //     else {
            //         for (int i = getRows() - 1;i >= 0; i--) {
            //             bool flag = false;
            //             for (int j = 0; j < getCols(); j++) {
            //                 if (grid[i][j] == 0) {
            //                     flag = true;
            //                     column_num = j;
            //                     break;
            //                 }
            //             }
            //             if (flag)break;
            //         }
            //     }
            // }
            while (animation <= row[column_num]) {
                ClearBackground(RAYWHITE);
                Tex_arr[animation][column_num] = yellowTex;
                if (animation > 0) { Tex_arr[animation - 1][column_num] = getEmptyTexture(); }
                WaitTime(0.05);
                Draw();
                EndDrawing();
                animation++;
            }
            grid[row[column_num]][column_num] = 1;
            row[column_num]--;
            cout << "Idher pohanchaa count =" << count << endl;//but not reaching here on my click
        }

        else if (count % 2 == 0) {
            int animation = 0;
            while (animation <= row[column_num]) {
                ClearBackground(RAYWHITE);
                Tex_arr[animation][column_num] = redTex;
                if (animation > 0) { Tex_arr[animation - 1][column_num] = getEmptyTexture(); }
                WaitTime(0.05);
                Draw();
                EndDrawing();
                animation++;
            }
            grid[row[column_num]][column_num] = 2;
            row[column_num]--;

        }
        if (count == getRows() * getCols()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            //Iss jaga pe draw image design kar ke daalni hain
            EndDrawing();
            WaitTime(5.0);
            throw GameEnd();
        }
        int win_row = 0, win_col = 0;
        bool right_diagnol = false, left_diagnol = false, vertical = false, horizontal = false;
        int mark = -5;
        if (win_check(grid, win_row, win_col, right_diagnol, left_diagnol, vertical, horizontal, mark) != -5) {
            if (win_check(grid, win_row, win_col, right_diagnol, left_diagnol, vertical, horizontal, mark) == 1) {
                cout << "Player 1 wins\n";
            }
            WaitTime(0.5);
            if (horizontal) {
                cout << "win_col = " << win_col << " win_row = " << win_row << endl;
                for (int i = win_col;i < win_col + 4; i++) {
                    WaitTime(0.05);
                    if (mark == 1)Tex_arr[win_row][i] = yellowTex_magnified;
                    else Tex_arr[win_row][i] = redTex_magnified;
                    Draw();
                    EndDrawing();
                }
            }
            else if (vertical) {
                cout << "win_col = " << win_col << " win_row = " << win_row << endl;
                for (int i = win_row;i < win_row + 4; i++) {
                    WaitTime(0.05);
                    if (mark == 1)Tex_arr[i][win_col] = yellowTex_magnified;
                    else Tex_arr[i][win_col] = redTex_magnified;
                    Draw();
                    EndDrawing();
                }
            }
            else if (right_diagnol) {
                cout << "win_col = " << win_col << " win_row = " << win_row << endl;
                for (int i = 0;i < 4; i++) {
                    WaitTime(0.05);
                    if (mark == 1)Tex_arr[win_row + i][win_col + i] = yellowTex_magnified;
                    else Tex_arr[win_row + i][win_col + i] = redTex_magnified;
                    Draw();
                    EndDrawing();
                }
            }
            else if (left_diagnol) {
                cout << "win_col = " << win_col << " win_row = " << win_row << endl;
                for (int i = 0;i < 4; i++) {
                    WaitTime(0.05);
                    if (mark == 1)Tex_arr[win_row - i][win_col + i] = yellowTex_magnified;
                    else Tex_arr[win_row - i][win_col + i] = redTex_magnified;
                    Draw();
                    EndDrawing();
                }
            }
            Draw();
            EndDrawing();
            WaitTime(2.0);
            BeginDrawing();
            ClearBackground(RAYWHITE);
            //Iss jaga pe if statements ke saath ending images design kar ke daalni hain
            EndDrawing();
            WaitTime(5.0);
            throw GameEnd();
        }

        if (AI && count % 2 != 0) {
            int aiCol = Ai.aiMove(grid, difficulty, getRows(), getCols(), 2);
            cout << "Column number from AI " << aiCol << endl;
            while (aiCol == -1) {
                difficulty -= 1;
                if (difficulty != 0) {
                    aiCol = Ai.aiMove(grid, difficulty, getRows(), getCols(), 2);
                    cout << "Column number from AI " << aiCol << endl;
                }
                else {
                    for (int i = getRows() - 1;i >= 0; i--) {
                        bool flag = false;
                        for (int j = 0; j < getCols(); j++) {
                            if (grid[i][j] == 0) {
                                flag = true;
                                aiCol = j;
                                break;
                            }
                        }
                        if (flag)break;
                    }
                }
            }
            turn(aiCol);
            for (int i = 0;i < getRows(); i++) {
                for (int j = 0; j < getCols(); j++) {
                    cout << grid[i][j];
                }
                cout << endl;
            }
        }

    }

    int win_check(int**& grid, int& win_row, int& win_col, bool& right_diagnol, bool& left_diagnol, bool& vertical, bool& horizontal, int& mark) {
        if (count % 2 == 0) mark = 2;
        else mark = 1;


        //Rows
        for (int i = 0; i < getRows(); i++) {
            bool flag = false;
            for (int j = 0; j < getCols() - 3; j++) {
                if (grid[i][j] == mark && grid[i][j + 1] == mark && grid[i][j + 2] == mark && grid[i][j + 3] == mark) {
                    flag = true;
                    win_row = i;
                    win_col = j;
                    horizontal = true;
                    cout << "win row = " << win_row << " win col = " << win_col << endl;
                    return mark;
                }
            }
            if (flag)break;
        }

        //Columns
        for (int i = 0; i < getRows() - 3; i++) {
            for (int j = 0; j < getCols(); j++) {
                if (grid[i][j] == mark && grid[i + 1][j] == mark && grid[i + 2][j] == mark && grid[i + 3][j] == mark) {
                    win_row = i;
                    win_col = j;
                    vertical = true;
                    return mark;
                }
            }
        }

        //Diagonals
        for (int i = 0; i < getRows() - 3; i++) {
            for (int j = 0; j < getCols() - 3; j++) {
                if (grid[i][j] == mark && grid[i + 1][j + 1] == mark && grid[i + 2][j + 2] == mark && grid[i + 3][j + 3] == mark) {
                    win_row = i;
                    win_col = j;
                    right_diagnol = true;
                    return mark;
                }
            }
        }

        for (int i = 3; i < getRows(); i++) {
            for (int j = 0; j < getCols() - 3; j++) {
                if (grid[i][j] == mark && grid[i - 1][j + 1] == mark && grid[i - 2][j + 2] == mark && grid[i - 3][j + 3] == mark) {
                    win_row = i;
                    win_col = j;
                    left_diagnol = true;
                    return mark;
                }
            }
        }
        return -5;
    }

    ~twoPlayer() {
        for (int i = 0; i < getRows(); i++) {
            delete[]grid[i];
        }
        delete[]grid;
    }

    void Draw() override
    {
        GameBoard::Draw();
        if (AI) {
            int COLS = getCols();
            int ROWS = getRows();
            float texture_width = GetScreenWidth() / static_cast<float>(COLS);//Width of each Texture
            float texture_height = GetScreenHeight() / static_cast<float>(ROWS);//Height of each Texture
            float texture_size = min(texture_width, texture_height);

            int font_size = texture_size * 0.6;

            if (COLS >= 6) {
                DrawText("A", GetGridCenterX(0) - font_size * 0.3125, GetGridCenterY(0) - font_size / 2, font_size, WHITE);
                DrawText("I", GetGridCenterX(1) - font_size / 8, GetGridCenterY(0) - font_size / 2, font_size, WHITE);

                DrawText("G", GetGridCenterX(COLS - 4) - font_size * 0.3125, GetGridCenterY(0) - font_size / 2, font_size, WHITE);
                DrawText("A", GetGridCenterX(COLS - 3) - font_size * 0.3125, GetGridCenterY(0) - font_size / 2, font_size, WHITE);
                DrawText("M", GetGridCenterX(COLS - 2) - font_size * 0.3125, GetGridCenterY(0) - font_size / 2, font_size, WHITE);
                DrawText("E", GetGridCenterX(COLS - 1) - font_size * 0.3125, GetGridCenterY(0) - font_size / 2, font_size, WHITE);
            }
            else if (COLS >= 2) {
                DrawText("A", GetGridCenterX(COLS / 2 - 1) - font_size * 0.3125, GetGridCenterY(0) - font_size / 2, font_size, WHITE);
                DrawText("I", GetGridCenterX(COLS / 2) - font_size / 8, GetGridCenterY(0) - font_size / 2, font_size, WHITE);
            }
            else {
                DrawText("AI", GetGridCenterX(0) - font_size * 0.21875, GetGridCenterY(0) - font_size / 4, font_size / 2, WHITE);
            }
        }
    }
};

int main() {
    cout << "Hello World ! " << endl;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(840, 720, "Connect 4");

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

        Rectangle destination_Load = textureStartPage_math(textureLoadPage);
        Rectangle destination_Start = textureStartPage_math(textureStartPage);

        BeginDrawing();
        if (elapsedTime < duration) {

            DrawTexturePro(textureLoadPage,
                { 0, 0, static_cast<float>(textureLoadPage.width), static_cast<float>(textureLoadPage.height) },
                destination_Load,
                { 0,0 },
                0,
                WHITE); // Destination            
        }

        if (elapsedTime >= duration) {
            // Unload the texture
            if (count_ignore_unload_texture == 1) {
                UnloadTexture(textureLoadPage);
            }

            count_ignore_unload_texture++;

            DrawTexturePro(textureStartPage, { 0,0,static_cast<float>(textureStartPage.width),static_cast<float>(textureStartPage.height) },
                destination_Start,
                { 0,0 },
                0,
                WHITE); // Here to draw i.e. destination

            //Single Player
            DrawRectangle((GetScreenWidth() / 2) - (GetScreenWidth() / 11), GetScreenHeight() / 8, GetScreenWidth() / 5, GetScreenHeight() / 10, darkBrightRed);
            DrawText("Single Player", (GetScreenWidth() / 2) - (GetScreenWidth() / 11) + 10, GetScreenHeight() / 8 + 10, GetScreenWidth() / 38, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                int clicked = 0;
                float mouseX = GetMouseX();
                float mouseY = GetMouseY();
                Rectangle rect = { (float)(GetScreenWidth() / 2) - (GetScreenWidth() / 11), (float)GetScreenHeight() / 8, (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 10 };

                if (CheckCollisionPointRec({ mouseX, mouseY }, rect)) {
                    count = 0, difficulty = 0;
                    cout << "Artificial Intelligence" << endl;
                    select_level(textureStartPage, destination_Start, clicked);

                    twoPlayer twoPlayerGame(true);
                    try {
                        while (!WindowShouldClose()) {
                            BeginDrawing();
                            ClearBackground(RAYWHITE);

                            twoPlayerGame.Draw();
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (clicked >= 2) && count % 2 == 0) {
                                cout << "in main count = " << count << endl;
                                Vector2 mousePos = GetMousePosition();
                                twoPlayerGame.click(mousePos);
                            }
                            clicked++;
                            EndDrawing();
                        }

                    }
                    catch (GameEnd&)
                    {
                        cout << "Game Ended" << endl;
                    }
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
                    count = 0;
                    cout << "DONO KHELO :)" << endl;

                    twoPlayer twoPlayerGame;

                    try {
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
                    catch (GameEnd&)
                    {
                        cout << "Game Ended" << endl;
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
                    break;
                }
            }
        }
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    cout << "Memory Leaks: " << _CrtDumpMemoryLeaks() << endl;
    return 0;
}