#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NONUM (uint64_t)(-1)
#define MAX_LINE_LEN 1024

#define MAX_LIST_SIZE 64
typedef struct uint_list {
    uint32_t size;
    uint64_t list[MAX_LIST_SIZE];
} uint_list_t;

typedef struct uint_list_list {
    uint32_t size;
    uint_list_t list[MAX_LIST_SIZE];
} uint_list_list_t;

uint8_t is_num(char c) {
    if (c >= '0' && c <= '9') {
        return 1;
    }
    return 0;
}

char* seek(char* str, char t) {
    while (*str != t && *str != '\0' && *str != '\n') {
        str++;
    }
    return str;
}

#define DELIM " \n\0"
uint_list_t* parse_nums(char* str) {
    uint_list_t* list = (uint_list_t*)malloc(sizeof(uint_list_t));
    uint64_t* nums = list->list;
    uint32_t* size = &(list->size);
    char* token = strtok(str, DELIM);
    do {
        *nums = atol(token);
        nums++;
        (*size)++;
    } while ((token = strtok(NULL, DELIM)) != NULL);
    return list;
}

uint64_t find_answer(uint_list_list_t* maps, uint64_t num) {
    for (int level = 0; level < 7; level++){
        for (uint32_t m = 0; m < maps->size; m++) {
            uint_list_t l = maps->list[m];
            uint64_t src, dst, rng;
            src = l.list[1];
            dst = l.list[0];
            rng = l.list[2];
            // printf("n:%lu s:%lu r:%lu d:%lu ", num, src, rng, dst);
            if (num >= src && num < src + rng) {
                num = (dst + num) - src;
                break;
            }
            // printf("%lu\n", num);
        }
        // printf("---\n");
        maps++;
    }
    return num;
}

int main() {
    char in_buff[MAX_LINE_LEN];
    uint_list_t* seeds;
    uint_list_list_t maps[7];

    FILE* f = fopen("in/day5.in", "r");
    uint8_t state = 0;
    while (fgets(in_buff, sizeof(char) * MAX_LINE_LEN, f) != NULL) {
        // printf("%lu    %s", sum, in_buff);
        // safe memory management is for nerds
        char* cursor, * cursor_2;
        if (in_buff[0] == '\n') {
            state++;
            continue;
        }
        cursor = cursor_2 = in_buff;
        if (state == 0) {
            cursor = seek(cursor, ':') + 1;
            seeds = parse_nums(cursor);
        }
        else if (!is_num(*cursor)) {
            continue;
        }
        else {
            maps[state - 1].list[maps[state - 1].size] = *parse_nums(cursor);
            maps[state - 1].size++;
        }
    }
    printf("\n\n");
    uint64_t min = -1;
    for (uint64_t i = 0; i < seeds->size; i += 2) {
        for (uint64_t j = seeds->list[i]; j < seeds->list[i] + seeds->list[i + 1]; j++) {
            uint64_t loc = find_answer(maps, j);
            //printf("%lu %lu\n", j, loc);
            min = (min > loc) ? loc : min;
        }
    }
    printf("\n\n%lu\n", min);
}