#include "FastLED.h"
#define DATA_PIN    2
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    144
CRGB leds[NUM_LEDS];
#define BUTTON_LEAD  3
#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  500

#define SPEED 92 //higher value is slower
bool showMode = true;
bool showEnd = true;
uint8_t pos = 0;
int posDir = 1;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
unsigned long time;
unsigned long timeS;
unsigned long timeE;
unsigned long timeC;

void setup() {
  delay(3000); // 3 second delay for recovery
  Serial.begin(9600);
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  attachInterrupt(1, buttonListener, RISING);
}

void loop() {
  time = millis();
  timeS = time;
  timeC = timeS - timeE;

  Serial.print ("Cycle time: ");
  Serial.println (timeC);
  line();
  EVERY_N_MILLISECONDS( 10 ) { gHue++;  /* slowly cycle the "base color" through the rainbow*/}
  timeE = time;
}

void line() {
    if (pos == NUM_LEDS-1) {
      showEnd = true;
      posDir = -1;
    }
    if (pos == 0) {
      showEnd = true;
      posDir = 1;
    }
    if (showEnd == true) {
      while (showMode == false){
        Serial.println ("wipe");
        wipe();
      }    
    }
    showEnd = false;
    pos += posDir;
    EVERY_N_MILLISECONDS(SPEED) {
    FastLED.clear();
    }
    leds[pos] += CHSV(gHue, 255, 255); //Center
    if (pos < NUM_LEDS-1) { leds[pos + 1] += CHSV(gHue, 255, 100); /*Leading edge of dot will be dimmed*/
    } 
    if (pos > 1) { leds[pos - 1] += CHSV(gHue, 255, 100); /*if pos is zero do not send a negetive number to (leds)*/
    }
    Serial.print (pos);
    Serial.print (" , ");
    Serial.print (posDir);
    Serial.print (" , ");
    Serial.print ("cntup showMode = ");
    EVERY_N_MILLISECONDS(SPEED) {
    Serial.println (showMode);
    FastLED.show();
    }
    FastLED.delay(0);
}



void wipe() {
  fadeToBlackBy(leds, NUM_LEDS, 15);
  FastLED.show();
}

/* monitor button press */
void buttonListener() {
  delay(20); /*Reccomend a hardware debounce to ensure flawless response*/
  if (digitalRead(3) == true) {
    showMode = !showMode;
    Serial.print ("Button Press showMode = ");
    Serial.println (showMode);
  }
}

