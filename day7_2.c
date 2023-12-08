#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NONUM (uint64_t)(-1)
#define MAX_LINE_LEN 1024
#define H_SIZE 5

#define MAX_LIST_SIZE 64
typedef struct uint_list {
    uint32_t size;
    uint64_t list[MAX_LIST_SIZE];
} uint_list_t;

typedef struct Hand {
    char cards[H_SIZE];
    uint64_t bid;
    uint8_t base;
} Hand_t;

uint8_t is_num(char c) {
    if (c >= '0' && c <= '9') {
        return 1;
    }
    return 0;
}

void print_hand(const char* const h) {
    for (int i = 0; i < H_SIZE; i++) {
        printf("%c", h[i]);
    }
}

uint8_t card_value(const char c) {
    if (is_num(c)) {
        return c - '0';
    }
    switch (c)
    {
    case 'J': return 1; // JOKER NOW BAD
    case 'T': return 10;
    case 'Q': return 11;
    case 'K': return 12;
    case 'A': return 13;
    default:
        break;
    }
    printf("ogiubwgIUPGNOGNIPOWIGNWOIGNOGINWLKEGNWEOGNEWGOI\n\n\n\n\n\n");
    return -1;
}

char val_to_char(const uint8_t val) {
    switch (val)
    {
    case 1:  return 'J'; // JOKER NOW BAD
    case 10: return 'T';
    case 11: return 'Q';
    case 12: return 'K';
    case 13: return 'A';
    default:
        break;
    }
    return '0' + val;
}

uint8_t base_hand_score(char* h) {
    uint8_t count[13];
    uint8_t triple = 0, pairs = 0;

    memset(count, 0, sizeof(uint8_t) * 13);
    for (int i = 0; i < H_SIZE; i++) {
        uint8_t v = card_value(h[i]);
        count[v - 1]++;
    }
    for (int i = 0; i < 13; i++) {
        if (count[i] == 5) {
            return -1; // Five kind
        }
        if (count[i] == 4) {
            return -2; // Four kind
        }
        else if (count[i] == 3) {
            triple++;
        }
        else if (count[i] == 2) {
            pairs++;
        }
    }
    if (triple == 1) {
        if (pairs == 1) {
            return -3; // Full house
        }
        return -4; // Three kind
    }
    if (pairs == 2) {
        return -5; // Two pair
    }
    if (pairs == 1) {
        return -6; // One pair
    }
    return -7; // High card
}

uint8_t new_base_hand_score(char* h, uint8_t index) {
    if (index == H_SIZE) {
        return base_hand_score(h);
    }
    for (int i = index; i < H_SIZE; i++) {
        if (h[i] != 'J') {
            continue;
        }
        char new_h[H_SIZE];
        memcpy(new_h, h, H_SIZE * sizeof(char));
        uint8_t best = 0;
        for (int j = 1; j < 14; j++) {
            new_h[i] = val_to_char(j);
            uint8_t tmp_best = new_base_hand_score(new_h, index + 1);
            best = best < tmp_best ? tmp_best : best;
            // if (best == 50) {}
        }
        return best;
    }
    return base_hand_score(h);
}

int cmp_card(const void* elem1, const void* elem2) {
    char a = *((char*)elem1);
    char b = *((char*)elem2);
    uint8_t a_n = card_value(a);
    uint8_t b_n = card_value(b);
    if (a_n > b_n) return  1;
    if (a_n < b_n) return -1;
    return 0 / 0;
}

int cmp_hand(const void* elem1, const void* elem2) {
    Hand_t a = *((Hand_t*)elem1);
    Hand_t b = *((Hand_t*)elem2);
    uint8_t a_n = a.base;
    uint8_t b_n = b.base;

    if (a_n > b_n) return  1;
    if (a_n < b_n) return -1;

    for (int i = 0; i < H_SIZE; i++) {
        uint8_t a_c = card_value(a.cards[i]);
        uint8_t b_c = card_value(b.cards[i]);
        if (a_c == b_c) {
            continue;
        }
        if (a_c > b_c) {
            return 1;
        }
        return -1;
    }

    print_hand(a.cards);
    printf("    ");
    print_hand(b.cards);
    printf("\n");
    return 0;
}

void sort_hand(char* const h) {
    qsort(h, H_SIZE, sizeof(char), cmp_card);
}

void rank_hands(Hand_t* h, uint64_t num_hands) {
    qsort(h, num_hands, sizeof(Hand_t), cmp_hand);
}

int main() {
    char in_buff[MAX_LINE_LEN];
    Hand_t hands[1000];
    FILE* f = fopen("in/day7.in", "r");
    uint64_t row = 0;
    while (fgets(in_buff, sizeof(char) * MAX_LINE_LEN, f) != NULL) {
        memcpy(hands[row].cards, in_buff, sizeof(char) * H_SIZE);
        hands[row].bid = atol(in_buff + H_SIZE);
        hands[row].base = new_base_hand_score(hands[row].cards, 0);
        // sort_hand(hands[row].cards);
        // print_hand(hands[row].cards);
        // printf("    %lu %u\n", hands[row].bid, hands[row].base);
        row++;
    }
    //printf("\n\n");
    rank_hands(hands, row);
    uint64_t sum = 0, score;
    // for (int i = row - 1; i >= 0; i--) {
    for (int i = 0; i < row; i++) {
        print_hand(hands[i].cards);
        score = hands[i].bid * (i + 1);
        sum += score;
        // printf("    %lu %lu %u %lu\n", hands[i].bid, score, hands[i].base, sum);
        printf("  %u %lu %lu %lu\n", hands[i].base, hands[i].bid, score, sum);
        // printf("%lu\n", hands[i].bid);
    }
    printf("\n%lu\n", sum);
    return 0;
}