#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define ROWS 6
#define COLS 7
#define CELL_SIZE 80
#define DISC_RADIUS 35
#define WINDOW_WIDTH (COLS * CELL_SIZE)
#define WINDOW_HEIGHT (ROWS * CELL_SIZE)

#ifdef main
#undef main
#endif

void initializeBoard(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = '.';
        }
    }
}

void printBoard(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    for (int j = 0; j < COLS; j++) {
        printf("%d ", j + 1);
    }
    printf("\n");
}

bool dropDisc(char board[ROWS][COLS], int col, char disc) {
    if (col < 0 || col >= COLS || board[0][col] != '.') {
        return false;
    }
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col] == '.') {
            board[i][col] = disc;
            return true;
        }
    }
    return false;
}

bool checkWin(char board[ROWS][COLS], char disc) {
    // Check horizontal
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] == disc && board[i][j + 1] == disc &&
                board[i][j + 2] == disc && board[i][j + 3] == disc) {
                return true;
            }
        }
    }

    // Check vertical
    for (int i = 0; i < ROWS - 3; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == disc && board[i + 1][j] == disc &&
                board[i + 2][j] == disc && board[i + 3][j] == disc) {
                return true;
            }
        }
    }

    // Check diagonal (bottom-left to top-right)
    for (int i = 3; i < ROWS; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] == disc && board[i - 1][j + 1] == disc &&
                board[i - 2][j + 2] == disc && board[i - 3][j + 3] == disc) {
                return true;
            }
        }
    }

    // Check diagonal (top-left to bottom-right)
    for (int i = 0; i < ROWS - 3; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] == disc && board[i + 1][j + 1] == disc &&
                board[i + 2][j + 2] == disc && board[i + 3][j + 3] == disc) {
                return true;
            }
        }
    }

    return false;
}

bool isBoardFull(char board[ROWS][COLS]) {
    for (int j = 0; j < COLS; j++) {
        if (board[0][j] == '.') {
            return false;
        }
    }
    return true;
}

// Helper to mark winning discs
void markWinningDiscs(char board[ROWS][COLS], char disc, int winMask[ROWS][COLS]) {
    // Clear mask
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            winMask[i][j] = 0;
    // Horizontal
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] == disc && board[i][j+1] == disc && board[i][j+2] == disc && board[i][j+3] == disc) {
                winMask[i][j] = winMask[i][j+1] = winMask[i][j+2] = winMask[i][j+3] = 1;
                return;
            }
        }
    }
    // Vertical
    for (int i = 0; i < ROWS - 3; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == disc && board[i+1][j] == disc && board[i+2][j] == disc && board[i+3][j] == disc) {
                winMask[i][j] = winMask[i+1][j] = winMask[i+2][j] = winMask[i+3][j] = 1;
                return;
            }
        }
    }
    // Diagonal (bottom-left to top-right)
    for (int i = 3; i < ROWS; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] == disc && board[i-1][j+1] == disc && board[i-2][j+2] == disc && board[i-3][j+3] == disc) {
                winMask[i][j] = winMask[i-1][j+1] = winMask[i-2][j+2] = winMask[i-3][j+3] = 1;
                return;
            }
        }
    }
    // Diagonal (top-left to bottom-right)
    for (int i = 0; i < ROWS - 3; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] == disc && board[i+1][j+1] == disc && board[i+2][j+2] == disc && board[i+3][j+3] == disc) {
                winMask[i][j] = winMask[i+1][j+1] = winMask[i+2][j+2] = winMask[i+3][j+3] = 1;
                return;
            }
        }
    }
}

void drawWinningLine(SDL_Renderer *renderer, int winMask[ROWS][COLS]) {
    int sx = -1, sy = -1, ex = -1, ey = -1;
    // Find the first and last winning disc
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (winMask[i][j]) {
                if (sx == -1) {
                    sx = j * CELL_SIZE + CELL_SIZE / 2 + 4;
                    sy = i * CELL_SIZE + CELL_SIZE / 2 + 4;
                }
                ex = j * CELL_SIZE + CELL_SIZE / 2 + 4;
                ey = i * CELL_SIZE + CELL_SIZE / 2 + 4;
            }
        }
    }
    if (sx != -1 && ex != -1) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green line
        // Draw a thicker line by drawing multiple lines around the main line
        for (int offset = -2; offset <= 2; offset++) {
            SDL_RenderDrawLine(renderer, sx + offset, sy + offset, ex + offset, ey + offset);
            SDL_RenderDrawLine(renderer, sx - offset, sy + offset, ex - offset, ey + offset);
        }
    }
}

