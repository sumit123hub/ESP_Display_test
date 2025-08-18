// Simple snake game using TFT_eSPI
// Upload this file adjusting the parameters according to your need



#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <TFT_eSPI.h>
#include <vector>

// Touch Screen pins
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);
TFT_eSPI tft = TFT_eSPI();

// Game constants
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;
const int SNAKE_SIZE = 10;
const int FOOD_SIZE = 10;
const int UPDATE_INTERVAL = 75;  // ms
const int REFRESH_SIZE = 5;

// Game structures
struct Point {
    int x, y;
};

// Game state
Point direction = {1, 0};  // Start moving right
Point nextDirection = {1, 0};
std::vector<Point> snake;
Point food;
bool gameOver = false;
bool scoreUpdated = false;
unsigned long lastUpdateTime = 0;
int score = 0;
int highScore = 0;

void drawGame() {
    // tft.fillScreen(TFT_BLACK);
    
    // Draw food
    tft.fillRect(food.x, food.y, FOOD_SIZE, FOOD_SIZE, TFT_GREEN);
    
    // Draw snake
    for (size_t i = 0; i < snake.size(); i++) {
        if (i == 0) {
            // Snake head
            tft.fillRect(snake[i].x, snake[i].y, SNAKE_SIZE, SNAKE_SIZE, TFT_RED);
        } else {
            // Snake body
            tft.fillRect(snake[i].x, snake[i].y, SNAKE_SIZE, SNAKE_SIZE, TFT_WHITE);
        }
    }
    
    // Draw score
    tft.setTextSize(1);
    if(scoreUpdated)
    {
        tft.setTextColor(TFT_BLACK);
        tft.setCursor(5, 5);
        tft.print("Score: ");
        tft.print(score - 1);
        scoreUpdated = false;
    }
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(5, 5);
    tft.print("Score: ");
    tft.print(score);

    tft.setCursor(SCREEN_WIDTH - 80, 5);
    tft.print("High: ");
    tft.print(highScore);
}

void spawnFood() {
    bool onSnake;
    do {
        onSnake = false;
        food.x = random(SNAKE_SIZE, SCREEN_WIDTH - FOOD_SIZE) / SNAKE_SIZE * SNAKE_SIZE;
        food.y = random(SNAKE_SIZE, SCREEN_HEIGHT - FOOD_SIZE) / SNAKE_SIZE * SNAKE_SIZE;
        
        for (const auto& segment : snake) {
            if (food.x == segment.x && food.y == segment.y) {
                onSnake = true;
                break;
            }
        }
    } while (onSnake);
}

void resetGame() {
    snake.clear();
    snake.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    snake.push_back({SCREEN_WIDTH / 2 - SNAKE_SIZE, SCREEN_HEIGHT / 2});
    snake.push_back({SCREEN_WIDTH / 2 - 2 * SNAKE_SIZE, SCREEN_HEIGHT / 2});
    
    direction = {1, 0};
    nextDirection = {1, 0};
    gameOver = false;
    score = 0;
    
    spawnFood();
    drawGame();
}

void setup() {
    Serial.begin(115200);

    // Initialize touch screen
    mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    ts.begin(mySpi);
    ts.setRotation(1);

    // Initialize display
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    // Initialize game
    resetGame();
}

void processInput() {
    if (ts.tirqTouched() && ts.touched()) {
        TS_Point p = ts.getPoint();
        
        if (p.x < 1000 && p.y > 1000 && p.y < 3000 && direction.x != 1) {
            nextDirection = {-1, 0};  // Left
        }
        else if (p.x > 3000 && p.y > 1000 && p.y < 3000 && direction.x != -1) {
            nextDirection = {1, 0};   // Right
        }
        else if (p.x > 1000 && p.x < 3000 && p.y < 1000 && direction.y != 1) {
            nextDirection = {0, -1};  // Up
        }
        else if (p.x > 1000 && p.x < 3000 && p.y > 3000 && direction.y != -1) {
            nextDirection = {0, 1};   // Down
        }
    }
}

void updateGame() {
    // Apply buffered direction
    direction = nextDirection;

    // Calculate new head position
    Point newHead = {
        snake[0].x + direction.x * SNAKE_SIZE,
        snake[0].y + direction.y * SNAKE_SIZE
    };

    // Wall collision (warp around)
    if (newHead.x < 0) newHead.x = SCREEN_WIDTH - SNAKE_SIZE;
    else if (newHead.x >= SCREEN_WIDTH) newHead.x = 0;
    if (newHead.y < 0) newHead.y = SCREEN_HEIGHT - SNAKE_SIZE;
    else if (newHead.y >= SCREEN_HEIGHT) newHead.y = 0;

    // Self collision detection
    for (size_t i = 0; i < snake.size(); i++) {
        if (newHead.x == snake[i].x && newHead.y == snake[i].y) {
            gameOver = true;
            if (score > highScore) highScore = score;
            return;
        }
    }

    // Add new head
    snake.insert(snake.begin(), newHead);

    // Food collection
    if (newHead.x == food.x && newHead.y == food.y) {
        score++;
        scoreUpdated = true;
        spawnFood();
    } else {
        // Remove tail if no food was eaten
        tft.fillRect(snake[snake.size() - 1].x, snake[snake.size() - 1].y, SNAKE_SIZE, SNAKE_SIZE, TFT_BLACK);
        snake.pop_back();
    }
}


void drawGameOver() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED);
    tft.setTextSize(2);
    tft.setCursor(SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 - 20);
    tft.print("GAME OVER");
    
    tft.setTextColor(TFT_YELLOW);
    tft.setTextSize(1);
    tft.setCursor(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 10);
    tft.print("Score: ");
    tft.print(score);
    
    tft.setCursor(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 30);
    tft.print("High Score: ");
    tft.print(highScore);
    
    tft.setTextColor(TFT_GREEN);
    tft.setCursor(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 60);
    tft.print("Touch to Restart");
}

void loop() {
    processInput();
    
    if (gameOver) {
        if (ts.touched()) {
            resetGame();
        }
        return;
    }
    
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
        updateGame();
        drawGame();
        lastUpdateTime = currentTime;
        
        if (gameOver) {
            drawGameOver();
        }
    }
}