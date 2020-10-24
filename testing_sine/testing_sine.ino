
// Sample Rate = 8000, sineWave has 32 samples which is 1 full wave 
// 1 period = SamplesInWave / SampleRate = 0.004 seconds
// Frequency = 1 / Period = 250 Hz
// Tone generated at 250Hz with an amplitude of .5.
static float sineWave[32] = {0.00000,0.09755,0.19134,0.27779,0.35355,0.41573,0.46194,0.49039,0.50000,0.49039,0.46194,0.41573,0.35355,0.27779,0.19134,0.09755,0.00000,-0.09755,-0.19134,-0.27779,-0.35355,-0.41573,-0.46194,-0.49039,-0.50000,-0.49039,-0.46194,-0.41573,-0.35355,-0.27779,-0.19134,-0.09755};
int sineWaveSize;
static int sampleRate = 8000;

// Sine wave: y = A sin (k (theta - b)) + c
// A = Amplitude, k = periodicity factor, b = left/right shift, c = up/down shift

// 60-250 HZ = base

#include <FastLED.h>
#include <math.h>

// Using 60 Hz - 10 kHz for range of colors
// hue = Log2(freq) - Log2(LOWER_FREQ) / (Log2(UPPER_FREQ) - Log2(LOWER_FREQ)) * 359
#define UPPER_FREQ  10000
#define LOWER_FREQ  60

#define LED_PIN     6
#define NUM_LEDS    60 // Change to 300 for Sam's LED Strip; 60 for Jordi's.
#define BRIGHTNESS  32 // It can be higher, but keep at 32 for now?
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100 // how fast the LEDs change color/brightness per second.

CRGB leds[NUM_LEDS];
TBlendType    currentBlending;
CRGBPalette16 currentPalette;

void setup() { 
  delay(3000); // power-up safety delay
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS); 
  FastLED.setBrightness(BRIGHTNESS);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  sineWaveSize = sizeof(sineWave) / sizeof(float);
}

void FillLEDsFromPaletteColors(uint8_t hue, uint8_t brightness, uint8_t saturation)
{
  // Brightness from 0-255;
  // Saturation from 0-255;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, saturation, brightness);
    Serial.println(hue);
  }
}

float getFreq() {
  return 250; // 250 Hz
}

void loop() { 
  static uint8_t hue = 0;
  static uint8_t brightness = 255;
  static uint8_t saturation = 255;
  
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  float freq = getFreq();

  // Changes color according to sample amplitude's absolute value.
//  if (startIndex >= sineWaveSize) {
//    startIndex = 0;
//  }
//  hue = abs(sineWave[startIndex]) * 359;

  // Changes color based on Frequency.
  hue = ((log(freq)/log(2)) - (log(LOWER_FREQ)/log(2))) / ((log(UPPER_FREQ)/log(2)) - (log(LOWER_FREQ)/log(2))) * 250; // For 60 Hz, Hue = 0; for 10 kHz, Hue = 250
  
  FillLEDsFromPaletteColors(hue, brightness, saturation);
  
  FastLED.show();
  delay(1000 / UPDATES_PER_SECOND);
}
