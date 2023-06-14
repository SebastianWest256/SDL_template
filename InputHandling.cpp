std::unordered_map<SDL_Keycode, int> key_map = {
        {SDLK_a, 0}, {SDLK_b, 1}, {SDLK_c, 2}, {SDLK_d, 3}, {SDLK_e, 4}, {SDLK_f, 5}, {SDLK_g, 6}, {SDLK_h, 7}, {SDLK_i, 8}, {SDLK_j, 9},
        {SDLK_k, 10}, {SDLK_l, 11}, {SDLK_m, 12}, {SDLK_n, 13}, {SDLK_o, 14}, {SDLK_p, 15}, {SDLK_q, 16}, {SDLK_r, 17}, {SDLK_s, 18}, {SDLK_t, 19},
        {SDLK_u, 20}, {SDLK_v, 21}, {SDLK_w, 22}, {SDLK_x, 23}, {SDLK_y, 24}, {SDLK_z, 25}, {SDLK_0, 26}, {SDLK_1, 27}, {SDLK_2, 28}, {SDLK_3, 29},
        {SDLK_4, 30}, {SDLK_5, 31}, {SDLK_6, 32}, {SDLK_7, 33}, {SDLK_8, 34}, {SDLK_9, 35}, {SDLK_SPACE, 36}, {SDLK_PERIOD, 37}
};

std::unordered_map<int, char> char_map = {
        {0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}, {4, 'E'}, {5, 'F'}, {6, 'G'}, {7, 'H'}, {8, 'I'}, {9, 'J'}, {10, 'K'}, {11, 'L'}, {12, 'M'}, {13, 'N'},
        {14, 'O'}, {15, 'P'}, {16, 'Q'}, {17, 'R'}, {18, 'S'}, {19, 'T'}, {20, 'U'}, {21, 'V'}, {22, 'W'}, {23, 'X'}, {24, 'Y'}, {25, 'Z'}, {26, '0'},
        {27, '1'}, {28, '2'}, {29, '3'}, {30, '4'}, {31, '5'}, {32, '6'}, {33, '7'}, {34, '8'}, {35, '9'}, {36, ' '}, {37, '.'}
};

struct input {
    bool mouse_pressed = false;
    int mouse_x = 0;
    int mouse_y = 0;
    std::vector<bool> key_pressed;
    std::vector<bool> key_reset;
    input() : key_pressed(38, false), key_reset(key_pressed.size(), true) {}
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

class textbox {
public:

    int active_char = 0;
    int x, y;
    int x_bound, y_bound;
    int text_size;
    int max_chars;
    Uint32 text_color;
    Uint32 box_color;
    Uint32 border_color;

    std::string text;

    void init(int x_in, int y_in, int text_size_in, int max_chars_in, Uint32 text_color_in, Uint32 box_color_in, Uint32 border_color_in){

        x = x_in;
        y = y_in;
        text_size = text_size_in;
        text_color = text_color_in;
        box_color = box_color_in;
        border_color = border_color_in;
        max_chars = max_chars_in;

        y_bound = y + text_size * 9;
        x_bound = x + text_size * (max_chars * 6 + 3);

    }

    void draw(SDL_Surface* surface) {

        draw_rectangle(surface, x, y, x_bound - x, y_bound - y + text_size * 2, border_color);
        draw_rectangle(surface, x + text_size, y + text_size, x_bound - x - text_size * 2, y_bound - y - text_size * 2 + text_size * 2, box_color);

        for (int i = 0; i < text.length(); i++) {

            draw_char(surface, text[i], x + text_size * 2 + text_size * 6 * i, y + text_size * 2, text_size, text_color);

        }

    }

    void draw_active(SDL_Surface* surface) {

        draw_rectangle(surface, x + text_size * 2 + text_size * 6 * active_char, y_bound, text_size * 5, text_size, border_color);

    }

};

void handle_textboxes(SDL_Surface* surface, std::vector<textbox>* textbox, int* active_textbox, bool* using_textbox, input* current_input) {

    for (int i = 0; i < (*textbox).size(); i++) {
        (*textbox)[i].draw(surface);
    }

    if ((*current_input).mouse_pressed) {
        *using_textbox = false;
        for (int i = 0; i < (*textbox).size(); i++) {
            if ((*current_input).mouse_x > (*textbox)[i].x && (*current_input).mouse_x < (*textbox)[i].x_bound &&
                (*current_input).mouse_y >(*textbox)[i].y && (*current_input).mouse_y < (*textbox)[i].y_bound) {
                if (i != *active_textbox) {
                    *active_textbox = i;
                    (*textbox)[*active_textbox].text = "";
                    (*textbox)[*active_textbox].active_char = 0;

                }
                *using_textbox = true;
            }
        }
    }

    if (!*using_textbox) {
        *active_textbox = -1;
    }

    if (*active_textbox >= 0) {
        if ((*textbox)[*active_textbox].active_char < (*textbox)[*active_textbox].max_chars) {
            (*textbox)[*active_textbox].draw_active(surface);
        }
    }

    draw_char(surface, input_to_char(*current_input), 100, 100, 10, 0x666666);

    if (char_to_int(input_to_char(*current_input)) != -1) {
        if ((*current_input).key_reset[char_to_int(input_to_char(*current_input))]) {
            std::cout << input_to_char(*current_input) << std::endl;
            if (*active_textbox >= 0) {
                if ((*textbox)[*active_textbox].active_char < (*textbox)[*active_textbox].max_chars) {
                    (*textbox)[*active_textbox].text += input_to_char(*current_input);
                    (*textbox)[*active_textbox].active_char++;
                }
            }
            (*current_input).key_reset[char_to_int(input_to_char(*current_input))] = false;
        }
    }

}