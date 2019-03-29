#include <Arduino.h>
#include <Keyboard.h>
#include <Wire.h>

#include "globals.h"
#include "keymatrix.h"
#include "oled.h"
#include "analogstick.h"

// #define FOR_DEBUG

KeyMatrix keyMatrix;
AnalogStick analogStick;
OLED oled;
uint8_t keyStates[COLUMNS];
int currentLayer = 1;
int nextLayer = 0;
int waitingLayer = nextLayer;
int waitingCounter = 0;

int rowPins[ROWS] = {
    18, 15, 14, 16, 10
};

int columnPins[COLUMNS] = {
    1, 6, 7, 8, 9, 5, 4, 0
};

uint8_t keyMap[LAYERS][ALLROWS][COLUMNS] = {
    { //defualt : PSO2
        {KC_DFLT,  KC_RAISE, KC_LOWER, KC_MEDIA, KC_PRSCN, KC_F1,    KC_F8,   KC_NONE}, 
        {KC_I,     KC_D,     KC_ESC,   KC_SPC,   KC_X,     KC_N,     KC_n8,   KC_NONE},
        {KC_U,     KC_R,     KC_LSHT,  KC_Q,     KC_E,     KC_ENTER, KC_n7,   KC_NONE},
        {KC_n9,    KC_n1,    KC_n2,    KC_n3,    KC_n4,    KC_n5,    KC_n6,   KC_NONE},
        {KC_G,     KC_B,     KC_F11,   KC_n0,    KC_F12,   KC_F5,    KC_F6,   KC_NONE},
        {KC_A,     KC_P,     KC_S,     KC_W,     KC_Z,     KC_NONE,  KC_NONE, KC_NONE}
    },
    { //raise
        {KC_DFLT,  KC_RAISE, KC_LOWER, KC_MEDIA, KC_PRSCN, KC_F1,    KC_F8,   KC_NONE}, 
        {KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,   KC_T,     KC_NONE, KC_NONE},
        {KC_LCTR,  KC_A,    KC_S,    KC_D,    KC_F,   KC_G,     KC_NONE, KC_NONE},
        {KC_LSHT,  KC_Z,    KC_X,    KC_C,    KC_V,   KC_B,     KC_NONE, KC_NONE},
        {KC_RAISE, KC_NONE, KC_LALT, KC_LGUI, KC_SPC, KC_LOWER, KC_NONE, KC_NONE},
        {KC_A,     KC_D,    KC_S,    KC_W,    KC_Z,   KC_NONE,  KC_NONE, KC_NONE}
    },
    { //lower
        {KC_DFLT,  KC_RAISE, KC_LOWER, KC_MEDIA, KC_PRSCN, KC_F1,    KC_F8,   KC_NONE}, 
        {KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,   KC_T,     KC_NONE, KC_NONE},
        {KC_LCTR,  KC_A,    KC_S,    KC_D,    KC_F,   KC_G,     KC_NONE, KC_NONE},
        {KC_LSHT,  KC_Z,    KC_X,    KC_C,    KC_V,   KC_B,     KC_NONE, KC_NONE},
        {KC_RAISE, KC_NONE, KC_LALT, KC_LGUI, KC_SPC, KC_LOWER, KC_NONE, KC_NONE},
        {KC_A,     KC_D,    KC_S,    KC_W,    KC_Z,   KC_NONE,  KC_NONE, KC_NONE}
    },
    { //media : Fusion360
        {KC_DFLT,  KC_RAISE, KC_LOWER, KC_MEDIA, KC_SCLCK, KC_F1,    KC_F8,   KC_NONE}, 
        {KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,   KC_T,     KC_NONE, KC_NONE},
        {KC_LCTR,  KC_A,    KC_S,    KC_D,    KC_F,   KC_G,     KC_NONE, KC_NONE},
        {KC_LSHT,  KC_Z,    KC_X,    KC_C,    KC_V,   KC_B,     KC_NONE, KC_NONE},
        {KC_RAISE, KC_NONE, KC_LALT, KC_LGUI, KC_SPC, KC_LOWER, KC_NONE, KC_NONE},
        {KC_A,     KC_D,    KC_S,    KC_W,    KC_Z,   KC_NONE,  KC_NONE, KC_NONE}
    },
}; 

