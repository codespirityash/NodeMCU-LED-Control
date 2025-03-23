#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class Ball {
public:
    int x, y;
    float vel_x, vel_y;
    int radius;
    unsigned long startTime;

    Ball() {
        x = 10;
        y = 10;
        vel_x = 1.0;
        vel_y = 1.0;
        radius = 3;
        startTime = millis();
    }

    void move() {
        x += vel_x;
        y += vel_y;
    }

    void increaseVelocity() {
        unsigned long elapsedTime = (millis() - startTime) / 1000;
        float speedIncrease = 0.1 * elapsedTime;

        if (vel_x > 0) {
            vel_x = 1.0 + speedIncrease;
        } else {
            vel_x = -1.0 - speedIncrease;
        }

        if (vel_y > 0) {
            vel_y = 1.0 + speedIncrease;
        } else {
            vel_y = -1.0 - speedIncrease;
        }
    }

    void checkWallCollision() {
        if (x - radius <= 0) {
            x = radius;
            vel_x = -vel_x;
        }
        if (y - radius <= 0) {
            y = radius;
            vel_y = -vel_y;
        }
        if (y + radius >= SCREEN_HEIGHT) {
            y = SCREEN_HEIGHT - radius;
            vel_y = -vel_y;
        }
    }

    void draw() {
        display.fillCircle(x, y, radius, SSD1306_WHITE);
    }
};

class Player {
public:
    int x, y;
    int width, height;

    Player() {
        x = SCREEN_WIDTH - 10;
        y = SCREEN_HEIGHT / 2;
        width = 3;
        height = 15;
    }

    void updatePosition() {
        int sensorValue = analogRead(A0);
        y = map(sensorValue, 0, 1023, 0, SCREEN_HEIGHT - height);
    }

    void draw() {
        display.fillRect(x, y, width, height, SSD1306_WHITE);
    }
};

Ball ball;
Player player;
bool gameOver = false;
unsigned long survivalTime;
unsigned long gameOverTime;

void resetGame() {
    ball = Ball();
    player = Player();
    gameOver = false;
}

void checkCollision() {
    if (ball.x + ball.radius > player.x && ball.y > player.y && ball.y < player.y + player.height) {
        ball.x = player.x - ball.radius;
        ball.vel_x = -ball.vel_x;
    }

    if (ball.x + ball.radius >= SCREEN_WIDTH) {
        gameOver = true;
        survivalTime = (millis() - ball.startTime) / 1000;
        gameOverTime = millis();
    }
}

void displayGameOver() {
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 20);
    display.println("YOU LOSE!!");
    display.setCursor(20, 30);
    display.print("Time: ");
    display.print(survivalTime);
    display.println("s");
}

void setup() {
    Serial.begin(9600);
    pinMode(A0, INPUT);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        while (true);
    }
    display.clearDisplay();
    resetGame();
}

void loop() {
    display.clearDisplay();

    if (!gameOver) {
        player.updatePosition();
        ball.increaseVelocity(); 
        ball.move();
        ball.checkWallCollision();
        checkCollision();

        ball.draw();
        player.draw();
    } else {
        displayGameOver();
        if (millis() - gameOverTime >= 3000) {
            resetGame();
        }
    }

    display.display();
    delay(10);
}
