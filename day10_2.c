#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define MAX_NUM_LINES 256
#define START_NODE (uint8_t)(-1)
#define NO_PREV (uint8_t)(-2)

#define PIPE 'X'

#define IN_OUT(x) (x == 0) ? '0' : '1';

typedef struct Node {
    uint32_t index;
    char pipe, dir;
    struct Node* prev;  // Index in adj of previous
} Node_t;

void grid_print(const char* const grid, const uint32_t width,
                const uint32_t grid_size) {
    for (uint32_t i = 0; i < grid_size; i++) {
        if (i % width == 0) {
            printf("\n");
        }
        printf("%c", grid[i]);
    }
    printf("\n");
}

char get_left(char forward) {
    switch (forward) {
        case 'N':
            return 'W';
        case 'S':
            return 'E';
        case 'W':
            return 'S';
        case 'E':
            return 'N';

        default:
            printf("\n\nget_left\n\n");
            exit(-1);
    }
}

char oppo(char c) {
    switch (c) {
        case 'N':
            return 'S';
        case 'S':
            return 'N';
        case 'W':
            return 'E';
        case 'E':
            return 'W';
        default:
            printf("\n%c oppo no work\n", c);
            exit(-1);
    }
}

// Pipe and the direction to check. RULES:
/*
    | is a vertical pipe connecting north and south.
    - is a horizontal pipe connecting east and west.
    L is a 90-degree bend connecting north and east.
    J is a 90-degree bend connecting north and west.
    7 is a 90-degree bend connecting south and west.
    F is a 90-degree bend connecting south and east.
    . is ground; there is no pipe in this tile.
    S is the starting position of the animal
*/
uint8_t does_conn(char pipe, char d) {
    switch (pipe) {
        case '|':
            return d == 'N' || d == 'S';
        case '-':
            return d == 'W' || d == 'E';
        case 'L':
            return d == 'N' || d == 'E';
        case 'J':
            return d == 'N' || d == 'W';
        case '7':
            return d == 'S' || d == 'W';
        case 'F':
            return d == 'S' || d == 'E';
        case '.':
            return 0;
        case 'S': {
            printf("START TILE CHECKED\n");
            return 0;
        }
        default:
            printf("You missed a tile dumbass\n");
            return 0;
    }
}

char dir_to_pipe(uint8_t n, uint8_t s, uint8_t w, uint8_t e) {
    if (n == 1 && s == 1) {
        return '|';
    }
    if (w == 1 && e == 1) {
        return '-';
    }
    if (n == 1 && w == 1) {
        return 'J';
    }
    if (n == 1 && e == 1) {
        return 'L';
    }
    if (s == 1 && w == 1) {
        return '7';
    }
    if (s == 1 && e == 1) {
        return 'F';
    }
    printf("Oops %d %d %d %d\n", n, s, w, e);
    exit(-1);
}

Node_t* new_node(Node_t* c_node, const char* const grid, uint32_t new_index,
                 char dir) {
    Node_t* new = (Node_t*)malloc(sizeof(Node_t));
    new->index = new_index;
    new->pipe = grid[new_index];
    new->prev = c_node;
    new->dir = dir;
    return new;
}

void set_adj(char* occ, const uint32_t pos, uint32_t width, uint32_t grid_size,
             char forward) {
    uint32_t n_pos_l, n_pos_r;  // new pos
    switch (forward) {
        case 'N': {
            n_pos_l = pos - 1;
            n_pos_r = pos + 1;
            break;
        }
        case 'S': {
            n_pos_l = pos + 1;
            n_pos_r = pos - 1;
            break;
        }
        case 'W': {
            n_pos_l = pos + width;
            n_pos_r = pos - width;
            break;
        }
        case 'E': {
            n_pos_l = pos - width;
            n_pos_r = pos + width;
            break;
        }

        default:
            printf("\nAHHHHH\n");
            exit(-1);
    }
    if (n_pos_l < grid_size && occ[n_pos_l] != PIPE) {
        occ[n_pos_l] = '1';
    }
    if (n_pos_r < grid_size && occ[n_pos_r] != PIPE) {
        occ[n_pos_r] = '0';
    }
}

void rotate_faces(char* forward_ptr, char pipe) {
    if (pipe == '|' || pipe == '-') {
        return;
    }
    char forward = *forward_ptr;

    if (pipe == 'L') {
        *forward_ptr = forward == 'S' ? 'E' : 'N';
    } else if (pipe == '7') {
        *forward_ptr = forward == 'N' ? 'W' : 'S';
    } else if (pipe == 'F') {
        *forward_ptr = forward == 'N' ? 'E' : 'S';
    } else if (pipe == 'J') {
        *forward_ptr = forward == 'S' ? 'W' : 'N';
    }
}

