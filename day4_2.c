#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NONUM (uint64_t)(-1)
#define MAX_LINE_LEN 1024
#define WIN_NUMS 10 // 5
#define MY_NUMS 25  // 8
#define CARDS 190 // 6

uint64_t win_nums[CARDS][WIN_NUMS], my_nums[CARDS][MY_NUMS];

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

uint64_t score_cards(uint32_t card_num, uint32_t depth) {
  uint64_t *win = win_nums[card_num], *my = my_nums[card_num], count = 0,
           score = 1;

  if (card_num >= CARDS) {
    printf("ERROR ERRROR %u\n", card_num + 1);
    return -1;
  }

  for (uint64_t m = 0; m < MY_NUMS; m++) {
    // printf("%lu ", my_nums[card_num][m]);
    for (uint64_t w = 0; w < WIN_NUMS; w++) {
      if (my_nums[card_num][m] == win_nums[card_num][w]) {
        count++;
        break;
      }
    }
  }
  // printf("\n");
  // for (uint32_t i = 0; i < depth; i++) {
  //   printf("    ");
  // }
  // printf("Card: %u Count: %lu\n", card_num + 1, count);
  for (uint64_t i = 0; i < count; i++) {
    score += score_cards(card_num + i + 1, depth + 1);
  }
  // for (uint32_t i = 0; i < depth; i++) {
  //   printf("    ");
  // }
  // printf("Card: %u Score: %lu\n", card_num + 1, score);
  return score;
}

int main() {
  char in_buff[MAX_LINE_LEN];

  FILE *f = fopen("in/day4.in", "r");
  uint64_t sum = 0, row = 0;
  while (fgets(in_buff, sizeof(char) * MAX_LINE_LEN, f) != NULL) {
    // printf("%lu    %s", sum, in_buff);
    // safe memory management is for nerds
    char *cursor, *cursor_2;
    cursor = seek(in_buff, ':') + 1;
    uint64_t num_cnt = 0;
    while (1) {
      if (num_cnt == WIN_NUMS) {
        cursor += 2;
      }
      if (*(cursor + 1) == ' ') {
        cursor++;
      }
      cursor_2 = seek(cursor + 1, ' ');
      uint64_t n = make_num(cursor, cursor_2);
      // printf("%lu \n", n);
      if (num_cnt < WIN_NUMS) {
        win_nums[row][num_cnt] = n;
      } else {
        my_nums[row][num_cnt - WIN_NUMS] = n;
      }
      num_cnt++;
      cursor = cursor_2;
      if (num_cnt == MY_NUMS + WIN_NUMS) {
        row++;
        break;
      }
    }
  }

  for (uint64_t i = 0; i < CARDS; i++) {
    sum += score_cards(i, 0);
  }
  printf("\n%lu\n", sum);
  return 0;
}