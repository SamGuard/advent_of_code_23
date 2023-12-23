#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEG_1 (uint32_t)(-1)
#define MAX_LINE_LEN 1024
#define MAX_GRID 200 * 200

typedef struct BreadCrumb {
    uint8_t dir[4];
} BreadCrumb_t;

#define DIR_TO_INDEX(x, width) \
    (x == -1) ? 0 : ((x == 1) ? 1 : ((x == -width) ? 2 : (x == width ? 3 : 4)))

char tracker[MAX_GRID];
BreadCrumb_t crumbs[MAX_GRID];

void grid_print(const char *const grid, uint32_t width, uint32_t grid_size,
                uint32_t pos) {
    for (uint32_t y = 0; y * width < grid_size; y++) {
        for (uint32_t x = 0; x < width; x++) {
            if (x + y * width == pos) {
                printf("X");
            } else {
                printf("%c", grid[x + y * width]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void bounce(char *grid, uint32_t width, uint32_t grid_size, uint32_t pos,
            int32_t dir, uint32_t row) {
    if (pos >= grid_size || (row != NEG_1 && pos / width != row) ||
        (tracker[pos] == '#' &&
         crumbs[pos].dir[DIR_TO_INDEX(dir, (int32_t)width)] == 1)) {
        return;
    }

    tracker[pos] = '#';
    crumbs[pos].dir[DIR_TO_INDEX(dir, (int32_t)width)] = 1;

    int32_t signed_width = (int32_t)width;
    while (grid[pos] == '.') {
        uint32_t new_pos = pos + dir;
        if (new_pos >= grid_size ||
            (row != NEG_1 && pos / width != new_pos / width)) {
            return;
        }
        pos = new_pos;
        tracker[pos] = '#';
        crumbs[pos].dir[DIR_TO_INDEX(dir, (int32_t)width)] = 1;
    }

    switch (grid[pos]) {
        case '-': {
            if (dir == 1 || dir == -1) {
                bounce(grid, width, grid_size, pos + dir, dir, row);
                return;
            }
            bounce(grid, width, grid_size, pos + 1, 1, pos / width);
            bounce(grid, width, grid_size, pos - 1, -1, pos / width);
            return;
        }
        case '|': {
            if (dir == signed_width || dir == -signed_width) {
                bounce(grid, width, grid_size, pos + dir, dir, -1);
                return;
            }
            bounce(grid, width, grid_size, pos + width, width, -1);
            bounce(grid, width, grid_size, pos - width, -width, -1);
            return;
        }
        case '/': {
            if (dir == 1) {
                dir = -width;
                row = -1;
            } else if (dir == -1) {
                dir = width;
                row = -1;
            } else if (dir == signed_width) {
                dir = -1;
                row = pos / width;
            } else if (dir == -signed_width) {
                dir = 1;
                row = pos / width;
            }
            bounce(grid, width, grid_size, pos + dir, dir, row);
            return;
        }
        case '\\': {
            if (dir == 1) {
                dir = width;
                row = -1;
            } else if (dir == -1) {
                dir = -width;
                row = -1;
            } else if (dir == signed_width) {
                dir = 1;
                row = pos / width;
            } else if (dir == -signed_width) {
                dir = -1;
                row = pos / width;
            }
            bounce(grid, width, grid_size, pos + dir, dir, row);
            return;
        }
        default:
            printf("AHHHHH %c\n", grid[pos]);
            exit(-1);
    }
}

int main() {
    char in_buff[MAX_LINE_LEN];
    char grid[MAX_GRID];
    uint32_t row = 0, width = -1;

    FILE *f = fopen("in/day16.in", "r");
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

    const uint32_t grid_size = row * width, height = row;
    uint32_t max_sum = 0;
    grid_print(grid, width, grid_size, -1);

    for (uint32_t i = 0; i < 2 * (width + height); i++) {
        uint32_t pos, row;
        int32_t dir;
        memset(tracker, '.', sizeof(char) * grid_size);
        memset(crumbs, 0, sizeof(BreadCrumb_t) * grid_size);

        if (i < width) {
            pos = i;
            dir = width;
            row = -1;
        } else if (i < 2 * width) {
            pos = (height - 1) * width + (i - width);
            dir = -width;
            row = -1;
        } else if (i < 2 * width + height) {
            pos = (i - 2 * width) * width;
            dir = 1;
            row = pos / width;
        } else if (i < 2 * (width + height)) {
            pos = (i - 2 * width + height) * width + width - 1;
            dir = -1;
            row = pos / width;
        }
        bounce(grid, width, grid_size, pos, dir, row);
        grid_print(tracker, width, grid_size, -1);
        uint32_t sum = 0;
        for (uint32_t i = 0; i < grid_size; i++) {
            if (tracker[i] == '#') {
                sum += 1;
            }
        }
        printf("%u\n", sum);
        if (sum > max_sum) {
            max_sum = sum;
        }
    }
    printf("\nMax sum: %u\n", max_sum);
    return 0;
}