void flood_it(char* occ, uint32_t width, uint32_t grid_size) {
    uint8_t changed_something = 0;
    do {
        changed_something = 0;
        for (uint32_t pos = 0; pos < grid_size; pos++) {
            if (occ[pos] != '.') {
                continue;
            }
            uint32_t new_pos;
            new_pos = pos - width;
            if (new_pos < grid_size && occ[new_pos] != '.') {
                occ[pos] = occ[new_pos];
                changed_something = 1;
                continue;
            }
            new_pos = pos + width;
            if (new_pos < grid_size && occ[new_pos] != '.') {
                occ[pos] = occ[new_pos];
                changed_something = 1;
                continue;
            }
            new_pos = pos - 1;
            if (new_pos < grid_size && occ[new_pos] != '.') {
                occ[pos] = occ[new_pos];
                changed_something = 1;
                continue;
            }
            new_pos = pos + 1;
            if (new_pos < grid_size && occ[new_pos] != '.') {
                occ[pos] = occ[new_pos];
                changed_something = 1;
                continue;
            }
        }
    } while (changed_something == 1);
}

void build_graph(Node_t* head, char* const grid, uint32_t width,
                 uint32_t grid_size) {
    char* occ = (char*)malloc(grid_size * sizeof(char));  // Occupancy
    memset(occ, '.', grid_size * sizeof(char));
    // Figure out what the start node is supposed to be
    {
        uint8_t n, s, w, e;
        uint32_t pos;
        pos = head->index - width;
        n = pos < grid_size ? does_conn(grid[pos], 'S') : 0;  // Check North
        pos = head->index + width;
        s = pos < grid_size ? does_conn(grid[pos], 'N') : 0;  // Check South
        pos = head->index - 1;
        w = pos < grid_size ? does_conn(grid[pos], 'E') : 0;  // Check West
        pos = head->index + 1;
        e = pos < grid_size ? does_conn(grid[pos], 'W') : 0;  // Check East
        head->pipe = dir_to_pipe(n, s, w, e);
    }
    printf("HEAD PIPE: %c\n", head->pipe);
    Node_t* c_node = head;
    uint8_t prev = -1;
    do {
        char c = c_node->pipe;
        uint32_t index = c_node->index, new_index;
        occ[index] = PIPE;
        // printf("%c %u\n", c_node->pipe, c_node->index);
        if (prev != 'S' && does_conn(c, 'N')) {
            new_index = index - width;
            prev = 'N';
        } else if (prev != 'N' && does_conn(c, 'S')) {
            new_index = index + width;
            prev = 'S';
        } else if (prev != 'E' && does_conn(c, 'W')) {
            new_index = index - 1;
            prev = 'W';
        } else if (prev != 'W' && does_conn(c, 'E')) {
            new_index = index + 1;
            prev = 'E';
        } else {
            printf("?????\n");
            exit(-1);
        }
        if (new_index == head->index) {
            head->prev = c_node;
            head->dir = prev;
            break;
        }
        c_node = new_node(c_node, grid, new_index, prev);
    } while (1);

    // Set all on one side to 1 and others to 0
    char forward = oppo(prev);
    while (1) {
        set_adj(occ, c_node->index, width, grid_size, forward);
        rotate_faces(&forward, c_node->pipe);
        set_adj(occ, c_node->index, width, grid_size, forward);
        // printf("\n");
        // grid_print(occ, width, grid_size);
        // printf("%c %c\n", forward, c_node->pipe);
        // printf("\n");

        if (c_node == head) {
            break;
        }

        c_node = c_node->prev;
    }
    grid_print(occ, width, grid_size);
    printf("\n");
    flood_it(occ, width, grid_size);
    grid_print(occ, width, grid_size);

    uint32_t zero_count = 0, one_count = 0;
    for (uint32_t i = 0; i < grid_size; i++) {
        if (occ[i] == '0') {
            zero_count++;
        } else if (occ[i] == '1') {
            one_count++;
        }
    }
    printf("Zero: %u    One: %u\n", zero_count, one_count);
}

int main() {
    char in_buff[MAX_LINE_LEN];
    char grid[200 * 200];
    uint32_t row = 0, width = -1, start = -1;

    FILE* f = fopen("in/day10.in", "r");
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
    uint32_t const grid_size = row * width;
    for (uint32_t i = 0; i < grid_size; i++) {
        if (grid[i] == 'S') {
            start = i;
            break;
        }
    }

    Node_t head;
    head.index = start;
    head.prev = NULL;
    head.pipe = 'S';
    head.dir = '\0';
    build_graph(&head, grid, width, grid_size);
}
