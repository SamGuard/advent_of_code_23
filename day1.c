#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NODIG 0
#define MAX_LINE_LEN 1024

char in_buff[MAX_LINE_LEN];

char digits[9][8] = {"one", "two", "three", "four", "five",
                   "six", "seven", "eight", "nine"};
uint8_t match_dig(char *in) {
    for (uint8_t i = 0; i < 9; i++) {
        char *dig = digits[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (in[j] == '\0') {
                break;
            }
            if (dig[j] == '\0') {
                return  i + 1;
            }
            if (in[j] != dig[j]) {
                break;
            }
        }
    }
    return 0;
}

int main() {
    FILE *f = fopen("day1.in", "r");

    uint64_t sum = 0;
    uint8_t is_dig = 0;
    char first_dig = NODIG, last_dig = NODIG;
    int count = 0;
    while (fgets(in_buff, sizeof(char) * MAX_LINE_LEN, f) != NULL) {
        printf("%lu    %s", sum, in_buff);

        for (int i = 0; i < MAX_LINE_LEN; i++) {
            char c = in_buff[i];

            if (c == '\n') {
                char num[3];
                num[0] = first_dig;
                num[1] = last_dig == NODIG ? first_dig : last_dig;
                num[2] = '\0';
                // printf("num %d\n", atoi(num));
                sum += atoi(num);
                first_dig = NODIG;
                last_dig = NODIG;
                break;
            }

            is_dig = match_dig(&(in_buff[i]));
            if (is_dig != 0) {
                // printf("is_dig %i %u   ", i, is_dig);
                if (first_dig == NODIG) {
                    first_dig = '0' + is_dig;
                } else {
                    last_dig = '0' + is_dig;
                }
            } else if (c >= '0' && c <= '9') {
                if (first_dig == NODIG) {
                    first_dig = c;
                } else {
                    last_dig = c;
                }
            }
        }
    }
    printf("\n%lu\n", sum);
    return 0;
}