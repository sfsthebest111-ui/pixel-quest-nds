#include <nds.h>
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_W 256
#define SCREEN_H 192

static u16 *framebuffer;

static inline u16 rgb(u8 r, u8 g, u8 b) {
    return RGB15(r >> 3, g >> 3, b >> 3);
}

static void clearScreen(u16 color) {
    for (int i = 0; i < SCREEN_W * SCREEN_H; i++) framebuffer[i] = color;
}

static void pixel(int x, int y, u16 color) {
    if (x >= 0 && x < SCREEN_W && y >= 0 && y < SCREEN_H)
        framebuffer[y * SCREEN_W + x] = color;
}

static void rect(int x, int y, int w, int h, u16 color) {
    if (w <= 0 || h <= 0) return;
    int x0 = x < 0 ? 0 : x;
    int y0 = y < 0 ? 0 : y;
    int x1 = x + w > SCREEN_W ? SCREEN_W : x + w;
    int y1 = y + h > SCREEN_H ? SCREEN_H : y + h;
    for (int yy = y0; yy < y1; yy++)
        for (int xx = x0; xx < x1; xx++)
            pixel(xx, yy, color);
}

static void drawNumber(int x, int y, int value, u16 color) {
    static const unsigned char digits[10][5] = {
        {7,5,5,5,7}, {2,6,2,2,7}, {7,1,7,4,7}, {7,1,7,1,7}, {5,5,7,1,1},
        {7,4,7,1,7}, {7,4,7,5,7}, {7,1,1,1,1}, {7,5,7,5,7}, {7,5,7,1,7}
    };
    if (value < 0) value = 0;
    if (value > 999) value = 999;
    int hundreds = value / 100;
    int tens = (value / 10) % 10;
    int ones = value % 10;
    int vals[3] = {hundreds, tens, ones};
    int start = value >= 100 ? 0 : (value >= 10 ? 1 : 2);
    for (int d = start; d < 3; d++) {
        int dx = x + (d - start) * 5;
        for (int yy = 0; yy < 5; yy++)
            for (int xx = 0; xx < 3; xx++)
                if (digits[vals[d]][yy] & (1 << (2 - xx)))
                    rect(dx + xx, y + yy, 1, 1, color);
    }
}

static void drawPlayer(int x, int y) {
    const u16 outline = RGB15(3, 4, 12);
    const u16 skin = RGB15(31, 22, 13);
    const u16 shirt = RGB15(5, 23, 31);
    const u16 trousers = RGB15(8, 10, 24);
    /* Chunky 12x16 pixel-art explorer */
    rect(x + 3, y,     6, 2, outline);
    rect(x + 2, y + 2, 8, 5, skin);
    rect(x + 2, y + 2, 8, 2, outline);
    rect(x + 4, y + 4, 1, 1, outline);
    rect(x + 8, y + 4, 1, 1, outline);
    rect(x + 1, y + 7, 10, 5, shirt);
    rect(x,     y + 8, 2, 4, skin);
    rect(x + 10,y + 8, 2, 4, skin);
    rect(x + 2, y + 12, 3, 4, trousers);
    rect(x + 7, y + 12, 3, 4, trousers);
    rect(x + 1, y + 15, 4, 1, outline);
    rect(x + 7, y + 15, 4, 1, outline);
}

static void drawGem(int x, int y, int phase) {
    u16 gold = RGB15(31, 25, 3);
    u16 shine = RGB15(31, 31, 18);
    int bob = (phase / 8) % 2;
    rect(x + 3, y + bob, 4, 2, shine);
    rect(x + 1, y + 2 + bob, 8, 4, gold);
    rect(x + 3, y + 6 + bob, 4, 2, gold);
    pixel(x + 3, y + 2 + bob, shine);
    pixel(x + 4, y + 3 + bob, shine);
}

