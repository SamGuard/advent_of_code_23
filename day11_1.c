#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define MAX_GRID 200 * 200

typedef struct Tile {
    uint32_t vert_dist, horz_dist;
} Tile_t;

void tiles_print(Tile_t *dists, uint32_t width, uint32_t grid_size) {
    for (uint32_t i = 0; i < grid_size; i++) {
        printf("%u  ", dists[i].horz_dist);
        if ((i + 1) % width == 0) {
            printf("\n");
        }
    }
    printf("\n\n");
    for (uint32_t i = 0; i < grid_size; i++) {
        printf("%u  ", dists[i].vert_dist);
        if ((i + 1) % width == 0) {
            printf("\n");
        }
    }
    printf("\n\n");
    for (uint32_t i = 0; i < grid_size; i++) {
        if (dists[i].vert_dist > 1 && dists[i].horz_dist > 1) {
            printf("+  ");
        } else if (dists[i].vert_dist > 1) {
            printf("-  ");
        } else if (dists[i].horz_dist > 1) {
            printf("|  ");
        } else {
            printf(".  ");
        }
        if ((i + 1) % width == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

#define MASK_SIZE 200
void adjust_dists(Tile_t *dists, uint32_t *galax, uint32_t galax_count,
                  uint32_t width, uint32_t grid_size) {
    uint8_t row_mask[MASK_SIZE], column_mask[MASK_SIZE];
    memset(row_mask, 0, MASK_SIZE);
    memset(column_mask, 0, MASK_SIZE);
    for (uint32_t i = 0; i < galax_count; i++) {
        row_mask[galax[i] / width] = 1;
        column_mask[galax[i] % width] = 1;
    }
    for (uint32_t i = 0; i < grid_size; i++) {
        uint32_t x = i % width, y = i / width;
        if (!column_mask[x]) {
            dists[i].horz_dist *= 2;
        }
        if (!row_mask[y]) {
            dists[i].vert_dist *= 2;
        }
    }
}

uint32_t get_path(const Tile_t *const dists, const uint32_t p1,
                  const uint32_t p2, const uint32_t width) {
    if (p1 == p2) {
        return 0;
    }
    uint32_t moves = 0, p1x = p1 % width, p1y = p1 / width,
             p2x = p2 % width, p2y = (p2 / width);
    int32_t d_x = (int32_t)p2x - (int32_t)p1x,
            d_y = (int32_t)p2y - (int32_t)p1y;
    // Give magnitude of 1
    d_x = d_x == 0 ? 0 : d_x / abs(d_x);
    d_y = d_y == 0 ? 0 : d_y / abs(d_y);

    while (p1x != p2x) {
        moves += dists[p1x + p1y * width].horz_dist;
        p1x += d_x;
        // printf("(%u,%u)->(%u,%u) (%d,%d)\n", p1x, p1y, p2x, p2y, d_x, d_y);
    }
    while (p1y != p2y) {
        moves += dists[p1x + p1y * width].vert_dist;
        p1y += d_y;
        // printf("(%u,%u)->(%u,%u) (%d,%d)\n", p1x, p1y, p2x, p2y, d_x, d_y);
    }
    return moves;
}

void shortest_paths(Tile_t *dists, uint32_t *points, uint32_t p_count,
                    uint32_t width) {
    uint64_t sum = 0;
    for (uint32_t i = 0; i < p_count - 1; i++) {
        for (uint32_t j = i + 1; j < p_count; j++) {
            uint32_t d = get_path(dists, points[i], points[j], width);
            printf("%u->%u=%u\n", i, j, d);
            sum += d;
        }
    }
    printf("\n%lu\n", sum);
}

int main() {
    char in_buff[MAX_LINE_LEN];
    char grid[MAX_GRID];
    uint32_t row = 0, width = -1;

    FILE *f = fopen("in/day11.in", "r");
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
    Tile_t dists[MAX_GRID];
    uint32_t galax[MAX_GRID], galax_count = 0;
    // Find all galaxies and set the default distances
    for (uint32_t i = 0; i < grid_size; i++) {
        dists[i].vert_dist = dists[i].horz_dist = 1;
        if (grid[i] == '#') {
            galax[galax_count] = i;
            galax_count++;
        }
    }

    adjust_dists(dists, galax, galax_count, width, grid_size);
    tiles_print(dists, width, grid_size);
    shortest_paths(dists, galax, galax_count, width);
    // printf("\n%u\n", get_path(dists, galax[0], galax[6], width, grid_size));
    // printf("%u\n", get_path(dists, galax[2], galax[5], width, grid_size));
    // printf("%u\n", get_path(dists, galax[4], galax[8], width, grid_size));
    // printf("%u\n", get_path(dists, galax[7], galax[8], width, grid_size));
    return 0;
}