void setup()
{
    //setup serial
#ifdef FOR_DEBUG
    Serial.begin(38400);
    while(!Serial);
#endif

    //initialize
    keyMatrix.begin();
    analogStick.begin();
    oled.begin();
    delay(2000);
}

void dumpByte(uint8_t byte)
{
    for (int i = 0; i < 8; i++) {
        if (byte & 0x80) {
            Serial.print('1');
        } else {
            Serial.print('0');
        }
        byte = byte << 1;
    }
}

void dumpAllRows()
{
    Serial.print("M: ");
    for (int i = 0; i < ROWS; i++) {
        dumpByte(keyMatrix.getRow(i));
        Serial.print(", ");
    }
    Serial.print("A: ");
    dumpByte(keyMatrix.getRow(ROWS));
    Serial.println('e');
    Serial.println("----");
}

void keyPress(uint8_t keyCode, uint8_t state)
{
    if (state == ST_NOCHANGE) return;
    if (state == ST_RELEASE) {
        Keyboard.release(keyCode);
        return;
    }
    Keyboard.press(keyCode);
}

void loop()
{
    // own keyscan
    for (int i = 0; i < ROWS; i++) {
        keyMatrix.storeRow(i, keyMatrix.scanRow(i));
    }
    analogStick.scan();
    keyMatrix.storeRow(ROWS, analogStick.getRow());
    for (int i = 0; i < ALLROWS; i++) {
        if (i < ROWS) {
            keyMatrix.getRowState(i, keyStates);
        } else {
            analogStick.getState(keyStates);            
        }
        for (int j = 0; j < COLUMNS; j++) {
            uint8_t keyCode = keyMap[currentLayer][i][j];
            uint8_t state = keyStates[j];
            if (state != ST_NOCHANGE) {
                switch (keyCode) {
                    case KC_DFLT:
                        {
                            if (state == ST_PRESS) {
                                waitingLayer = LAYER_DEFAULT;
                            } else {
                                if (nextLayer != waitingLayer) {
                                    waitingLayer = nextLayer;
                                }
                            }
                        }
                        break;
                    case KC_RAISE:
                        {
                            if (state == ST_PRESS) {
                                waitingLayer = LAYER_RAISE;
                            } else {
                                if (nextLayer != waitingLayer) {
                                    waitingLayer = nextLayer;
                                }
                            }
                        }
                        break;
                    case KC_LOWER:
                        {
                            if (state == ST_PRESS) {
                                waitingLayer = LAYER_LOWER;
                            } else {
                                if (nextLayer != waitingLayer) {
                                    waitingLayer = nextLayer;
                                }
                            }
                        }
                        break;
                    case KC_MEDIA:
                        {
                            if (state == ST_PRESS) {
                                waitingLayer = LAYER_MEDIA;
                            } else {
                                if (nextLayer != waitingLayer) {
                                    waitingLayer = nextLayer;
                                }
                            }
                        }
                        break;
                    case KC_NONE:
                        //no effect
                        break;
                    default:
                        keyPress(keyCode, state);
                        break;
                }
            }
        }
    }
    //Change Layer with long push
    if (nextLayer != waitingLayer) {
        if (waitingCounter > 300) {
            nextLayer = waitingLayer;
            waitingCounter = 0;
        }
        waitingCounter++;
    } else {
        waitingCounter = 0;
    }

    // Change Layer
    if (currentLayer != nextLayer) {
        currentLayer = nextLayer;
        oled.clear();
        oled.printLayer(currentLayer);
    }
#ifdef FOR_DEBUG
    //DEBUG: keymatrix
    static int c = 0;
    if ( c % 20 == 0) {
        dumpAllRows();
    }
    c++;
#endif
    keyMatrix.swapBuffers();
    analogStick.swapBuffers();
    delay(10);
}