static void drawScene(int px, int py, int gx, int gy, int score, int tick) {
    clearScreen(RGB15(5, 9, 20));

    /* Night sky and twinkling pixel stars */
    for (int i = 0; i < 26; i++) {
        int sx = (i * 47 + 13) % SCREEN_W;
        int sy = (i * 29 + 9) % 112;
        u16 c = ((tick / 20 + i) % 3 == 0) ? RGB15(31, 31, 22) : RGB15(15, 19, 27);
        pixel(sx, sy, c);
        if (i % 4 == 0) pixel(sx + 1, sy, c);
    }

    /* Distant hills */
    rect(0, 112, 256, 48, RGB15(4, 13, 20));
    for (int x = 0; x < 256; x += 32) {
        rect(x + 4, 104, 16, 8, RGB15(4, 13, 20));
        rect(x + 8, 100, 8, 4, RGB15(4, 13, 20));
    }

    /* Ground */
    rect(0, 160, 256, 32, RGB15(7, 20, 9));
    rect(0, 160, 256, 3, RGB15(12, 28, 13));
    for (int x = 4; x < 256; x += 19)
        rect(x, 168 + (x % 3), 3, 2, RGB15(10, 25, 12));

    /* Floating platforms */
    rect(42, 132, 45, 5, RGB15(13, 17, 23));
    rect(42, 132, 45, 2, RGB15(20, 25, 29));
    rect(145, 118, 42, 5, RGB15(13, 17, 23));
    rect(145, 118, 42, 2, RGB15(20, 25, 29));

    drawGem(gx, gy, tick);
    drawPlayer(px, py);

    /* Simple HUD */
    rect(5, 5, 74, 18, RGB15(1, 3, 10));
    rect(5, 5, 74, 1, RGB15(15, 19, 27));
    rect(9, 10, 3, 3, RGB15(31, 25, 3));
    drawNumber(17, 9, score, RGB15(31, 31, 31));

    /* Tiny control legend */
    rect(170, 5, 81, 18, RGB15(1, 3, 10));
    rect(175, 11, 5, 5, RGB15(18, 22, 28));
    rect(177, 9, 1, 9, RGB15(31, 31, 31));
    rect(173, 13, 9, 1, RGB15(31, 31, 31));
    rect(188, 11, 5, 5, RGB15(5, 23, 31));
    rect(190, 9, 1, 9, RGB15(31, 31, 31));
    rect(186, 13, 9, 1, RGB15(31, 31, 31));
    rect(203, 11, 5, 5, RGB15(24, 8, 8));
    rect(205, 9, 1, 9, RGB15(31, 31, 31));
    rect(201, 13, 9, 1, RGB15(31, 31, 31));
}

int main(void) {
    videoSetMode(MODE_FB0);
    vramSetBankA(VRAM_A_LCD);
    framebuffer = (u16 *)VRAM_A;

    int playerX = 24;
    int playerY = 144;
    int velocityY = 0;
    int gemX = 170;
    int gemY = 98;
    int score = 0;
    int tick = 0;
    bool onGround = true;

    while (pmMainLoop()) {
        scanKeys();
        int held = keysHeld();
        int down = keysDown();

        if (held & KEY_LEFT) playerX -= 2;
        if (held & KEY_RIGHT) playerX += 2;

        if ((down & KEY_A) && onGround) {
            velocityY = -7;
            onGround = false;
        }

        velocityY += 1;
        if (velocityY > 5) velocityY = 5;
        playerY += velocityY;

        if (playerY >= 144) {
            playerY = 144;
            velocityY = 0;
            onGround = true;
        }

        /* A couple of easy-to-reach floating platforms */
        if (playerY + 16 >= 132 && playerY + 16 <= 139 &&
            playerX + 12 > 42 && playerX < 87 && velocityY >= 0) {
            playerY = 116;
            velocityY = 0;
            onGround = true;
        }
        if (playerY + 16 >= 118 && playerY + 16 <= 125 &&
            playerX + 12 > 145 && playerX < 187 && velocityY >= 0) {
            playerY = 102;
            velocityY = 0;
            onGround = true;
        }

        if (playerX < 0) playerX = 0;
        if (playerX > SCREEN_W - 12) playerX = SCREEN_W - 12;

        /* Collect the glowing gem */
        if (playerX < gemX + 10 && playerX + 12 > gemX &&
            playerY < gemY + 10 && playerY + 16 > gemY) {
            score++;
            gemX = 12 + ((score * 67 + 31) % 220);
            gemY = 82 + ((score * 23) % 65);
        }

        drawScene(playerX, playerY, gemX, gemY, score, tick++);
        swiWaitForVBlank();
    }
    return 0;
}
