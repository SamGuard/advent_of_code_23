#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define DELIM ",\n\r"

uint32_t hash(char *str) {
    uint32_t len = strlen(str);
    uint8_t val = 0;
    for (uint32_t i = 0; i < len; i++) {
        val += str[i];
        val *= 17;
    }
    return val;
}

int main() {
    char in_buff[MAX_LINE_LEN];
    char *input = (char *)malloc(sizeof(char));
    *input = '\0';
    uint32_t in_size = 0;

    FILE *f = fopen("in/day15.in", "r");
    if (f == NULL) {
        printf("File no open\n");
        return -1;
    }
    while (fgets(in_buff, sizeof(char) * MAX_LINE_LEN, f) != NULL) {
        in_size += MAX_LINE_LEN;
        input = (char *)realloc(input, sizeof(char) * in_size);
        strcat(input, in_buff);
    }

    char *tok = strtok(input, DELIM);
    uint32_t sum = 0;
    do {
        uint32_t v = hash(tok);
        printf("%s %u\n", tok, v);
        sum += v;
    } while ((tok = strtok(NULL, DELIM)) != NULL);

    printf("\nSum: %u\n", sum);

    return 0;
}
