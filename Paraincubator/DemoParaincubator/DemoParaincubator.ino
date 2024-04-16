#include "Arduino.h"
#include <FastLED.h>

#define LED_PIN_1   3
#define NUM_LEDS_1  8

#define LED_PIN_2   2   
#define NUM_LEDS_2  32   

CRGB leds_1[NUM_LEDS_1];
CRGB leds_2[NUM_LEDS_2];

void(* resetFunc) (void) = 0;

#define STB (1 << PORTB3)
#define STB_DDR (1 << DDB3)

#define DAT (1 << PORTB4)
#define DAT_DDR (1 << DDB4)

#define AY0 (1 << PORTB0) 
#define AY1 (1 << PORTB1)
#define AY2 (1 << PORTB2)

#define AY0_DDR (1 << DDB0)
#define AY1_DDR (1 << DDB1)
#define AY2_DDR (1 << DDB2)

#define AX0 (1 << PORTC0) //A0
#define AX1 (1 << PORTC1) //A1
#define AX2 (1 << PORTC2) //A2
#define AX3 (1 << PORTC3) //A3

#define AX0_DDR (1 << DDC0) //A0
#define AX1_DDR (1 << DDC1) //A1
#define AX2_DDR (1 << DDC2) //A2
#define AX3_DDR (1 << DDC3) //A3

#define AX_PORT PORTC
#define AY_PORT PORTB
#define CONTROL_PORT PORTB

#define AX_PORT_DDR DDRC
#define AY_PORT_DDR DDRB 
#define CONTROL_PORT_DDR DDRB

#define ADDR0 (1 << PORTD4)
#define ADDR1 (1 << PORTD5)
#define ADDR2 (1 << PORTD6)
#define ADDR3 (1 << PORTD7)

#define ADDR0_DDR (1 << DDD4)
#define ADDR1_DDR (1 << DDD5)
#define ADDR2_DDR (1 << DDD6)
#define ADDR3_DDR (1 << DDD7)

#define ADDR_PORT PORTD
#define ADDR_PORT_DDR DDRD

#define MAX_ADDRESS 15 // change later
#define MAX_AX 15 // 2^n -1
#define MAX_AY 7

int setConnection(uint8_t addr, uint8_t AX, uint8_t AY, bool mode){
  if (addr > MAX_ADDRESS || AX > MAX_AX || AY > MAX_AY){
    return -1;
  }

  ADDR_PORT = (ADDR_PORT & 0x0F) | (addr << 4); /// pazq starta stojnost na 4-te bita koito ne iskam da pipam i zadavam nova stojnost na 4 bita, kojto promenqm

  AX_PORT = (AX_PORT & 0xF0) | AX;

  AY_PORT = (AY_PORT & 0xF8) | AY; 

  if (mode){
    CONTROL_PORT |= DAT;
  }else{
    CONTROL_PORT &= ~DAT;
  }

  CONTROL_PORT |= STB;

  delayMicroseconds(2);

  CONTROL_PORT &= ~STB;

  return 1;
} 


void setup(){

    Serial.begin(9600);

    ADDR_PORT_DDR |= ADDR0_DDR | ADDR1_DDR | ADDR2_DDR | ADDR3_DDR; // Init D Port Arduino

    ADDR_PORT &= ~(ADDR0 | ADDR1 | ADDR2 | ADDR3);


    AY_PORT_DDR |= AY0_DDR | AY1_DDR | AY2_DDR;// Init B Port Arduino

    AY_PORT &= ~(AY0 | AY1 | AY2);


    AX_PORT_DDR |= AX0_DDR | AX1_DDR | AX2_DDR | AX3_DDR;// Init C Port Arduino

    AX_PORT &= ~(AX0 | AX1 | AX2 | AX3);


    CONTROL_PORT_DDR |= STB_DDR | DAT_DDR;

    CONTROL_PORT |= DAT;
    CONTROL_PORT &= ~STB;

    FastLED.addLeds<WS2812, LED_PIN_1, GRB>(leds_1, NUM_LEDS_1);
    FastLED.addLeds<WS2812, LED_PIN_2, GRB>(leds_2, NUM_LEDS_2);

    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 8; y++) {
            setConnection(0b1000, x, y, false);
        }
    }

    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 8; y++) {
            setConnection(0b1001, x, y, false);
        }
    }

    delay(500);
    // LED

    // 2 breadboard -> 4 mcu: (GND)
    setConnection(0b1001, 4, 0, true); 
    setConnection(0b1000, 0, 4, true); 

    // 11 breadboard -> 5 mcu: (P0)
    setConnection(0b1001, 14, 1, true); 
    setConnection(0b1000, 1, 3, true); 

    // Potentiometer

    // 14 breadboard -> 4 mcu: (GND)
    setConnection(0b1000, 9, 3, true); 

    // 16 breadboard -> 2 mcu: (P3)
    setConnection(0b1000, 11, 1, true); 

    // 18 breadboard -> 2 mcu: (VCC)
    setConnection(0b1000, 13, 7, true); 

    // Buzzer

    // 23 breadboard -> 3 mcu
    setConnection(0b1001, 2, 2, true); 
    setConnection(0b1000, 2, 2, true); 

    leds_1[4] =  CRGB(25, 25, 25); // GND
    leds_2[10] =  CRGB(25, 25, 25); // GND
    leds_2[17] =  CRGB(25, 25, 25); // GND

    leds_1[3] =  CRGB(25, 0, 0); // VCC
    leds_2[13] =  CRGB(25, 0, 0); // VCC

    leds_1[6] =  CRGB(0, 0, 25); // P3
    leds_2[15] =  CRGB(0, 0, 25); // P3

    leds_1[0] =  CRGB(0, 25, 0); // P3
    leds_2[0] =  CRGB(0, 25, 0); // P3

    leds_1[5] =  CRGB(35, 15, 0); // P4
    leds_2[22] =  CRGB(35, 15, 0); // P4

    FastLED.show();


}

void loop(){
    
}
