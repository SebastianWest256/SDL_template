//useful mathematical functions

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

struct input {
    bool mouse_pressed = false;
    int mouse_x = 0;
    int mouse_y = 0;
    std::vector<bool> key_pressed;
    std::vector<bool> key_reset;
    input() : key_pressed(38, false), key_reset(key_pressed.size(), true) {}
};

std::unordered_map<int, char> char_map = {
        {0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}, {4, 'E'}, {5, 'F'}, {6, 'G'}, {7, 'H'}, {8, 'I'}, {9, 'J'}, {10, 'K'}, {11, 'L'}, {12, 'M'}, {13, 'N'}, 
        {14, 'O'}, {15, 'P'}, {16, 'Q'}, {17, 'R'}, {18, 'S'}, {19, 'T'}, {20, 'U'}, {21, 'V'}, {22, 'W'}, {23, 'X'}, {24, 'Y'}, {25, 'Z'}, {26, '0'}, 
        {27, '1'}, {28, '2'}, {29, '3'}, {30, '4'}, {31, '5'}, {32, '6'}, {33, '7'}, {34, '8'}, {35, '9'}, {36, ' '}, {37, '.'}
};

char input_to_char(input input) {
    char c;
    for (int i = 0; i < input.key_pressed.size(); i++) {

        if (input.key_pressed[i]) {
            auto it = char_map.find(i);
            if (it != char_map.end()) {
                c = it->second;
                return c;
            }
        }

    }
    return NULL;

}

int char_to_int(char c) {
    for (const auto& pair : char_map) {
        if (pair.second == c) {
            return pair.first;
        }
    }
    return -1; 
}
