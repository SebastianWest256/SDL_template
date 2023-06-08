//useful mathematical functions

float randomFloat(float min, float max) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float range = max - min;
    return (random * range) + min;
}

uint32_t getColor(uint8_t red, uint8_t green, uint8_t blue) {
    return (red << 16) | (green << 8) | blue;
}

float lerp(float start, float end, float t) {
    return start + t * (end - start);
}

float map(float input, float inputMin, float inputMax, float targetMin, float targetMax) {
    // Calculate the input value's position within the input range
    float inputRange = inputMax - inputMin;
    float normalizedInput = (input - inputMin) / inputRange;

    // Map the normalized input to the target range
    float targetRange = targetMax - targetMin;
    float mappedValue = targetMin + (normalizedInput * targetRange);

    return mappedValue;
}

double logarithm(double base, double x) {
    return log(x) / log(base);
}