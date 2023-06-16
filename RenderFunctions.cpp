//useful functions for drawing various shapes

#include <SDL_gesture.h>
#include <corecrt_math.h>
#include <xhash>
#include <string>
#include <optional>
#include <optional>



void setPixelColor(SDL_Surface* surface, int x, int y, Uint32 color) {

    Uint32* pixels = (Uint32*)surface->pixels;
    pixels[y * surface->w + x] = color;
}

void draw_line(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        Uint32* pixels = (Uint32*)surface->pixels;
        pixels[y1 * surface->w + x1] = color;
        if (x1 == x2 && y1 == y2) break;
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void draw_thick_line(SDL_Surface* surface, int x1, int y1, int x2, int y2, int thickness, Uint32 color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        for (int i = -thickness / 2; i <= thickness / 2; i++) {
            for (int j = -thickness / 2; j <= thickness / 2; j++) {
                int draw_x = x1 + i;
                int draw_y = y1 + j;
                if (draw_x >= 0 && draw_x < surface->w && draw_y >= 0 && draw_y < surface->h) {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[draw_y * surface->w + draw_x] = color;
                }
            }
        }
        if (x1 == x2 && y1 == y2) break;
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void draw_square(SDL_Surface* surface, int x, int y, int sideLength, Uint32 color) {
    SDL_Rect square;
    square.x = x;
    square.y = y;
    square.w = sideLength;
    square.h = sideLength;
    SDL_FillRect(surface, &square, color);
}

void draw_square_outline(SDL_Surface* surface, int x, int y, int side, int thickness, Uint32 color) {
    for (int i = 0; i < side; i++) {
        for (int j = 0; j < thickness; j++) {
            if (x + i >= 0 && x + i < surface->w && y + j >= 0 && y + j < surface->h) {
                Uint32* pixels = (Uint32*)surface->pixels;
                pixels[(y + j) * surface->w + (x + i)] = color;
                pixels[(y + side - j - 1) * surface->w + (x + i)] = color;
            }
        }
    }
    for (int i = 0; i < thickness; i++) {
        for (int j = 0; j < side; j++) {
            if (x + i >= 0 && x + i < surface->w && y + j >= 0 && y + j < surface->h) {
                Uint32* pixels = (Uint32*)surface->pixels;
                pixels[(y + j) * surface->w + (x + i)] = color;
                pixels[(y + j) * surface->w + (x + side - i - 1)] = color;
            }
        }
    }
}

//misses pixels
void draw_rotated_square(SDL_Surface* surface, int x, int y, int side, float angle, Uint32 color)
{
    float c = cos(angle);
    float s = sin(angle);
    int half_side = side / 2;
    for (int i = -half_side; i <= half_side; i++)
    {
        for (int j = -half_side; j <= half_side; j++)
        {
            int new_x = (int)(x + (i * c - j * s));
            int new_y = (int)(y + (i * s + j * c));
            if (new_x >= 0 && new_x < surface->w && new_y >= 0 && new_y < surface->h)
            {
                Uint32* pixels = (Uint32*)surface->pixels;
                pixels[new_y * surface->w + new_x] = color;
            }
        }
    }
}

void draw_rectangle(SDL_Surface* surface, int x, int y, int w, int h, Uint32 color) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_FillRect(surface, &rect, color);
}

void draw_rectangle_outline(SDL_Surface* surface, int x, int y, int w, int h, int thickness, Uint32 color) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < thickness; j++) {
            if (x + i >= 0 && x + i < surface->w && y + j >= 0 && y + j < surface->h) {
                Uint32* pixels = (Uint32*)surface->pixels;
                pixels[(y + j) * surface->w + (x + i)] = color;
            }
            if (x + i >= 0 && x + i < surface->w && y + h - j - 1 >= 0 && y + h - j - 1 < surface->h) {
                Uint32* pixels = (Uint32*)surface->pixels;
                pixels[(y + h - j - 1) * surface->w + (x + i)] = color;
            }
        }
    }

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < thickness; j++) {
            if (x + j >= 0 && x + j < surface->w && y + i >= 0 && y + i < surface->h) {
                Uint32* pixels = (Uint32*)surface->pixels;
                pixels[(y + i) * surface->w + (x + j)] = color;
            }
            if (x + w - j - 1 >= 0 && x + w - j - 1 < surface->w && y + i >= 0 && y + i < surface->h) {
                Uint32* pixels = (Uint32*)surface->pixels;
                pixels[(y + i) * surface->w + (x + w - j - 1)] = color;
            }
        }
    }
}

