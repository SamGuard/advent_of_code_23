#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define MAX_GRID 200 * 200

uint32_t find_reflect(const char *const grid, const uint32_t width,
                      const uint32_t grid_size, uint32_t *n_cols, uint32_t *n_rows) {
    const uint32_t height = grid_size / width;
    for (uint32_t y = 1; y < height; y++) {
        // check rows d away from y
        uint8_t match = 1;
        for (uint32_t d = 1; y - d < height && y + d - 1 < height && match;
             d++) {
            // check rows match
            for (uint32_t x = 0; x < width; x++) {
                // printf("%c", grid[x + (y - d) * width]);
                if (grid[x + (y - d) * width] !=
                    grid[x + (y + d - 1) * width]) {
                    match = 0;
                    break;
                }
            }
        }
        if (match) {
            *n_rows += y;
            return y;
        }
    }
    for (uint32_t x = 1; x < width; x++) {
        // check rows d away from y
        uint8_t match = 1;
        for (uint32_t d = 1; x - d < width && x + d - 1 < width && match; d++) {
            // check rows match
            for (uint32_t y = 0; y < height; y++) {
                // printf("%c", grid[x + (y - d) * width]);
                if (grid[x - d + y * width] != grid[x + d - 1 + y * width]) {
                    match = 0;
                    break;
                }
            }
        }
        if (match) {
            *n_cols += x;
            return x;
        }
    }
    return 0;
}

int main() {
    char in_buff[MAX_LINE_LEN];
    char grid[MAX_GRID];
    uint32_t row = 0, width = -1, set = 0, n_cols, n_rows;

    FILE *f = fopen("in/day13.in", "r");
    if (f == NULL) {
        printf("File no open\n");
        return -1;
    }
    while (fgets(in_buff, sizeof(char) * MAX_LINE_LEN, f) != NULL) {
        if (width == (uint32_t)(-1)) {
            width = strlen(in_buff) - 1;  // -1 for the \n
        }
        if (in_buff[0] == '\n') {
            const uint32_t grid_size = row * width;
            uint32_t n = find_reflect(grid, width, grid_size, &n_cols, &n_rows);
            printf("set: %u %u\n", set, n);
            row = 0;
            memset(grid, 0, sizeof(char) * MAX_GRID);
            width = -1;
            set++;
            if (in_buff == NULL) {
                break;
            }
            continue;
        }
        // printf("%s\n", in_buff);
        memcpy(grid + row * width, in_buff, width);
        row++;
    }
    const uint32_t grid_size = row * width;
    uint32_t n = find_reflect(grid, width, grid_size, &n_cols, &n_rows);
    printf("%u\n", n);
    printf("\nn_cols:%u n_rows:%u = %u\n", n_cols, n_rows, 100 * n_rows + n_cols);
    return 0;
}
