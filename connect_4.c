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

void drawBoard(SDL_Renderer *renderer, char board[ROWS][COLS]) {
    // Draw background
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
    SDL_RenderClear(renderer);
    // Draw cells and discs
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            // Draw cell
            SDL_Rect cell = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255);
            SDL_RenderFillRect(renderer, &cell);
            // Draw disc
            if (board[i][j] != '.') {
                int cx = j * CELL_SIZE + CELL_SIZE / 2;
                int cy = i * CELL_SIZE + CELL_SIZE / 2;
                if (board[i][j] == 'X')
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
                else
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
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
                int cx = j * CELL_SIZE + CELL_SIZE / 2;
                int cy = i * CELL_SIZE + CELL_SIZE / 2;
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
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    char board[ROWS][COLS];
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
    drawBoard(renderer, board);
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && !gameOver) {
                int x = event.button.x;
                int col = x / CELL_SIZE;
                if (dropDisc(board, col, currentPlayer)) {
                    drawBoard(renderer, board);
                    if (checkWin(board, currentPlayer)) {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", currentPlayer == player1 ? "Player 1 (Red) wins!" : "Player 2 (Yellow) wins!", window);
                        gameOver = true;
                    } else if (isBoardFull(board)) {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "It's a draw!", window);
                        gameOver = true;
                    } else {
                        currentPlayer = (currentPlayer == player1) ? player2 : player1;
                    }
                }
            } else if (event.type == SDL_KEYDOWN && gameOver) {
                if (event.key.keysym.sym == SDLK_r) {
                    initializeBoard(board);
                    drawBoard(renderer, board);
                    currentPlayer = player1;
                    gameOver = false;
                }
            }
        }
        SDL_Delay(10);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}












// #include <stdio.h>
// #include <stdbool.h>
// 
// #define ROWS 6
// #define COLS 7
// 
// void initializeBoard(char board[ROWS][COLS]) {
//     for (int i = 0; i < ROWS; i++) {
//         for (int j = 0; j < COLS; j++) {
//             board[i][j] = '.';
//         }
//     }
// }
// 
// void printBoard(char board[ROWS][COLS]) {
//     for (int i = 0; i < ROWS; i++) {
//         for (int j = 0; j < COLS; j++) {
//             printf("%c ", board[i][j]);
//         }
//         printf("\n");
//     }
//     for (int j = 0; j < COLS; j++) {
//         printf("%d ", j + 1);
//     }
//     printf("\n");
// }
// 
// bool dropDisc(char board[ROWS][COLS], int col, char disc) {
//     if (col < 0 || col >= COLS || board[0][col] != '.') {
//         return false;
//     }
//     for (int i = ROWS - 1; i >= 0; i--) {
//         if (board[i][col] == '.') {
//             board[i][col] = disc;
//             return true;
//         }
//     }
//     return false;
// }
// 
// bool checkWin(char board[ROWS][COLS], char disc) {
//     // Check horizontal
//     for (int i = 0; i < ROWS; i++) {
//         for (int j = 0; j < COLS - 3; j++) {
//             if (board[i][j] == disc && board[i][j + 1] == disc &&
//                 board[i][j + 2] == disc && board[i][j + 3] == disc) {
//                 return true;
//             }
//         }
//     }
// 
//     // Check vertical
//     for (int i = 0; i < ROWS - 3; i++) {
//         for (int j = 0; j < COLS; j++) {
//             if (board[i][j] == disc && board[i + 1][j] == disc &&
//                 board[i + 2][j] == disc && board[i + 3][j] == disc) {
//                 return true;
//             }
//         }
//     }
// 
//     // Check diagonal (bottom-left to top-right)
//     for (int i = 3; i < ROWS; i++) {
//         for (int j = 0; j < COLS - 3; j++) {
//             if (board[i][j] == disc && board[i - 1][j + 1] == disc &&
//                 board[i - 2][j + 2] == disc && board[i - 3][j + 3] == disc) {
//                 return true;
//             }
//         }
//     }
// 
//     // Check diagonal (top-left to bottom-right)
//     for (int i = 0; i < ROWS - 3; i++) {
//         for (int j = 0; j < COLS - 3; j++) {
//             if (board[i][j] == disc && board[i + 1][j + 1] == disc &&
//                 board[i + 2][j + 2] == disc && board[i + 3][j + 3] == disc) {
//                 return true;
//             }
//         }
//     }
// 
//     return false;
// }
// 
// bool isBoardFull(char board[ROWS][COLS]) {
//     for (int j = 0; j < COLS; j++) {
//         if (board[0][j] == '.') {
//             return false;
//         }
//     }
//     return true;
// }
// 
// int main() {
//     char board[ROWS][COLS];
//     initializeBoard(board);
// 
//     char player1 = 'X';
//     char player2 = 'O';
//     char currentPlayer = player1;
// 
//     printf("Welcome to Connect4!\n");
//     printBoard(board);
// 
//     while (true) {
//         int col;
//         printf("Player %c, enter a column (1-%d): ", currentPlayer, COLS);
//         scanf("%d", &col);
//         col--; // Convert to 0-based index
// 
//         if (!dropDisc(board, col, currentPlayer)) {
//             printf("Invalid move. Try again.\n");
//             continue;
//         }
// 
//         printBoard(board);
// 
//         if (checkWin(board, currentPlayer)) {
//             printf("Player %c wins!\n", currentPlayer);
//             break;
//         }
// 
//         if (isBoardFull(board)) {
//             printf("It's a draw!\n");
//             break;
//         }
// 
//         currentPlayer = (currentPlayer == player1) ? player2 : player1;
//     }
// 
//     return 0;
// }
