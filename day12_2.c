#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024

typedef struct PartList {
    char springs[256];
    uint8_t seq[128];
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

uint64_t iter_count = 0;
uint64_t do_iterations(PartList *part, uint32_t index, uint32_t curr_seq,
                       uint32_t seq_val, uint8_t in_hash) {
    uint64_t iters = 0;
    uint32_t i;
    // printf("%u: %s \n", index, part->springs);
    if (index == 0) {
        curr_seq = 0;
        seq_val = part->seq[curr_seq];
    }

    int32_t total_left = 0;
    if (index != part->num_springs && index > part->num_springs / 5 && part->num_seq != curr_seq) {
        // Calc minumum number of spaces needed to finish sequence
        for (i = curr_seq + 1; i < part->num_seq; i++) {
            total_left += part->seq[i];
        }
        total_left += seq_val + part->num_seq - curr_seq - 1;
        if ((int32_t)(part->num_springs - index) < total_left) {
            return 0;
        }
    }

    for (i = index; i < part->num_springs; i++) {
        if (part->springs[i] == '#') {
            if (seq_val == 0) {
                return 0;
            }
            in_hash = 1;
            seq_val--;
        } else if (part->springs[i] == '.') {
            if (in_hash == 1) {
                in_hash = 0;
                curr_seq++;
                if (seq_val != 0) {
                    return 0;
                }
                if (curr_seq < part->num_seq) {
                    seq_val = part->seq[curr_seq];
                } else {
                    seq_val = 0;
                }
            }
        } else if (part->springs[i] == '?') {
            PartList *new = (PartList *)malloc(sizeof(PartList));
            memcpy(new, part, sizeof(PartList));
            if (seq_val > 0 || curr_seq < part->num_seq - 1) {
                new->springs[i] = '#';
                iters += do_iterations(new, i, curr_seq, seq_val, in_hash);
            }
            new->springs[i] = '.';
            iters += do_iterations(new, i, curr_seq, seq_val, in_hash);
            free(new);
            break;
        }
    }
    if (part->num_springs == i) {
        // printf("%u: %s \n", index, part->springs);
        // uint8_t good = is_good(part);
        if (iter_count % 10000000 == 0) {
            printf("%u: %s num_seq:%u curr_seq:%u seq_val:%u\n", index, part->springs, part->num_seq, curr_seq, seq_val);
            printf("%d %u\n", total_left, part->num_springs);
        }
        iter_count++;
        return (part->num_seq == curr_seq ||
                (part->num_seq == curr_seq + 1 && seq_val == 0));
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

void convert_part(PartList *part) {
    for (uint32_t i = 1; i < 5; i++) {
        memcpy(part->seq + i * part->num_seq, part->seq,
               sizeof(uint8_t) * part->num_seq);
        memcpy(part->springs + i * (part->num_springs + 1), part->springs,
               sizeof(char) * part->num_springs);
        part->springs[i * (part->num_springs + 1) - 1] = '?';
    }
    part->num_seq *= 5;
    part->num_springs = part->num_springs * 5 + 4;
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
        convert_part(&s);
        printf("%s %u %u \n", s.springs, s.num_springs, s.num_seq);
        uint64_t iters = do_iterations(&s, 0, 0, 0, 0);
        sum += iters;
        printf("%lu, %lu\n", sum, iters);
        // return 0;
    }
    return 0;
}
