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

typedef struct Visit_Period {
    uint64_t v_1, v_2, loc, index;
} Visit_Period_t;

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

uint8_t is_goal(uint64_t x) {
    return (x % 26) == ('Z' - 'A');
}

void parse_nums(char* str, Map_t* map, uint64_t* ghosts, uint64_t* num_ghosts) {
    uint64_t index, left, right;
    index = hash(str);
    left = hash(str + 7);
    right = hash(str + 7 + 5);

    map[index].l = left;
    map[index].r = right;
    printf("%lu %lu %lu\n", index, left, right);
    if (str[2] != 'A') {
        return;
    }
    ghosts[*num_ghosts] = index;
    (*num_ghosts)++;
}

uint64_t get_max(uint64_t * nums, uint64_t l) {
    uint64_t m = nums[0];
    for (uint64_t i = 1; i < l; i++){
        if (nums[i] > m) {
            m = nums[i];
        }
    }
    return m;
}

int main() {
    char in_buff[MAX_LINE_LEN], inst[MAX_LINE_LEN];
    Map_t map[MAX_MAPS];
    uint64_t ghosts[26 * 26];
    Visit_Period_t ghost_visit[2][26 * 26];
    uint64_t num_steps = 0, num_ghosts = 0;
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
        parse_nums(in_buff, map, ghosts, &num_ghosts);
    }

    printf("num ghosts %lu\n", num_ghosts);
    for (uint64_t i = 0; i < 2; i++) {
        for (uint64_t j = 0; j < num_ghosts; j++) {
            ghost_visit[i][j].v_1 = NONUM;
            ghost_visit[i][j].v_2 = NONUM;
            ghost_visit[i][j].loc = NONUM;
        }
    }
    uint64_t index = 0, steps = 0, num_found = 0;
    while (1) {
        char c = inst[index];
        uint64_t num_at_goal = 0;
        for (uint64_t i = 0; i < num_ghosts; i++) {
            uint64_t loc = ghosts[i];
            ghosts[i] = c == 'L' ? map[loc].l : map[loc].r;
            loc = ghosts[i];
            if (is_goal(ghosts[i])) {
                uint8_t l_or_r = c == 'L' ? 1 : 0;
                Visit_Period_t* vis = &ghost_visit[l_or_r][i];
                if (vis->loc == NONUM) {
                    vis->loc = loc;
                    vis->index = index;
                }
                else if (vis->loc != loc || vis->index != index) {
                    continue;
                }
                if (vis->v_1 == NONUM) {
                    vis->v_1 = steps;
                    continue;
                }
                if (vis->v_2 == NONUM) {
                    vis->v_2 = steps;
                    num_found++;
                    char *loc_c = anti_hash(loc);
                    printf("Found ghost %lu at loc %c%c%c index %lu step %lu\n", i, loc_c[0], loc_c[1], loc_c[2], index, steps);
                    continue;
                }
            }
        }
        steps++;
        if (num_at_goal == num_ghosts || num_found == num_ghosts) {
            break;
        }
        index = (index + 1) % num_steps;

        if (steps % 10 == 0) {
            // for (uint64_t i = 0; i < num_ghosts; i++) {
            //     printf("%lu    ", ghosts[i]);
            // }
            //printf("%lu \n", steps);
        }
    }
    uint64_t nums[26 * 26], diffs[26 * 26];
    for (uint64_t i = 0; i < num_ghosts; i++) {
        uint64_t v_1, v_2;
        if (ghost_visit[0][i].v_1 != NONUM && ghost_visit[0][i].v_1 != NONUM) {
            v_1 = ghost_visit[0][i].v_1;
            v_2 = ghost_visit[0][i].v_2;
        }
        else {
            v_1 = ghost_visit[1][i].v_1;
            v_2 = ghost_visit[1][i].v_2;
        }
        nums[i] = v_1;
        diffs[i] = v_2 - v_1;
        printf("num %lu diff %lu\n", nums[i], diffs[i]);
    }
    // I cheated and used a LCM calculator here ;)
    return 0;
}