//misses pixels
void draw_rotated_rectangle(SDL_Surface* surface, int x, int y, int w, int h, float angle, Uint32 color) {
    float c = cos(angle);
    float s = sin(angle);
    int half_w = w / 2;
    int half_h = h / 2;
    for (int i = -half_w; i <= half_w; i++) {
        for (int j = -half_h; j <= half_h; j++) {
            int new_x = (int)(x + (i * c - j * s));
            int new_y = (int)(y + (i * s + j * c));
            if (new_x >= 0 && new_x < surface->w && new_y >= 0 && new_y < surface->h) {
                Uint32* pixels = (Uint32*)surface->pixels;
                pixels[new_y * surface->w + new_x] = color;
            }
        }
    }
}

void draw_circle(SDL_Surface* surface, int x, int y, int radius, Uint32 color) {
    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            if (i * i + j * j <= radius * radius) {
                int draw_x = x + i;
                int draw_y = y + j;
                if (draw_x >= 0 && draw_x < surface->w && draw_y >= 0 && draw_y < surface->h) {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[draw_y * surface->w + draw_x] = color;
                }
            }
        }
    }
}

void draw_circle_outline(SDL_Surface* surface, int x, int y, int r, int thickness, Uint32 color) {
    for (int i = -r; i <= r; i++) {
        for (int j = -r; j <= r; j++) {
            if (i * i + j * j <= r * r && i * i + j * j > (r - thickness) * (r - thickness)) {
                int new_x = x + i;
                int new_y = y + j;
                if (new_x >= 0 && new_x < surface->w && new_y >= 0 && new_y < surface->h) {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[new_y * surface->w + new_x] = color;
                }
            }
        }
    }
}

bool is_inside_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y) {
    float denominator = ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));
    float a = ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / denominator;
    float b = ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / denominator;
    float c = 1 - a - b;
    return (a >= 0) && (a <= 1) && (b >= 0) && (b <= 1) && (c >= 0) && (c <= 1);
}

void draw_triangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color) {
    int min_x = std::min(x1, std::min(x2, x3));
    int max_x = std::max(x1, std::max(x2, x3));
    int min_y = std::min(y1, std::min(y2, y3));
    int max_y = std::max(y1, std::max(y2, y3));

    for (int x = min_x; x <= max_x; x++) {
        for (int y = min_y; y <= max_y; y++) {
            if (is_inside_triangle(x1, y1, x2, y2, x3, y3, x, y)) {
                Uint32* pixels = (Uint32*)surface->pixels;
                pixels[y * surface->w + x] = color;
            }
        }
    }
}

void draw_triangle_outline(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, int thickness, Uint32 color) {
    draw_thick_line(surface, x1, y1, x2, y2, thickness, color);
    draw_thick_line(surface, x2, y2, x3, y3, thickness, color);
    draw_thick_line(surface, x3, y3, x1, y1, thickness, color);
}

void draw_ellipse(SDL_Surface* surface, int x, int y, int rx, int ry, Uint32 color) {
    for (int i = -rx; i <= rx; i++) {
        for (int j = -ry; j <= ry; j++) {
            if (i * i * ry * ry + j * j * rx * rx <= rx * rx * ry * ry) {
                int draw_x = x + i;
                int draw_y = y + j;
                if (draw_x >= 0 && draw_x < surface->w && draw_y >= 0 && draw_y < surface->h) {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[draw_y * surface->w + draw_x] = color;
                }
            }
        }
    }
}

