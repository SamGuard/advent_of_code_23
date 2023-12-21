#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024

typedef struct PartList {
    char springs[64];
    uint8_t seq[32];
    uint32_t num_springs, num_seq;
} PartList;

uint8_t is_good(PartList *part) {
    uint32_t curr_seq = 0, i = 0;
    while (1) {
        while (part->springs[i] == '.') {
            i++;
            if (i == part->num_springs) {
                return part->num_seq == curr_seq;
            }
        }
        if (part->num_seq == curr_seq) {
            return 0;
        }
        uint8_t seq_val = part->seq[curr_seq];
        while (part->springs[i] == '#') {
            seq_val--;
            i++;
            if (i == part->num_springs) {
                break;
            }
        }
        if (seq_val != 0) {
            return 0;
        }
        curr_seq++;
        if (i == part->num_springs) {
            break;
        }
    }
    return part->num_seq == curr_seq && part->num_springs == i;
}

uint64_t do_iterations(PartList *part, uint32_t index) {
    // printf("%u: %s\n", index, part->springs);
    uint64_t iters = 0;
    uint32_t i;
    for (i = index; i < part->num_springs; i++) {
        if (part->springs[i] == '?') {
            PartList *new = (PartList *)malloc(sizeof(PartList));
            memcpy(new, part, sizeof(PartList));
            new->springs[i] = '#';
            iters += do_iterations(new, i + 1);
            new->springs[i] = '.';
            iters += do_iterations(new, i + 1);
            break;
        }
    }
    if (part->num_springs == i) {
        uint8_t good = is_good(part);
        // printf("%u: %s %u\n", index, part->springs, good);
        return good;
    }
    return iters;
}

#define DELIM ",\n\0"
uint32_t process_seq(char *str, uint8_t *out) {
    uint32_t len = 0;
    char *token = strtok(str, DELIM);
    do {
        out[len] = atol(token);
        len++;
    } while ((token = strtok(NULL, DELIM)) != NULL);
    return len;
}

int main() {
    char in_buff[MAX_LINE_LEN];
    char in_seq[64];
    uint64_t sum = 0;

    FILE *f = fopen("in/day12.in", "r");
    if (f == NULL) {
        printf("File no open\n");
        return -1;
    }

    while (fgets(in_buff, sizeof(char) * MAX_LINE_LEN, f) != NULL) {
        PartList s;
        sscanf(in_buff, "%s %s", s.springs, in_seq);
        s.num_seq = process_seq(in_seq, s.seq);
        s.num_springs = strlen(s.springs);  // -1 For \0
        printf("%s %u %u \n", s.springs, s.num_springs, s.num_seq);
        uint64_t iters = do_iterations(&s, 0);
        sum += iters;
        printf("%lu, %lu\n", sum, iters);
        // return 0;
    }
    return 0;
}
