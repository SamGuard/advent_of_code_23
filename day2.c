#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NODIG 0
#define MAX_LINE_LEN 1024

char in_buff[MAX_LINE_LEN];
char num_buff[MAX_LINE_LEN];

char* seek(char* str, char t) {
    while (*str != t && *str != '\0' && *str != '\n' && *str != ';' && *str != ',') {
        str++;
    }
    return str;
}


char colours[3][8] = {"red", "green", "blue"};
uint8_t get_colour(char *str) {
    for (int i = 0; i < 3; i++) {
        char *col = colours[i];
        uint8_t is_good = 0;
        for (int j = 0; j < 8; j++) {
            if (col[j] == '\0') {
                is_good = 1;
                break;
            }
            if (str[j] == '\0' || str[j] != col[j]) {
                break;
            }
        }
        if (is_good) {
            return i;
        }
    }
    return 3;
}

int main() {
    FILE* f = fopen("day2.in", "r");
    uint64_t sum = 0, game_id = 1, colours_cnt[3], max_col_cnt[3];
    char* cursor;
    while (fgets(in_buff, sizeof(char) * MAX_LINE_LEN, f) != NULL) {
        printf("%lu    %s", sum, in_buff);

        cursor = seek(in_buff, ':');
        cursor += 2;
        colours_cnt[0] = colours_cnt[1] = colours_cnt[2] = 0;
        max_col_cnt[0] = max_col_cnt[1] = max_col_cnt[2] = 0;
        while (1) {
            uint64_t num, str_len;
            char* cursor_2 = seek(cursor, ' ');
            // janky copy string
            str_len = (cursor_2 - cursor); 
            memcpy(num_buff, cursor, str_len);
            num_buff[str_len] = '\0';
            num = atoll(num_buff);

            cursor = cursor_2 + 1;

            cursor_2 = seek(cursor, 0); // go until dont
            str_len = (cursor_2 - cursor); // dont include separator
            memcpy(num_buff, cursor, str_len);
            num_buff[str_len] = '\0';
            colours_cnt[get_colour(num_buff)] += num;
            if (*cursor_2 == ';' || *cursor_2 == '\n') {
                if (colours_cnt[0] > 12 || colours_cnt[1] > 13 || colours_cnt[2] > 14) {
                    // for first star
                    // game_id++;
                    // break;
                }
                // im too lazy for a loop
                max_col_cnt[0] = max_col_cnt[0] < colours_cnt[0] ? colours_cnt[0] : max_col_cnt[0];
                max_col_cnt[1] = max_col_cnt[1] < colours_cnt[1] ? colours_cnt[1] : max_col_cnt[1];
                max_col_cnt[2] = max_col_cnt[2] < colours_cnt[2] ? colours_cnt[2] : max_col_cnt[2];

                if (*cursor_2 == '\n') {
                    // sum for first star
                    // sum += game_id;
                    // -------
                    // sum for second star
                    sum += max_col_cnt[0] * max_col_cnt[1] * max_col_cnt[2];
                    game_id++;
                    break;
                }
                colours_cnt[0] = colours_cnt[1] = colours_cnt[2] = 0;
            }
            cursor = cursor_2 + 2;
        }
    }
    
    printf("\n%lu\n", sum);
    return 0;
}