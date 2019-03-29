#include <Arduino.h>
#include <Keyboard.h>
#include <Wire.h>

#include "globals.h"
#include "oled.h"
#include "keymatrix.h"

#define MASTER_LEFT
// #define FOR_DEBUG
OLED oled;
KeyMatrix keyMatrix;
uint8_t keyStates[COLUMNS];
int currentLayer = 1;
int nextLayer = 0;

int rowPins[ROWS] = {
    18, 15, 14, 16, 10
};

int columnPins[COLUMNS] = {
#ifdef MASTER_LEFT
    1, 6, 7, 8, 9, 5, 4, 0
#else
    1, 0, 4, 5, 9, 8, 7, 6
#endif
};

uint8_t analogKeyMap[5] = {
    KC_A, KC_D, KC_W, KC_S, KC_Z
};

uint8_t keyMap[LAYERS][ALLROWS][COLUMNS] = {
    { //defualt
        {KC_ESC,   KC_n1,   KC_n2,   KC_n3,   KC_n4,  KC_n5,    KC_BQT,   KC_NONE}, 
        {KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,   KC_T,     KC_NONE,  KC_NONE},
        {KC_LCTR,  KC_A,    KC_S,    KC_D,    KC_F,   KC_G,     KC_NONE,  KC_NONE},
        {KC_LSHT,  KC_Z,    KC_X,    KC_C,    KC_V,   KC_B,     KC_NONE,  KC_NONE},
        {KC_RAISE, KC_NONE, KC_LALT, KC_LGUI, KC_SPC, KC_LOWER, KC_ENTER, KC_NONE},

        {KC_n6,  KC_n7,   KC_n8,   KC_n9,   KC_n0,   KC_HYP,  KC_EQL,   KC_NONE},
        {KC_Y,   KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRK, KC_RBRK,  KC_NONE},
        {KC_H,   KC_J,    KC_K,    KC_L,    KC_SCLN, KC_SQT,  KC_ENTER, KC_NONE},
        {KC_N,   KC_M,    KC_CMMA, KC_PRD,  KC_SLH,  KC_UP,   KC_RAISE, KC_NONE},
        {KC_SPC, KC_RGUI, KC_RALT, KC_DEL,  KC_LEFT, KC_DOWN, KC_RIGHT, KC_NONE}
    },
    { //raise
        {KC_ESC,   KC_F1,   KC_F2,   KC_F3,   KC_F4,  KC_F5,    KC_BQT,   KC_NONE}, 
        {KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,   KC_T,     KC_NONE,  KC_NONE},
        {KC_LCTR,  KC_A,    KC_S,    KC_D,    KC_F,   KC_G,     KC_NONE,  KC_NONE},
        {KC_LSHT,  KC_Z,    KC_X,    KC_C,    KC_V,   KC_B,     KC_NONE,  KC_NONE},
        {KC_RAISE, KC_NONE, KC_LALT, KC_LGUI, KC_SPC, KC_LOWER, KC_ENTER, KC_NONE},

        {KC_F6,  KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,  KC_F12,   KC_NONE},
        {KC_Y,   KC_U,    KC_I,    KC_O,    KC_PRSCN, KC_DEL,  KC_BS,    KC_NONE},
        {KC_H,   KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_SQT,  KC_ENTER, KC_NONE},
        {KC_N,   KC_M,    KC_CMMA, KC_PRD,  KC_BSLH,  KC_PGUP, KC_RAISE, KC_NONE},
        {KC_SPC, KC_RGUI, KC_RALT, KC_RCTR, KC_HOME,  KC_PGDN, KC_END,   KC_NONE}
    },
    { //lower
        {KC_ESC,   KC_n1,   KC_n2,   KC_n3,   KC_n4,  KC_n5,    KC_BQT,   KC_NONE}, 
        {KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,   KC_T,     KC_NONE,  KC_NONE},
        {KC_LCTR,  KC_A,    KC_S,    KC_D,    KC_F,   KC_G,     KC_NONE,  KC_NONE},
        {KC_LSHT,  KC_Z,    KC_X,    KC_C,    KC_V,   KC_B,     KC_NONE,  KC_NONE},
        {KC_RAISE, KC_NONE, KC_LALT, KC_LGUI, KC_SPC, KC_LOWER, KC_ENTER, KC_NONE},

        {KC_n6,  KC_n7,   KC_n8,   KC_n9,   KC_n0,   KC_HYP,  KC_EQL,   KC_NONE},
        {KC_Y,   KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRK, KC_RBRK,  KC_NONE},
        {KC_H,   KC_J,    KC_K,    KC_L,    KC_SCLN, KC_SQT,  KC_ENTER, KC_NONE},
        {KC_N,   KC_M,    KC_CMMA, KC_PRD,  KC_BSLH, KC_VLUP, KC_RAISE, KC_NONE},
        {KC_SPC, KC_RGUI, KC_RALT, KC_RCTR, KC_BLDN, KC_VLDN, KC_BLUP,  KC_NONE}
    },
}; 