//misses some edge pixels
void draw_ellipse_outline(SDL_Surface* surface, int x, int y, int rx, int ry, Uint32 color, int thickness) {
    int rxSq = rx * rx;
    int rySq = ry * ry;
    int x1 = 0, y1 = ry;
    int p = rySq - (rxSq * ry) + (0.25 * rxSq);

    while (rySq * x1 < rxSq * y1) {
        if (p < 0) {
            x1++;
            p += 2 * rySq * x1 + rySq;
        }
        else {
            x1++;
            y1--;
            p += 2 * rySq * x1 - 2 * rxSq * y1 + rySq;
        }
        for (int i = -thickness; i <= thickness; i++) {
            for (int j = -thickness; j <= thickness; j++) {
                if (x + x1 + i >= 0 && x + x1 + i < surface->w && y + y1 + j >= 0 && y + y1 + j < surface->h) {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[(y + y1 + j) * surface->w + (x + x1 + i)] = color;
                }
                if (x + x1 + i >= 0 && x + x1 + i < surface->w && y - y1 + j >= 0 && y - y1 + j < surface->h) {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[(y - y1 + j) * surface->w + (x + x1 + i)] = color;
                }
                if (x - x1 + i >= 0 && x - x1 + i < surface->w && y + y1 + j >= 0 && y + y1 + j < surface->h) {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[(y + y1 + j) * surface->w + (x - x1 + i)] = color;
                }
                if (x - x1 + i >= 0 && x - x1 + i < surface->w && y - y1 + j >= 0 && y - y1 + j < surface->h) {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[(y - y1 + j) * surface->w + (x - x1 + i)] = color;
                }
            }
        }
    }
    p = rySq * (x1 + 0.5) * (x1 + 0.5) + rxSq * (y1 - 1) * (y1 - 1) - rxSq * rySq;

    while (y1 > 0) {
        if (p > 0) {
            y1--;
            p -= 2 * rxSq * y1 + rxSq;
        }
        else {
            x1++;
            y1--;
            p += 2 * rySq * x1 - 2 * rxSq * y1 + rxSq;
        }
        for (int i = -thickness; i <= thickness; i++) {
            for (int j = -thickness; j <= thickness; j++) {
                if (x + x1 + i >= 0 && x + x1 + i < surface->w && y + y1 + j >= 0 && y + y1 + j < surface->h) {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[(y + y1 + j) * surface->w + (x + x1 + i)] = color;
                }
                if (x + x1 + i >= 0 && x + x1 + i < surface->w && y - y1 + j >= 0 && y - y1 + j < surface->h) {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[(y - y1 + j) * surface->w + (x + x1 + i)] = color;
                }
                if (x - x1 + i >= 0 && x - x1 + i < surface->w && y + y1 + j >= 0 && y + y1 + j < surface->h) {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[(y + y1 + j) * surface->w + (x - x1 + i)] = color;
                }
                if (x - x1 + i >= 0 && x - x1 + i < surface->w && y - y1 + j >= 0 && y - y1 + j < surface->h) {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[(y - y1 + j) * surface->w + (x - x1 + i)] = color;
                }
            }
        }
    }
}

void draw_bezier_curve(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, Uint32 color) {

    int steps = 50;

    double x = x1, y = y1;

    double t_step = 1.0 / steps;

    for (int i = 0; i <= steps; i++) {

        double t = i * t_step;
        x = pow(1 - t, 3) * x1 + 3 * t * pow(1 - t, 2) * x2 + 3 * pow(t, 2) * (1 - t) * x3 + pow(t, 3) * x4;
        y = pow(1 - t, 3) * y1 + 3 * t * pow(1 - t, 2) * y2 + 3 * pow(t, 2) * (1 - t) * y3 + pow(t, 3) * y4;

        if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
            Uint32* pixels = (Uint32*)surface->pixels;
            pixels[(int)y * surface->w + (int)x] = color;
        }
    }
}

