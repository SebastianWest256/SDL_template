//useful functions for drawing various shapes

void setPixelColor(SDL_Surface* surface, int x, int y, Uint32 color) {

    //bounds checking \/
    //x = std::max(x, 0);
    //x = std::min(x, screenWidth - 1);
    //y = std::max(y, 0);
    //y = std::min(y, screenHeight - 1);
    Uint32* pixels = (Uint32*)surface->pixels;
    pixels[y * surface->w + x] = color;
}

void drawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color) {
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

void drawThickLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, int thickness, Uint32 color) {
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

void drawSquare(SDL_Surface* surface, int x, int y, int sideLength, Uint32 color) {
    SDL_Rect square;
    square.x = x;
    square.y = y;
    square.w = sideLength;
    square.h = sideLength;
    SDL_FillRect(surface, &square, color);
}

void drawSquareOutline(SDL_Surface* surface, int x, int y, int side, int thickness, Uint32 color) {
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
void drawRotatedSquare(SDL_Surface* surface, int x, int y, int side, float angle, Uint32 color)
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

void drawRectangle(SDL_Surface* surface, int x, int y, int w, int h, Uint32 color) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_FillRect(surface, &rect, color);
}

void drawRectangleOutline(SDL_Surface* surface, int x, int y, int w, int h, int thickness, Uint32 color) {
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
void drawRotatedRectangle(SDL_Surface* surface, int x, int y, int w, int h, float angle, Uint32 color) {
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

void drawCircle(SDL_Surface* surface, int x, int y, int radius, Uint32 color) {
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

void drawCircleOutline(SDL_Surface* surface, int x, int y, int r, int thickness, Uint32 color) {
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

bool isInsideTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y) {
    float denominator = ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));
    float a = ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / denominator;
    float b = ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / denominator;
    float c = 1 - a - b;
    return (a >= 0) && (a <= 1) && (b >= 0) && (b <= 1) && (c >= 0) && (c <= 1);
}

void drawTriangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color) {
    int min_x = std::min(x1, std::min(x2, x3));
    int max_x = std::max(x1, std::max(x2, x3));
    int min_y = std::min(y1, std::min(y2, y3));
    int max_y = std::max(y1, std::max(y2, y3));

    for (int x = min_x; x <= max_x; x++) {
        for (int y = min_y; y <= max_y; y++) {
            if (isInsideTriangle(x1, y1, x2, y2, x3, y3, x, y)) {
                Uint32* pixels = (Uint32*)surface->pixels;
                pixels[y * surface->w + x] = color;
            }
        }
    }
}

void drawTriangleOutline(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, int thickness, Uint32 color) {
    drawThickLine(surface, x1, y1, x2, y2, thickness, color);
    drawThickLine(surface, x2, y2, x3, y3, thickness, color);
    drawThickLine(surface, x3, y3, x1, y1, thickness, color);
}

void drawEllipse(SDL_Surface* surface, int x, int y, int rx, int ry, Uint32 color) {
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
void drawEllipseOutline(SDL_Surface* surface, int x, int y, int rx, int ry, Uint32 color, int thickness) {
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

void drawBezierCurve(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, Uint32 color) {
    // The number of steps for drawing the curve
    int steps = 50;

    // The current position of the curve
    double x = x1, y = y1;

    // The step size
    double t_step = 1.0 / steps;

    // Loop over the steps
    for (int i = 0; i <= steps; i++) {
        // Calculate the position of the curve at this step
        double t = i * t_step;
        x = pow(1 - t, 3) * x1 + 3 * t * pow(1 - t, 2) * x2 + 3 * pow(t, 2) * (1 - t) * x3 + pow(t, 3) * x4;
        y = pow(1 - t, 3) * y1 + 3 * t * pow(1 - t, 2) * y2 + 3 * pow(t, 2) * (1 - t) * y3 + pow(t, 3) * y4;

        // Draw the pixel on the surface
        if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
            Uint32* pixels = (Uint32*)surface->pixels;
            pixels[(int)y * surface->w + (int)x] = color;
        }
    }
}