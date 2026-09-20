#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>

// ====== Matrix Pin Definitions (adjust if needed) ======
#define CLK  11
#define OE   9
#define LAT  10
#define A    A0
#define B    A1
#define C    A2
#define D    A3  // Only used for 32x32 matrices

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

// ====== Constants ======
#define NUMCOLS 32
#define NUMROWS 32
#define FALLRATE 500  // Milliseconds between gravity steps

// ====== Colors ======
#define WALL_COLOR   matrix.Color333(9, 0 , 0)  // Red
#define AVATAR_COLOR matrix.Color333(0, 9, 0)  // Green
#define EMPTY_COLOR  0                        // Off

// ====== Maze and Avatar ======
char MAZE[NUMROWS][NUMCOLS];
int avatarX = NUMCOLS / 2;
int avatarY = 0;
unsigned long lastFallTime = 0;

// ====== Function Prototypes ======
void generate_maze(int difficulty);
void draw_maze();
void draw_character(int x, int y, uint16_t color);

void setup() {
  matrix.begin();
  Serial.begin(9600);  // Match with PC parser baud rate

  generate_maze(50);  // Difficulty: 0 (easy) to 100 (impossible)
  draw_maze();
  draw_character(avatarX, avatarY, AVATAR_COLOR);
}

void loop() {
  // ========== Movement Input ==========
  if (Serial.available()) {
    char input = Serial.read();
    if (input == 'L' && avatarX > 0 && MAZE[avatarY][avatarX - 1] != '*') {
      draw_character(avatarX, avatarY, EMPTY_COLOR);
      avatarX -= 1;
      draw_character(avatarX, avatarY, AVATAR_COLOR);
    } else if (input == 'R' && avatarX < NUMCOLS - 1 && MAZE[avatarY][avatarX + 1] != '*') {
      draw_character(avatarX, avatarY, EMPTY_COLOR);
      avatarX += 1;
      draw_character(avatarX, avatarY, AVATAR_COLOR);
    }
  }

  // ========== Gravity (falling down) ==========
  unsigned long currentTime = millis();
  if (currentTime - lastFallTime > FALLRATE && avatarY < NUMROWS - 1 && MAZE[avatarY + 1][avatarX] != '*') {
    draw_character(avatarX, avatarY, EMPTY_COLOR);
    avatarY += 1;
    draw_character(avatarX, avatarY, AVATAR_COLOR);
    lastFallTime = currentTime;
  }
}

// ====== Draw One Character ======
void draw_character(int x, int y, uint16_t color) {
  matrix.drawPixel(x, y, color);
}

// ====== Draw the Maze to the Matrix ======
void draw_maze() {
  for (int i = 0; i < NUMROWS; i++) {
    for (int j = 0; j < NUMCOLS; j++) {
      if (MAZE[i][j] == '*') {
        draw_character(j, i, WALL_COLOR);
      }
    }
  }
}

// ====== Maze Generator ======
void generate_maze(int difficulty) {
  for (int i = 0; i < NUMROWS; i++) {
    for (int j = 0; j < NUMCOLS; j++) {
      if (random(100) < difficulty) {
        MAZE[i][j] = '*';
      } else {
        MAZE[i][j] = ' ';
      }
    }q
  }
}