void draw_char(SDL_Surface* surface, char c, int x, int y, int size, Uint32 color) {

    std::vector<std::vector<bool>> grid(7, std::vector<bool>(5));

    if (c == 'A') {
        grid[0] = { 0,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,1 };
        grid[3] = { 1,1,1,1,1 };
        grid[4] = { 1,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 1,0,0,0,1 };
    }
    else if (c == 'B') {
        grid[0] = { 1,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,1 };
        grid[3] = { 1,1,1,1,0 };
        grid[4] = { 1,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 1,1,1,1,0 };
    }
    else if (c == 'C') {
        grid[0] = { 0,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,0 };
        grid[3] = { 1,0,0,0,0 };
        grid[4] = { 1,0,0,0,0 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 0,1,1,1,0 };
    }
    else if (c == 'D') {
        grid[0] = { 1,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,1 };
        grid[3] = { 1,0,0,0,1 };
        grid[4] = { 1,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 1,1,1,1,0 };
    }
    else if (c == 'E') {
        grid[0] = { 1,1,1,1,1 };
        grid[1] = { 1,0,0,0,0 };
        grid[2] = { 1,0,0,0,0 };
        grid[3] = { 1,1,1,1,0 };
        grid[4] = { 1,0,0,0,0 };
        grid[5] = { 1,0,0,0,0 };
        grid[6] = { 1,1,1,1,1 };
    }
    else if (c == 'F') {
        grid[0] = { 1,1,1,1,1 };
        grid[1] = { 1,0,0,0,0 };
        grid[2] = { 1,0,0,0,0 };
        grid[3] = { 1,1,1,1,0 };
        grid[4] = { 1,0,0,0,0 };
        grid[5] = { 1,0,0,0,0 };
        grid[6] = { 1,0,0,0,0 };
    }
    else if (c == 'G') {
        grid[0] = { 0,1,1,1,1 };
        grid[1] = { 1,0,0,0,0 };
        grid[2] = { 1,0,0,0,0 };
        grid[3] = { 1,0,1,1,1 };
        grid[4] = { 1,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 0,1,1,1,1 };
    }
    else if (c == 'H') {
        grid[0] = { 1,0,0,0,1 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,1 };
        grid[3] = { 1,1,1,1,1 };
        grid[4] = { 1,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 1,0,0,0,1 };
    }
    else if (c == 'I') {
        grid[0] = { 1,1,1,1,1 };
        grid[1] = { 0,0,1,0,0 };
        grid[2] = { 0,0,1,0,0 };
        grid[3] = { 0,0,1,0,0 };
        grid[4] = { 0,0,1,0,0 };
        grid[5] = { 0,0,1,0,0 };
        grid[6] = { 1,1,1,1,1 };
    }
    else if (c == 'J') {
        grid[0] = { 0,0,0,0,1 };
        grid[1] = { 0,0,0,0,1 };
        grid[2] = { 0,0,0,0,1 };
        grid[3] = { 0,0,0,0,1 };
        grid[4] = { 1,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 0,1,1,1,0 };
    }
    else if (c == 'K') {
        grid[0] = { 1,0,0,0,1 };
        grid[1] = { 1,0,0,1,0 };
        grid[2] = { 1,0,1,0,0 };
        grid[3] = { 1,1,0,0,0 };
        grid[4] = { 1,0,1,0,0 };
        grid[5] = { 1,0,0,1,0 };
        grid[6] = { 1,0,0,0,1 };
    }
    else if (c == 'L') {
        grid[0] = { 1,0,0,0,0 };
        grid[1] = { 1,0,0,0,0 };
        grid[2] = { 1,0,0,0,0 };
        grid[3] = { 1,0,0,0,0 };
        grid[4] = { 1,0,0,0,0 };
        grid[5] = { 1,0,0,0,0 };
        grid[6] = { 1,1,1,1,1 };
    }
    else if (c == 'M') {
        grid[0] = { 1,0,0,0,1 };
        grid[1] = { 1,1,0,1,1 };
        grid[2] = { 1,0,1,0,1 };
        grid[3] = { 1,0,0,0,1 };
        grid[4] = { 1,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 1,0,0,0,1 };
    }
    else if (c == 'N') {
        grid[0] = { 1,0,0,0,1 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,1,0,0,1 };
        grid[3] = { 1,0,1,0,1 };
        grid[4] = { 1,0,0,1,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 1,0,0,0,1 };
    }
    else if (c == 'O') {
        grid[0] = { 0,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,1 };
        grid[3] = { 1,0,0,0,1 };
        grid[4] = { 1,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 0,1,1,1,0 };
    }
    else if (c == 'P') {
        grid[0] = { 1,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,1 };
        grid[3] = { 1,1,1,1,0 };
        grid[4] = { 1,0,0,0,0 };
        grid[5] = { 1,0,0,0,0 };
        grid[6] = { 1,0,0,0,0 };
    }
    else if (c == 'Q') {
        grid[0] = { 0,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,1 };
        grid[3] = { 1,0,1,0,1 };
        grid[4] = { 1,0,0,1,1 };
        grid[5] = { 0,1,1,1,0 };
        grid[6] = { 0,0,0,0,1 };
    }
    else if (c == 'R') {
        grid[0] = { 1,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,1 };
        grid[3] = { 1,1,1,1,0 };
        grid[4] = { 1,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 1,0,0,0,1 };
    }
    else if (c == 'S') {
        grid[0] = { 0,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,0 };
        grid[3] = { 0,1,1,1,0 };
        grid[4] = { 0,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 0,1,1,1,0 };
    }
    else if (c == 'T') {
        grid[0] = { 1,1,1,1,1 };
        grid[1] = { 0,0,1,0,0 };
        grid[2] = { 0,0,1,0,0 };
        grid[3] = { 0,0,1,0,0 };
        grid[4] = { 0,0,1,0,0 };
        grid[5] = { 0,0,1,0,0 };
        grid[6] = { 0,0,1,0,0 };
    }
    else if (c == 'U') {
        grid[0] = { 1,0,0,0,1 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,1 };
        grid[3] = { 1,0,0,0,1 };
        grid[4] = { 1,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 0,1,1,1,0 };
    }
    else if (c == 'V') {
        grid[0] = { 1,0,0,0,1 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,1 };
        grid[3] = { 0,1,0,1,0 };
        grid[4] = { 0,1,0,1,0 };
        grid[5] = { 0,0,1,0,0 };
        grid[6] = { 0,0,1,0,0 };
    }
    else if (c == 'W') {
        grid[0] = { 1,0,0,0,1 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,1,0,1 };
        grid[3] = { 1,0,1,0,1 };
        grid[4] = { 1,0,1,0,1 };
        grid[5] = { 0,1,0,1,0 };
        grid[6] = { 0,1,0,1,0 };
    }
    else if (c == 'X') {
        grid[0] = { 1,0,0,0,1 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 0,1,0,1,0 };
        grid[3] = { 0,0,1,0,0 };
        grid[4] = { 0,1,0,1,0 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 1,0,0,0,1 };
    }
    else if (c == 'Y') {
        grid[0] = { 1,0,0,0,1 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 0,1,0,1,0 };
        grid[3] = { 0,0,1,0,0 };
        grid[4] = { 0,0,1,0,0 };
        grid[5] = { 0,0,1,0,0 };
        grid[6] = { 0,0,1,0,0 };
    }
    else if (c == 'Z') {
        grid[0] = { 1,1,1,1,1 };
        grid[1] = { 0,0,0,0,1 };
        grid[2] = { 0,0,0,1,0 };
        grid[3] = { 0,0,1,0,0 };
        grid[4] = { 0,1,0,0,0 };
        grid[5] = { 1,0,0,0,0 };
        grid[6] = { 1,1,1,1,1 };
    }
    else if (c == '0') {
        grid[0] = { 0,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,1,1 };
        grid[3] = { 1,0,1,0,1 };
        grid[4] = { 1,1,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 0,1,1,1,0 };
    }
    else if (c == '1') {
        grid[0] = { 0,0,1,0,0 };
        grid[1] = { 0,1,1,0,0 };
        grid[2] = { 1,0,1,0,0 };
        grid[3] = { 0,0,1,0,0 };
        grid[4] = { 0,0,1,0,0 };
        grid[5] = { 0,0,1,0,0 };
        grid[6] = { 1,1,1,1,1 };
    }
    else if (c == '2') {
        grid[0] = { 0,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 0,0,0,0,1 };
        grid[3] = { 0,0,0,1,0 };
        grid[4] = { 0,0,1,0,0 };
        grid[5] = { 0,1,0,0,0 };
        grid[6] = { 1,1,1,1,1 };
    }
    else if (c == '3') {
        grid[0] = { 0,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 0,0,0,0,1 };
        grid[3] = { 0,0,1,1,0 };
        grid[4] = { 0,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 0,1,1,1,0 };
    }
    else if (c == '4') {
        grid[0] = { 1,0,0,0,1 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,1 };
        grid[3] = { 1,1,1,1,1 };
        grid[4] = { 0,0,0,0,1 };
        grid[5] = { 0,0,0,0,1 };
        grid[6] = { 0,0,0,0,1 };
    }
    else if (c == '5') {
        grid[0] = { 1,1,1,1,1 };
        grid[1] = { 1,0,0,0,0 };
        grid[2] = { 1,0,0,0,0 };
        grid[3] = { 1,1,1,1,0 };
        grid[4] = { 0,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 0,1,1,1,0 };
    }
    else if (c == '6') {
        grid[0] = { 0,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,0 };
        grid[3] = { 1,1,1,1,0 };
        grid[4] = { 1,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 0,1,1,1,0 };
    }
    else if (c == '7') {
        grid[0] = { 1,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 0,0,0,0,1 };
        grid[3] = { 0,0,0,1,0 };
        grid[4] = { 0,0,1,0,0 };
        grid[5] = { 0,0,1,0,0 };
        grid[6] = { 0,0,1,0,0 };
    }
    else if (c == '8') {
        grid[0] = { 0,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,1 };
        grid[3] = { 0,1,1,1,0 };
        grid[4] = { 1,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 0,1,1,1,0 };
    }
    else if (c == '9') {
        grid[0] = { 0,1,1,1,0 };
        grid[1] = { 1,0,0,0,1 };
        grid[2] = { 1,0,0,0,1 };
        grid[3] = { 0,1,1,1,1 };
        grid[4] = { 0,0,0,0,1 };
        grid[5] = { 1,0,0,0,1 };
        grid[6] = { 0,1,1,1,0 };
    }
    else if (c == ' ') {
        grid[0] = { 0,0,0,0,0 };
        grid[1] = { 0,0,0,0,0 };
        grid[2] = { 0,0,0,0,0 };
        grid[3] = { 0,0,0,0,0 };
        grid[4] = { 0,0,0,0,0 };
        grid[5] = { 0,0,0,0,0 };
        grid[6] = { 0,0,0,0,0 };
    }
    else if (c == '.') {
        grid[0] = { 0,0,0,0,0 };
        grid[1] = { 0,0,0,0,0 };
        grid[2] = { 0,0,0,0,0 };
        grid[3] = { 0,0,0,0,0 };
        grid[4] = { 0,0,0,0,0 };
        grid[5] = { 0,0,0,0,0 };
        grid[6] = { 0,0,1,0,0 };
    }

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 5; j++) {
            if (grid[i][j]) {
                draw_square(surface, x + size*j, y + size*i, size, color);
            }
        }
    }

}

