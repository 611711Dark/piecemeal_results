#include "display.h"
#include "drawings.h"
#include <stdio.h>
#include <system.h>
void rectangle(void) {
    Filled_rectangle(0, 19, 0, 99, GRAPHICS_COLOR_ORANGE);
}

void circle(void) {
    Filled_circle(50, 50, 20, GRAPHICS_COLOR_GREEN);
}

void write_str(uint8_t num) {
    if (num < '0' || num > '9') {
        write_string("Invalid exercise number", 1, 105, GRAPHICS_COLOR_BLACK);
        return;
    }
    uint16_t exercise_num = num - '0';

    uint8_t message[25];
    sprintf((char*)message, "Part 3, Exercise 2%d", exercise_num);

    write_string((uint8_t*)message, 1, 105, GRAPHICS_COLOR_BLACK);
    write_string("has been completed!", 1, 115, GRAPHICS_COLOR_BLACK);
}

void write_num_str(int16_t num,int8_t offset) {
    uint8_t buf[7];
    itoa_7_null_terminated(num, buf);
    write_string(buf, 50, 40 + offset, GRAPHICS_COLOR_BLACK);
}

void write_voltage_str(float voltage, int8_t offset) {
    uint8_t str[8]; // 足够存储 "xx.xxV\0"
    
    // 将浮点电压转换为整数部分和小数部分
    uint16_t integer_part = (uint16_t)voltage;
    uint16_t decimal_part = (uint16_t)((voltage - integer_part) * 100);
    
    // 手动构建字符串
    str[0] = '0' + (integer_part / 10);    // 十位
    str[1] = '0' + (integer_part % 10);    // 个位
    str[2] = '.';
    str[3] = '0' + (decimal_part / 10);    // 十分位
    str[4] = '0' + (decimal_part % 10);    // 百分位
    str[5] = 'V';
    str[6] = '\0';
    
    write_string(str, 50, 40 + offset, GRAPHICS_COLOR_BLACK);
}

int8_t clear_drawing(uint8_t fig_number) {
    switch(fig_number) {
        case 1: 
            Filled_rectangle(0, 19, 0, 99, GRAPHICS_COLOR_WHITE);
            return 0;
        case 2: 
            Filled_circle(50, 50, 20, GRAPHICS_COLOR_WHITE);
            return 0;
        case 3: 
            Filled_rectangle(50, 70, 40, 46, GRAPHICS_COLOR_WHITE);
            return 0;
        default:
            write_string("Error", 60, 60, GRAPHICS_COLOR_RED);
            return -1;
    }
}
