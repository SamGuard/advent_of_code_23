#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NONUM (uint64_t)(-1)
#define MAX_LINE_LEN 1024
#define MAX_MAPS 26*26*26

typedef struct Map {
    uint64_t l, r;
} Map_t;

char* seek(char* str, char t) {
    while (*str != t && *str != '\0' && *str != '\n') {
        str++;
    }
    return str;
}

uint64_t hash(char* in) {
    return 26 * (26 * (in[0] - 'A') + (in[1] - 'A')) + (in[2] - 'A');
}

char* anti_hash(uint64_t x) {
    char* str = (char*)malloc(3 * sizeof(char));
    uint64_t col1, col2, col3;
    col1 = x / (26 * 26);
    col2 = (x - col1 * 26 * 26) / 26;
    col3 = (x - 26 * (col1 * 26 + col2));
    str[0] = col1 + 'A';
    str[1] = col2 + 'A';
    str[2] = col3 + 'A';
    return str;
}

void parse_nums(char* str, Map_t* map) {
    uint64_t index, left, right;
    index = hash(str);
    left = hash(str + 7);
    right = hash(str + 7 + 5);

    map[index].l = left;
    map[index].r = right;
    printf("%lu %lu %lu\n", index, left, right);
}

int main() {
    char in_buff[MAX_LINE_LEN], inst[MAX_LINE_LEN];
    Map_t map[MAX_MAPS];
    uint64_t num_steps;
    inst[0] = '\0';

    FILE* f = fopen("in/day8.in", "r");
    while (fgets(in_buff, sizeof(char) * MAX_LINE_LEN, f) != NULL) {
        if (inst[0] == '\0') {
            strcpy(inst, in_buff);
            num_steps = strlen(inst) - 1;
            continue;
        }
        if (in_buff[0] == '\n') {
            continue;
        }
        parse_nums(in_buff, map);
    }

    const uint64_t goal = hash("ZZZ");
    uint64_t loc = hash("AAA"), index = 0, steps = 0;
    while (loc != goal) {
        char * str = anti_hash(loc);
        printf("%c%c%c %c\n", str[0], str[1], str[2], inst[index]);
        free(str);
        loc = inst[index] == 'L' ? map[loc].l : map[loc].r;
        index = (index + 1) % num_steps;
        steps++;
    }
    printf("%lu\n", steps);
}