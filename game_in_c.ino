#include <LiquidCrystal.h>

#define BUTTON_PIN 2  // Push button connected to pin 2

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

bool gameRunning = false;  
int score = 0;
int boyRow = 1;   // 1 = ground, 0 = jump
int obsPos = 15;   // Obstacle position
bool isJumping = false;
int jumpTimer = 0;

// Speed Variables
int gameSpeed = 400;  // Initial speed (higher value = slower)
int minSpeed = 100;   // Fastest speed limit
int speedDecreaseRate = 20;  // How much speed increases per point

// Custom character for the boy
byte boyCharacter[8] = {
    0b00110,
    0b00110,
    0b01110,
    0b10101,
    0b00110,
    0b00110,
    0b01010,
    0b01010
};

// Custom character for brick obstacle
byte brickCharacter[8] = {
    0b11111,
    0b10001,
    0b11111,
    0b10001,
    0b11111,
    0b10001,
    0b11111,
    0b11111
};

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    lcd.begin(16, 2);
    lcd.createChar(0, boyCharacter);   // Create the boy character
    lcd.createChar(1, brickCharacter); // Create the brick obstacle
    showStartScreen();
}

void loop() {
    if (!gameRunning) {
        if (digitalRead(BUTTON_PIN) == LOW) {  
            delay(200);  // Debounce delay
            startGame();
        }
        return;
    }

    updateGame();
    delay(gameSpeed);  // Game speed is dynamic now!
}

void showStartScreen() {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Start Game");
    lcd.setCursor(0, 1);
    lcd.print("Press Button!");
}

void startGame() {
    gameRunning = true;
    score = 0;
    boyRow = 1;
    obsPos = 15;
    isJumping = false;
    jumpTimer = 0;
    gameSpeed = 400;  // Reset speed at the start of each game
    lcd.clear();
}

void updateGame() {
    lcd.clear();

    // Check if button is pressed for jumping
    if (digitalRead(BUTTON_PIN) == LOW && !isJumping) {
        isJumping = true;
        jumpTimer = 3;  // Dino stays in air for 3 cycles
    }

    // Handle jumping
    if (isJumping) {
        boyRow = 0;  // Dino in air
        jumpTimer--;
        if (jumpTimer <= 0) {
            isJumping = false;  // End jump
            boyRow = 1;  // boy lands
        }
    }

    // Show Boy character at correct row
    lcd.setCursor(1, boyRow);
    lcd.write(byte(0));  // Display the custom "boy" character

    // Show Brick Obstacle
    lcd.setCursor(obsPos, 1);
    lcd.write(byte(1));  // Display the custom "brick" character

    // Collision Detection
    if (obsPos == 1 && boyRow == 1) {  
        gameOver();
        return;
    }

    // Move obstacle left
    obsPos--;
    if (obsPos < 0) {
        obsPos = 15;  // Reset obstacle position (infinite obstacles)
        score++;  // Increase score when an obstacle is passed

        // Increase speed gradually
        if (gameSpeed > minSpeed) {
            gameSpeed -= speedDecreaseRate;
        }
    }

    // Show score
    lcd.setCursor(8, 0);
    lcd.print("Score:");
    lcd.setCursor(14, 0);
    lcd.print(score);
}

void gameOver() {
    gameRunning = false;
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Game Over");
    lcd.setCursor(2, 1);
    lcd.print("Score: ");
    lcd.print(score);
    delay(2000);
    showStartScreen();  
}
