//useful mathematical functions

#include <cstdlib>
#include <stdlib.h>
#include <cstdint>

float random_float(float min, float max) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float range = max - min;
    return (random * range) + min;
}

uint32_t get_color(uint8_t red, uint8_t green, uint8_t blue) {
    return (red << 16) | (green << 8) | blue;
}

struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

color get_RGB(uint32_t hexColor) {
    color color;

    color.r = (hexColor & 0xFF0000) >> 16;
    color.g = (hexColor & 0x00FF00) >> 8;
    color.b = hexColor & 0x0000FF;

    return color;
}

float lerp(float start, float end, float t) {
    return start + t * (end - start);
}

float map(float input, float inputMin, float inputMax, float targetMin, float targetMax) {

    float inputRange = inputMax - inputMin;
    float normalizedInput = (input - inputMin) / inputRange;

    float targetRange = targetMax - targetMin;
    float mappedValue = targetMin + (normalizedInput * targetRange);

    return mappedValue;
}

double logarithm(double base, double x) {
    return log(x) / log(base);
}