void draw_string(SDL_Surface* surface, std::string s, int x, int y, int size, Uint32 color) {

    for (int i = 0; i < s.length(); i++) {

        draw_char(surface, s[i], x + (size*6)*i, y, size, color);

    }

}

void draw_asset(SDL_Surface* surface,std::vector<Uint32> color, int x, int y, int pixel_size) {

    int line_counter = 0;

    for (int i = 0; i < 256; i++) {
        if (color[line_counter]!=100000000) {
            draw_square(surface, (line_counter % 16) * pixel_size + x, ((line_counter - line_counter % 16) / 16) * pixel_size + y, pixel_size, color[line_counter]);
        }
            line_counter++;
    }

    /*
    int line_counter;
    file_name += ".txt";
    std::ifstream inputFile(file_name);
    if (inputFile.is_open()) {
        std::string line;
        line_counter = 0;
        while (std::getline(inputFile, line)) {
            if (std::stoi(line) != 100000000) {
                draw_square(surface, (line_counter % 16) * pixel_size + x, ((line_counter - line_counter % 16) / 16) * pixel_size + y, pixel_size, std::stoi(line));
            }
            line_counter++;
        }
        inputFile.close();
    }
    */

}

void draw_world(SDL_Surface* surface, std::vector<std::vector<Uint32>> texture, std::vector<std::vector<cell>> world, int x, int y, int pixel_size) {

    for (int i = 0; i < world[0].size(); i++) {
        for (int j = 0; j < world.size(); j++) {

           draw_asset(surface, texture[world[j][i].texture], x + j * pixel_size * 16, y + i * pixel_size * 16, pixel_size);

        }
    }

}