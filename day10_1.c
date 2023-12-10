#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define MAX_NUM_LINES 256
#define START_NODE (uint8_t)(-1)
#define NO_PREV (uint8_t)(-2)

typedef struct Node {
    uint32_t index, dist;
    char pipe;
    uint8_t prev;         // Index in adj of previous
    struct Node* adj[4];  // Order is NSWE
} Node_t;

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
                 uint8_t prev) {
    Node_t* new = (Node_t*)malloc(sizeof(Node_t));
    new->index = new_index;
    new->pipe = grid[new_index];
    new->prev = prev;
    new->dist = c_node->dist + 1;
    new->adj[prev] = c_node;
    return new;
}

void build_graph(Node_t* head, const char* const grid, uint32_t width,
                 uint32_t grid_size) {
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
        printf("%c %u %u\n", c_node->pipe, c_node->index, c_node->dist);
        if (prev != 1 && does_conn(c, 'N')) {
            new_index = index - width;
            prev = 0;
        } else if (prev != 0 && does_conn(c, 'S')) {
            new_index = index + width;
            prev = 1;
        } else if (prev != 3 && does_conn(c, 'W')) {
            new_index = index - 1;
            prev = 2;
        } else if (prev != 2 && does_conn(c, 'E')) {
            new_index = index + 1;
            prev = 3;
        } else {
            printf("?????\n");
            exit(-1);
        }
        if (new_index == head->index) {
            break;
        }
        c_node = new_node(c_node, grid, new_index, prev);
    } while (1);
    printf("\n\n");
    // Go back along the pipe and set the dist correctly
    uint32_t dist = 1;
    do {
        c_node->dist = dist;
        printf("%c %u %u\n", c_node->pipe, c_node->index, c_node->dist);
        dist++;
        c_node = c_node->adj[c_node->prev];
    } while(c_node->dist > dist);  

    if (c_node[c_node->prev].dist > dist) {
        dist++;
    }
    printf("\nMax dist = %u\n", c_node->dist);
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
    head.prev = START_NODE;
    head.pipe = 'S';
    head.dist = 0;
    build_graph(&head, grid, width, grid_size);
}
