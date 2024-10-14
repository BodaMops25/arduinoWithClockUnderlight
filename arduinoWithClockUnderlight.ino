#define COLOR_PONT A1
#define PONT_COLOR_RANGE 16
#define PWR_BTN 13

#define WAKE_HOUR 7
#define WAKE_MINUTE 30

#define LED_STRIP_PIN 12
#define LED_AM 300

#include <GyverDS3231.h>
GyverDS3231 ds;

#include <EncButton.h>
Button btn(PWR_BTN);

#include "FastLED.h"
CRGB leds[LED_AM];

bool power = 1;
byte color = 0;

void setup() {
  pinMode(COLOR_PONT, INPUT);

  Wire.begin();
  if (!ds.begin()) ds.setBuildTime();

  FastLED.addLeds<WS2811, LED_STRIP_PIN, GRB>(leds, LED_AM).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255);
  FastLED.clear();

  // Serial.begin(9600);
}

int pontPrev = 0;

void loop() {
  btn.tick();
  
  if(ds.tick()) {
    Datime dt(ds);

    if(dt.hour == WAKE_HOUR && dt.minute == WAKE_MINUTE && power == 0) {
      power = 1;
      // Serial.print("WAKE UP!!!!!!!!!!!!!!!!!!!!!!!!");
    }
  }

  if(btn.click()) {
    power = !power;

    if(power == 0) {
      FastLED.clear();
      FastLED.show();
    }
  }

  if(power) {
    int rawColor = analogRead(COLOR_PONT);

    if(rawColor < pontPrev - PONT_COLOR_RANGE || pontPrev + PONT_COLOR_RANGE < rawColor) {
      color = constrain(
        map(rawColor, 15, 950, 0, 255),
        0, 255
      );

      pontPrev = rawColor;
    }

    // if(rawColor % 4 == 0) {
    //   color = constrain(
    //     map(rawColor, 15, 970, 0, 255),
    //     0, 255
    //   );
    // }

    // color = constrain(rawColor / 1024.0 * 255, 0, 255);

    fill_solid(leds, LED_AM, CHSV(color, 255, 255));
    FastLED.show();

    // Serial.print("Power: ");Serial.print(power);
    // Serial.print(", Color: ");Serial.print(color);
    // Serial.print(", Raw: ");Serial.println(rawColor);
  }
}