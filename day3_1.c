#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define WIDTH 141
#define HEIGHT 140

char in_buff[MAX_LINE_LEN];
char grid[WIDTH * HEIGHT];

char* seek(char* str, char t) {
    while (*str != t && *str != '\0' && *str != '\n' && *str != ';' &&
           *str != ',') {
        str++;
    }
    return str;
}

uint8_t is_num(char c) {
    if (c >= '0' && c <= '9') {
        return 1;
    }
    return 0;
}

// Scan grid 3x3 around numbers for symbols
// Could be done more efficiently as its looking at the same place multiple
// times
uint8_t is_near_symb(char* str, uint64_t cursor) {
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

int main() {
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
        if (is_num(c)) {
            c_num[c_num_l] = c;
            c_num_symbol |= is_near_symb(grid, cursor);
            c_num_l++;
        } else if (c_num_l > 0) {
            if (c_num_symbol) {
                c_num[c_num_l] = '\0';
                printf("%s\n", c_num);
                sum += atoll(c_num);
            }
            c_num_l = 0;
            c_num_symbol = 0;
        }
        cursor++;
    }

    printf("\n%lu\n", sum);
    return 0;
}