#ifdef MASTER_LEFT
#else
void wireRequest()
{
    Wire.write(keyMatrix.getRow(0));
    Wire.write(keyMatrix.getRow(1));
    Wire.write(keyMatrix.getRow(2));
    Wire.write(keyMatrix.getRow(3));
    Wire.write(keyMatrix.getRow(4));
}
#endif

void setup()
{
    //setup serial
    Serial.begin(38400);
#ifdef FOR_DEBUG
   while(!Serial);
#endif

    //initialize
    keyMatrix.begin();
#ifdef MASTER_LEFT
    Keyboard.begin();
    // Wire.begin();
    oled.begin();
    delay(3000);
#else
    Wire.begin(I2CADDR_SLAVE);
    Wire.onRequest(wireRequest);
#endif
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
#ifdef MASTER_LEFT
    Serial.println('e');
    Serial.print("S: ");
    for (int i = ROWS; i < ALLROWS; i++) {
        dumpByte(keyMatrix.getRow(i));
        Serial.print(", ");
    }
#endif
    Serial.println('e');
    Serial.println("----");
}

#ifdef MASTER_LEFT
void keyPress(uint8_t keyCode, uint8_t state)
{
    if (state == ST_NOCHANGE) return;
    if (state == ST_RELEASE) {
        Keyboard.release(keyCode);
        return;
    }
    Keyboard.press(keyCode);
}
#endif

void loop()
{
    // own keyscan
    for (int i = 0; i < ROWS; i++) {
        keyMatrix.storeRow(i, keyMatrix.scanRow(i));
    }
#ifdef MASTER_LEFT
    Wire.requestFrom(I2CADDR_SLAVE, 5);
    for (int i = 0; i < ROWS; i++) {
        while (!Wire.available());
        keyMatrix.storeRow(ROWS + i, Wire.read());
    }    
#else
#endif

#ifdef MASTER_LEFT
    // main sequence
    for (int i = 0; i < ALLROWS; i++) {
        keyMatrix.getRowState(i, keyStates);
        for (int j = 0; j < COLUMNS; j++) {
            uint8_t keyCode = keyMap[currentLayer][i][j];
            uint8_t state = keyStates[j];
            if (state != ST_NOCHANGE) {
                switch (keyCode) {
                    case KC_RAISE:
                        {
                            if (state == ST_PRESS) {
                                nextLayer = LAYER_RAISE;
                            } else {
                                nextLayer = LAYER_DEFAULT;
                            }
                        }
                        break;
                    case KC_LOWER:
                        {
                            if (state == ST_PRESS) {
                                nextLayer = LAYER_LOWER;
                            } else {
                                nextLayer = LAYER_DEFAULT;
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

    // Change Layer
    if (currentLayer != nextLayer) {
        currentLayer = nextLayer;
        oled.clear();
        oled.printLayer(currentLayer);
    }
#endif
#ifdef FOR_DEBUG
    //DEBUG: keymatrix
    static int c = 0;
    if ( c % 20 == 0) {
        dumpAllRows();
    }
    c++;
#endif
    keyMatrix.swapBuffers();
    delay(10);
}