void drawBoard(SDL_Renderer *renderer, char board[ROWS][COLS], char currentPlayer, int winMask[ROWS][COLS]) {
    // Set border color based on current player
    if (currentPlayer == 'X') {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red border for Player 1 (X)
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow border for Player 2 (O)
    }
    SDL_Rect border = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &border);

    // Draw background (inside border)
    SDL_Rect bg = {4, 4, WINDOW_WIDTH - 8, WINDOW_HEIGHT - 8};
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
    SDL_RenderFillRect(renderer, &bg);

    // Draw cells and discs
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            SDL_Rect cell = {j * CELL_SIZE + 4, i * CELL_SIZE + 4, CELL_SIZE, CELL_SIZE};
            SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255);
            SDL_RenderFillRect(renderer, &cell);
            // Draw disc
            if (board[i][j] != '.') {
                int cx = j * CELL_SIZE + CELL_SIZE / 2 + 4;
                int cy = i * CELL_SIZE + CELL_SIZE / 2 + 4;
                if (winMask && winMask[i][j]) {
                    // Draw normal disc, then overlay line later
                    if (board[i][j] == 'X')
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    else
                        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                } else if (board[i][j] == 'X') {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
                } else {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
                }
                for (int w = 0; w < DISC_RADIUS; w++) {
                    for (int h = 0; h < DISC_RADIUS; h++) {
                        int dx = w - DISC_RADIUS/2;
                        int dy = h - DISC_RADIUS/2;
                        if (dx*dx + dy*dy <= (DISC_RADIUS/2)*(DISC_RADIUS/2)) {
                            SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
                        }
                    }
                }
            } else {
                // Draw empty circle
                int cx = j * CELL_SIZE + CELL_SIZE / 2 + 4;
                int cy = i * CELL_SIZE + CELL_SIZE / 2 + 4;
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                for (int w = 0; w < DISC_RADIUS; w++) {
                    for (int h = 0; h < DISC_RADIUS; h++) {
                        int dx = w - DISC_RADIUS/2;
                        int dy = h - DISC_RADIUS/2;
                        if (dx*dx + dy*dy <= (DISC_RADIUS/2)*(DISC_RADIUS/2)) {
                            SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
                        }
                    }
                }
            }
        }
    }
    // Draw winning line if winMask is set
    if (winMask) drawWinningLine(renderer, winMask);
    SDL_RenderPresent(renderer);
}

int main() {
    char board[ROWS][COLS];
    int winMask[ROWS][COLS] = {0};
    initializeBoard(board);
    char player1 = 'X';
    char player2 = 'O';
    char currentPlayer = player1;
    bool running = true;
    bool gameOver = false;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        system("pause");
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Connect4", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        system("pause");
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        system("pause");
        return 1;
    }
    drawBoard(renderer, board, currentPlayer, winMask);
    int animating = 0;
    int animCol = -1, animRow = -1;
    char animDisc = '.';
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && !gameOver && !animating) {
                int x = event.button.x;
                int col = x / CELL_SIZE;
                if (col >= 0 && col < COLS && board[0][col] == '.') {
                    int row = -1;
                    for (int i = ROWS - 1; i >= 0; i--) {
                        if (board[i][col] == '.') {
                            row = i;
                            break;
                        }
                    }
                    if (row != -1) {
                        animating = 1;
                        animCol = col;
                        animRow = row;
                        animDisc = currentPlayer;
                    }
                }
            } else if (event.type == SDL_KEYDOWN && gameOver) {
                if (event.key.keysym.sym == SDLK_r) {
                    initializeBoard(board);
                    for (int i = 0; i < ROWS; i++) for (int j = 0; j < COLS; j++) winMask[i][j] = 0;
                    currentPlayer = player1;
                    drawBoard(renderer, board, currentPlayer, winMask);
                    gameOver = false;
                }
            }
        }
        // Animate disc drop if needed
        if (animating) {
            int startY = CELL_SIZE / 2;
            int endY = animRow * CELL_SIZE + CELL_SIZE / 2;
            int y = startY;
            int speed = 20;
            while (y < endY) {
                drawBoard(renderer, board, currentPlayer, winMask);
                // Draw animated disc
                int cx = animCol * CELL_SIZE + CELL_SIZE / 2 + 4;
                int cy = y + 4;
                if (animDisc == 'X') SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                else SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                for (int w = 0; w < DISC_RADIUS; w++) {
                    for (int h = 0; h < DISC_RADIUS; h++) {
                        int dx = w - DISC_RADIUS/2;
                        int dy = h - DISC_RADIUS/2;
                        if (dx*dx + dy*dy <= (DISC_RADIUS/2)*(DISC_RADIUS/2)) {
                            SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
                        }
                    }
                }
                SDL_RenderPresent(renderer);
                SDL_Delay(10);
                y += speed;
                if (y > endY) y = endY;
            }
            // Place the disc
            board[animRow][animCol] = animDisc;
            // Switch player BEFORE drawing the board, so the border color shows the next player's turn
            if (!checkWin(board, animDisc) && !isBoardFull(board)) {
                currentPlayer = (currentPlayer == player1) ? player2 : player1;
            }
            // Highlight winning discs if win
            if (checkWin(board, animDisc)) {
                markWinningDiscs(board, animDisc, winMask);
                drawBoard(renderer, board, currentPlayer, winMask);
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", animDisc == player1 ? "Player 1 (Red) wins!" : "Player 2 (Yellow) wins!", window);
                gameOver = true;
            } else if (isBoardFull(board)) {
                drawBoard(renderer, board, currentPlayer, winMask);
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "It's a draw!", window);
                gameOver = true;
            } else {
                drawBoard(renderer, board, currentPlayer, winMask);
            }
            animating = 0;
            animCol = -1;
            animRow = -1;
            animDisc = '.';
        }
        SDL_Delay(10);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
