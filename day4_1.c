#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NONUM (uint64_t)(-1)
#define MAX_LINE_LEN 1024
#define WIN_NUMS 10
#define MY_NUMS 25

char *seek(char *str, char t) {
  while (*str != t && *str != '\0' && *str != '\n') {
    str++;
  }
  return str;
}

uint64_t make_num(char *start, char *end) {
    char num[64];
    uint64_t l = end - start;
    memcpy(num, start, l);
    num[l] = '\0';
    return atoll(num);
}

int main() {
  char in_buff[MAX_LINE_LEN];
  uint64_t win_nums[WIN_NUMS], my_nums[MY_NUMS];

  FILE *f = fopen("in/day4.in", "r");
  uint64_t sum = 0, row = 0;
  while (fgets(in_buff, sizeof(char) * MAX_LINE_LEN, f) != NULL) {
    // printf("%lu    %s", sum, in_buff);
    // safe memory management is for nerds
    char *cursor, *cursor_2;
    cursor = seek(in_buff, ':') + 1;
    uint8_t is_winning = 1;
    uint64_t num_cnt = 0, score = 0;
    while (1) {
        if (num_cnt == WIN_NUMS) {
            cursor += 2;
        }
        if(*(cursor + 1) == ' ') {
            cursor++;
        }
        cursor_2 = seek(cursor + 1, ' ');
        uint64_t n = make_num(cursor, cursor_2);
        if (num_cnt < WIN_NUMS) {
            win_nums[num_cnt] = n;
        } else {
            // Check if its in win_nums
            for(int i = 0; i < WIN_NUMS; i++) {
                if (win_nums[i] == n) {
                    score = score == 0 ? 1 : (score << 1);
                    break;
                }
            }
        }
        num_cnt++;
        cursor = cursor_2;
        if (num_cnt == MY_NUMS + WIN_NUMS) {
            break;
        }
    }
    sum += score;
    printf("%lu\n", sum);
  }
  return 0;
}