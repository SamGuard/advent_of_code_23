#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define MAX_NUM_LINES 256
#define MAX_LIST_SIZE 64

typedef struct Sequence {
    int64_t nums[MAX_LIST_SIZE], size;
} Sequence_t;

char* seek(char* str, char t) {
    while (*str != t && *str != '\0' && *str != '\n') {
        str++;
    }
    return str;
}

#define DELIM " \n\0"
void parse_nums(char* str, Sequence_t* seq) {
    int64_t *nums = seq->nums, *size = &(seq->size);
    char* token = strtok(str, DELIM);
    do {
        *nums = atoi(token);
        nums++;
        (*size)++;
    } while ((token = strtok(NULL, DELIM)) != NULL);
}

int64_t next_in_seq(const Sequence_t* const seq) {
    Sequence_t n_seq;
    int64_t first_elem = seq->nums[0];
    n_seq.size = seq->size - 1;
    uint8_t is_zero = 1;
    printf("\n");
    for (int i = 0; i < n_seq.size; i++) {
        n_seq.nums[i] = seq->nums[i + 1] - seq->nums[i];
        printf("%ld ", n_seq.nums[i]);
        is_zero &= n_seq.nums[i] == 0;
    }
    if (is_zero) {
        return first_elem;
    }
    int64_t v = next_in_seq(&n_seq);
    // printf("\n%ld\n", v);
    return first_elem - v;
}

int main() {
    char in_buff[MAX_LINE_LEN];
    Sequence_t seq[MAX_NUM_LINES];
    int64_t sum = 0, row = 0;

    FILE* f = fopen("in/day9.in", "r");
    if (f == NULL) {
        printf("File no open\n");
        return -1;
    }
    while (fgets(in_buff, sizeof(char) * MAX_LINE_LEN, f) != NULL) {
        parse_nums(in_buff, &(seq[row]));
        for (int64_t i = 0; i < seq[row].size; i++) {
            printf("%ld ", seq[row].nums[i]);
        }
        {
            int64_t out = next_in_seq(&(seq[row]));
            printf("= %ld\n", out);
            sum += out;
        }
        row++;
    }
    printf("\n\n%ld\n", sum);
}
