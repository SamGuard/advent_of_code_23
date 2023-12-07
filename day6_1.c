#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NONUM (uint64_t)(-1)
#define MAX_LINE_LEN 1024
#define NUM_THINGS 1

#define MAX_LIST_SIZE 64
typedef struct uint_list {
    uint32_t size;
    uint64_t list[MAX_LIST_SIZE];
} uint_list_t;

uint8_t is_num(char c) {
    if (c >= '0' && c <= '9') {
        return 1;
    }
    return 0;
}

#define DELIM " \n\0"
uint_list_t* parse_nums(char* str) {
    uint_list_t* list = (uint_list_t*)malloc(sizeof(uint_list_t));
    uint64_t* nums = list->list;
    uint32_t* size = &(list->size);
    char* token = strtok(str, DELIM);
    do {
        *nums = atol(token);
        nums++;
        (*size)++;
    } while ((token = strtok(NULL, DELIM)) != NULL);
    return list;
}

double calc_time(double t_total, double d_b) {
    // d = t_h * (t_t - t_h) - d_b
    // 0 = -t_h^2 + t_t*t_h - d_b
    // a = -1 b = t_t c = -d_b
    // x = (-t_t +- dt^0.5) / -2
    // dt = (t_t^2 - 4d_b)^0.5
    double dt = (t_total * t_total - 4 * d_b);
    if (dt < 0) {
        return __DBL_MAX__;
    }
    dt = sqrt(dt);
    double x_1 = (-t_total + dt) / -2.0f;
    double x_2 = (-t_total - dt) / -2.0f;
    printf("total:%f d_b:%f dt:%f x1:%f x2:%f\n", t_total, d_b, dt, x_1, x_2);

    double r_x_1 = ceilf(x_1);
    double r_x_2 = floorf(x_2);
    if (x_1 == r_x_1) {
        r_x_1++;
    }
    if (x_2 == r_x_2) {
        r_x_2--;
    }
    return 1 + r_x_2 - r_x_1;
}

uint64_t calc_time_brute(uint64_t t, uint64_t d) {
    uint64_t count = 0;
    for (uint64_t i = 1; i < UINT64_MAX; i++){
        if (i * (t - i) > d) {
            count++;
        } else if (count > 0) {
            return count;
        }
    }
    return count;
}

int main() {
    char in_buff[MAX_LINE_LEN];
    // double times[NUM_THINGS] = { 71530 };
    // double dst[NUM_THINGS] = { 940200 };
    // double times[NUM_THINGS] = { 54946592.0f };
    // double dst[NUM_THINGS] = { 302147610291404.0f };
    uint64_t times[NUM_THINGS] = { 54946592 };
    uint64_t dst[NUM_THINGS] = { 302147610291404 };

    uint64_t sum = 1;
    for (uint32_t i = 0; i < NUM_THINGS; i++) {
        //double res = calc_time(times[i], dst[i]);
        // sum *= res;
        // printf("%lu %f\n", sum, res);
        printf("%lu \n", calc_time_brute(times[i], dst[i]));
    }
    return 0;
}