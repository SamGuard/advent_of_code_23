#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define DELIM ",\n\r"

typedef struct HashEntry {
    uint32_t power;
    uint32_t box;
    char *str;
    struct HashEntry *next;
} HashEntry_t;

typedef struct HashMap {
    HashEntry_t *focus[256];
} HashMap_t;

HashMap_t map;

uint32_t hash(char *str) {
    uint32_t len = strlen(str);
    uint8_t val = 0;
    for (uint32_t i = 0; i < len; i++) {
        val += str[i];
        val *= 17;
    }
    return val;
}

void hash_new(char *str, uint32_t power) {
    uint8_t h = hash(str);
    HashEntry_t *entry = map.focus[h], *prev = NULL;
    uint32_t box = h;
    uint8_t str_match = 0;
    while (entry != NULL) {
        box = entry->box;
        str_match = strcmp(entry->str, str) == 0;
        if (str_match != 0) {
            break;
        }
        prev = entry;
        entry = entry->next;
    }

    if (str_match) {  // Label already exists
        entry->power = power;
        return;
    }

    entry = (HashEntry_t *)malloc(sizeof(HashEntry_t));
    entry->power = power;
    entry->box = box;
    entry->str = (char *)malloc(sizeof(char) * strlen(str));
    strcpy(entry->str, str);
    entry->next = NULL;

    if (prev == NULL) {
        map.focus[h] = entry;
    } else {
        prev->next = entry;
    }

    printf("Added %s %u\n", entry->str, entry->power);
}

void hash_pop(char *str) {
    uint8_t h = hash(str);
    HashEntry_t *entry = map.focus[h], *prev = NULL;

    while (entry != NULL && strcmp(str, entry->str) != 0) {
        prev = entry;
        entry = entry->next;
    }
    if (entry == NULL) {
        return;
    }

    if (prev == NULL) {
        map.focus[h] = entry->next;
    } else {
        prev->next = entry->next;
    }
    printf("Removed %s %u\n", entry->str, entry->power);
    free(entry->str);
    free(entry);
}

void hash_print() {
    for (uint32_t i = 0; i < 256; i++) {
        if (map.focus[i] == NULL) {
            continue;
        }
        HashEntry_t *entry = map.focus[i];
        printf("Box %u:", entry->box);
        while (entry != NULL) {
            printf(" \"%s %u\"", entry->str, entry->power);
            entry = entry->next;
        }
        printf("\n");
    }
}

int main() {
    memset(&map, 0, sizeof(HashMap_t));
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
    char label[32];
    do {
        uint32_t tok_len = strlen(tok);
        if (tok[tok_len - 2] == '=') {
            memcpy(label, tok,
                   sizeof(char) *
                       (tok_len - 2));  // take of last two chars as theyre "=?"
            label[tok_len - 2] = '\0';
            hash_new(label, (tok[tok_len - 1]) - '0');
        } else {
            memcpy(label, tok,
                   sizeof(char) *
                       (tok_len - 1));  // take of last two chars as theyre "=?"
            label[tok_len - 1] = '\0';
            hash_pop(label);
        }
        printf("---\n");
        hash_print();
        printf("---\n");
    } while ((tok = strtok(NULL, DELIM)) != NULL);

    for (uint32_t i = 0; i < 256; i++) {
        if (map.focus[i] == NULL) {
            continue;
        }
        HashEntry_t *entry = map.focus[i];
        uint32_t slot = 1;
        while (entry != NULL) {
            sum += (i + 1) * slot * entry->power;
            entry = entry->next;
            slot++;
        }
    }

    printf("\nSum: %u\n", sum);

    return 0;
}
