#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <raylib.h>

#define WIDTH 1000
#define HEIGHT 1000
#define GRID_SIZE 10
#define NUM_ROWS (WIDTH / GRID_SIZE)
#define NUM_COLS (HEIGHT / GRID_SIZE)

void sInput(int* grid, int* buffer);
static inline void iterateCA(int* grid, int* buffer);
static inline void clearGrid(int* grid);

// Glob
static bool isRunning = false;

static int directions[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0},
                      {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

int main(int argc, char** argv)
{
    InitWindow(WIDTH, HEIGHT, "PersianCarpet Generator");
    SetTargetFPS(30);

    // Alocations
    int* grid = malloc(sizeof(int) * NUM_ROWS * NUM_COLS);
    if (!grid) return -1;
    int* nextBuff = malloc(sizeof(int) * NUM_ROWS * NUM_COLS);
    if (!nextBuff) return -1;
    clearGrid(grid);

    // Main loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        sInput(grid, nextBuff);

        if (isRunning) {
            iterateCA(grid, nextBuff);
        }

        // Rendering
        for (int i = 1; i < NUM_ROWS - 1; i++)
            for (int j = 1; j < NUM_COLS - 1; j++)
                DrawRectangle(i * GRID_SIZE, j * GRID_SIZE, GRID_SIZE - 1, GRID_SIZE - 1, (grid[i * NUM_ROWS + j]) ? BLACK : WHITE);

        EndDrawing();
    }

    // Realeasing
    free(grid);
    free(nextBuff);
    CloseWindow();
    return 0;
}

static inline void clearGrid(int* grid) 
{
    for (int i = 0; i < NUM_ROWS; i++)
        for (int j = 0; j < NUM_COLS; j++)
            grid[i * NUM_ROWS + j] = 0;
}

void sInput(int* grid, int* buffer)
{
    int grid_x = GetMouseX() / GRID_SIZE;
    int grid_y = GetMouseY() / GRID_SIZE;

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        grid[grid_x * NUM_ROWS + grid_y] = 1;
    }

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        grid[grid_x * NUM_ROWS + grid_y] = 0;
    }

    if (IsKeyPressed(KEY_SPACE))
        isRunning = !isRunning;

    if (IsKeyPressed(KEY_R)) {
        clearGrid(grid);
        isRunning = false;
    }

    if (IsKeyPressed(KEY_I)) {
        iterateCA(grid, buffer);
    }
}

static inline void iterateCA(int* grid, int* buffer)
{
    for (int i  = 1; i < NUM_ROWS - 1; i++) {
        for (int j = 1; j < NUM_COLS - 1; j++) {
            int cell = grid[i * NUM_ROWS + j];
            int livingN = 0;
            // Rules
            for (int k = 0; k < 8; k++)
                livingN += grid[(i + directions[k][0]) * NUM_ROWS + (j + directions[k][1])];

            int final = 0;
            if (!cell) {
                if (livingN == 2 || livingN == 3 || livingN == 4)
                    final = 1;
            }

            buffer[i * NUM_ROWS + j] = final;
        }
    }

    memcpy(grid, buffer, NUM_ROWS * NUM_COLS * sizeof(int));
}
