#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// Setup the display
void setup() {

    // Initialize the I2C
    Wire.begin();

    // Initialize Display on adress 0x3C, change if nedded
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

}

// Main loop
void loop() {

    // Clear the display
    display.clearDisplay();

    // Set the text size
    display.setTextSize(1);

    // Set the text color
    display.setTextColor(WHITE);

    // Set the text cursor position and print
    display.setCursor(0,0);
    display.println("FunBots");
    display.setCursor(0,10);
    display.println("Temperatura");
    display.println("25.5 C");
    display.setCursor(0,20);
    
    display.drawLine(0, 20, 128, 20, SSD1306_WHITE);

    // Display the text
    display.display();

    // Wait 1000ms
    delay(1000);

}