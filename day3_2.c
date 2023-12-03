#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NONUM (uint64_t)(-1)
#define MAX_LINE_LEN 1024
#define WIDTH 141
#define HEIGHT 140

uint8_t is_num(char c) {
    if (c >= '0' && c <= '9') {
        return 1;
    }
    return 0;
}

// Scan grid 3x3 around numbers for symbols
// Could be done more efficiently as its looking at the same place multiple
// times
uint8_t is_near_symb(const char* str, const uint64_t cursor) {
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            uint64_t o_cursor = cursor + x - 1 + (y - 1) * WIDTH;
            if (o_cursor == cursor || o_cursor < 0 ||
                o_cursor > WIDTH * HEIGHT) {
                continue;
            }
            char c = str[o_cursor];
            if (c != '.' && c != '\n' && !is_num(c)) {
                return 1;
            }
        }
    }
    return 0;
}

uint64_t star_thing(const uint64_t* nums, const uint64_t cursor) {
    uint8_t counted_num = 0;
    uint64_t num_parts = 0, num1 = NONUM, num2 = NONUM;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            uint64_t o_cursor = cursor + x - 1 + (y - 1) * WIDTH;
            if (o_cursor < 0 || o_cursor > WIDTH * HEIGHT) {
                continue;
            }
            uint64_t n = nums[o_cursor];
            if (n == NONUM) {
                counted_num = 0;
                continue;
            }
            if (counted_num == 0) {
                num_parts++;
                num2 = num1 != NONUM ? n : num2;
                num1 = num1 == NONUM ? n : num1;
                counted_num = 1;
            }
        }
        counted_num = 0;
    }
    if (num_parts != 2) {
        return 0;
    }
    return num1 * num2;
}

int main() {
    char in_buff[MAX_LINE_LEN], grid[WIDTH * HEIGHT];
    uint64_t nums[WIDTH * HEIGHT];

    FILE* f = fopen("in/day3.in", "r");
    uint64_t sum = 0, row = 0;
    while (fgets(in_buff, sizeof(char) * MAX_LINE_LEN, f) != NULL) {
        // printf("%lu    %s", sum, in_buff);
        // safe memory management is for nerds
        memcpy(&(grid[row * WIDTH]), in_buff, WIDTH);
        row++;
    }

    uint64_t cursor = 0,       // Points to next char
        c_num_l = 0;           // length of current number
    uint8_t c_num_symbol = 0;  // Is this num next to a symbol
    char c_num[MAX_LINE_LEN];  // Current number

    while (cursor < WIDTH * HEIGHT) {
        char c = grid[cursor];
        nums[cursor] = NONUM;
        if (is_num(c)) {
            c_num[c_num_l] = c;
            c_num_symbol |= is_near_symb(grid, cursor);
            c_num_l++;
        } else if (c_num_l > 0) {
            if (c_num_symbol) {
                c_num[c_num_l] = '\0';
                uint64_t n = atoll(c_num);
                // printf("%lu\n", n);
                for (int i = cursor - c_num_l; i < cursor; i++) {
                    nums[i] = n;
                }
            }
            c_num_l = 0;
            c_num_symbol = 0;
        }
        cursor++;
    }

    cursor = 0;
    while (cursor < WIDTH * HEIGHT) {
        char c = grid[cursor];
        if (c == '*') {
            sum += star_thing(nums, cursor);
            printf("%lu\n", star_thing(nums, cursor));
        }
        cursor++;
    }

    printf("\n%lu\n", sum);
    return 0;
}