#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define MAX_GRID 200 * 200

void grid_print(const char *const grid, uint32_t width, uint32_t grid_size) {
    for (uint32_t y = 0; y * width < grid_size; y++) {
        for (uint32_t x = 0; x < width; x++) {
            printf("%c", grid[x + y * width]);
        }
        printf("\n");
    }
}

uint64_t tilt(char *const grid, uint32_t width, uint32_t grid_size) {
    uint64_t weight = 0;
    uint32_t total_row = grid_size / width;
    for (uint32_t y = 0; y * width < grid_size; y++) {
        for (uint32_t x = 0; x < width; x++) {
            uint32_t o_pos = x + y * width, pos;
            pos = o_pos;
            if (grid[pos] != 'O') {
                continue;
            }
            while (pos - width < grid_size && grid[pos - width] == '.') {
                pos -= width;
            }
            grid[pos] = 'O';
            if (pos != o_pos) {
                grid[o_pos] = '.';
            }
            weight += total_row - pos / width;
            printf("\n");
            grid_print(grid, width, grid_size);
        }
    }
    return weight;
}

int main() {
    char in_buff[MAX_LINE_LEN];
    char grid[MAX_GRID];
    uint32_t row = 0, width = -1;

    FILE *f = fopen("in/day14.in", "r");
    if (f == NULL) {
        printf("File no open\n");
        return -1;
    }
    while (fgets(in_buff, sizeof(char) * MAX_LINE_LEN, f) != NULL) {
        if (width == (uint32_t)(-1)) {
            width = strlen(in_buff) - 1;  // -1 for the \n
        }
        memcpy(grid + row * width, in_buff, width);
        row++;
    }
    const uint32_t grid_size = row * width;
    grid_print(grid, width, grid_size);
    uint64_t out = tilt(grid, width, grid_size);
    grid_print(grid, width, grid_size);
    printf("%lu\n", out);
    return 0;
}
