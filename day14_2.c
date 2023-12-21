#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define MAX_GRID 128 * 128

void grid_print(const char *const grid, uint32_t width, uint32_t grid_size) {
    for (uint32_t y = 0; y * width < grid_size; y++) {
        for (uint32_t x = 0; x < width; x++) {
            printf("%c", grid[x + y * width]);
        }
        printf("\n");
    }
    printf("\n");
}

uint64_t tilt(char *const grid, const uint32_t width, const uint32_t grid_size,
              const char dir) {
    uint64_t weight = 0;
    uint32_t total_row = grid_size / width;
    uint32_t diff, start_x = 0, end_x = width, diff_x = 1, start_y = 0,
                   diff_y = 1;
    uint8_t lock_row = 0;
    switch (dir) {
        case 'N':
            diff = -width;
            break;
        case 'S':
            diff = width;
            start_y = (grid_size - 1) / width;
            diff_y = -1;
            break;
        case 'E':
            diff = 1;
            lock_row = 1;
            start_x = width - 1;
            end_x = 0 - 1;
            diff_x = -1;
            break;
        case 'W':
            diff = -1;
            lock_row = 1;
            break;
        default:
            return -1;
            break;
    }
    for (uint32_t y = start_y; y * width < grid_size; y += diff_y) {
        for (uint32_t x = start_x; x < grid_size && x != end_x; x += diff_x) {
            uint32_t o_pos = x + y * width, pos;
            pos = o_pos;
            if (grid[pos] != 'O') {
                continue;
            }
            if (lock_row) {
                while (pos + diff < grid_size &&
                       pos / width == (pos + diff) / width &&
                       grid[pos + diff] == '.') {
                    pos += diff;
                }
            } else {
                while (pos + diff < grid_size && grid[pos + diff] == '.') {
                    pos += diff;
                }
            }
            grid[pos] = 'O';
            if (pos != o_pos) {
                grid[o_pos] = '.';
            }
            weight += total_row - pos / width;
        }
    }
    return weight;
}

uint64_t roll(char *const grid, const uint32_t width,
              const uint32_t grid_size) {
    uint64_t weight = 0;

    uint32_t num_saved = 0, buffer_size = 65536 * grid_size;
        char *saved_grid = (char *)malloc(sizeof(char) * buffer_size);
    memset(saved_grid, 0, grid_size * sizeof(char));
    for (uint32_t i = 0; i < 1000000000; i++) {
        tilt(grid, width, grid_size, 'N');
        // grid_print(grid, width, grid_size);
        tilt(grid, width, grid_size, 'W');
        // grid_print(grid, width, grid_size);
        tilt(grid, width, grid_size, 'S');
        // grid_print(grid, width, grid_size);
        weight = tilt(grid, width, grid_size, 'E');
        // grid_print(grid, width, grid_size);
        for (uint32_t i = 0; i < num_saved; i++) {
            char *cache = saved_grid + num_saved * grid_size * sizeof(char);
            if (memcmp(cache, grid, grid_size * sizeof(char)) == 0) {
                printf("FOUND FOUND FOUND\n");
                exit(0);
            }
        }
        memcpy(saved_grid + num_saved * grid_size * sizeof(char), grid,
               grid_size * sizeof(char));
        num_saved++;
        if (num_saved * grid_size >= buffer_size) {
          buffer_size *= 1.25;
          saved_grid = realloc(saved_grid, sizeof(char) * buffer_size);
          if (saved_grid == NULL){
            printf("No mem:(\n");
            exit(-1);
          }
        }
        if (i % 1000000 == 0) {
            grid_print(grid, width, grid_size);
            printf("%u\n", i);
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
    uint64_t out = roll(grid, width, grid_size);
    grid_print(grid, width, grid_size);
    printf("%lu\n", out);
    return 0;
}
