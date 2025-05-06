
#include <stdio.h>
#include <stdbool.h>

#define ROWS 6
#define COLS 7

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

int main() {
    char board[ROWS][COLS];
    initializeBoard(board);

    char player1 = 'X';
    char player2 = 'O';
    char currentPlayer = player1;

    printf("Welcome to Connect4!\n");
    printBoard(board);

    while (true) {
        int col;
        printf("Player %c, enter a column (1-%d): ", currentPlayer, COLS);
        scanf("%d", &col);
        col--; // Convert to 0-based index

        if (!dropDisc(board, col, currentPlayer)) {
            printf("Invalid move. Try again.\n");
            continue;
        }

        printBoard(board);

        if (checkWin(board, currentPlayer)) {
            printf("Player %c wins!\n", currentPlayer);
            break;
        }

        if (isBoardFull(board)) {
            printf("It's a draw!\n");
            break;
        }

        currentPlayer = (currentPlayer == player1) ? player2 : player1;
    }

    return 0;
}
