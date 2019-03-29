#ifndef __GLOBALS__
#define __GLOBALS__

#include <Arduino.h>
#include <Keyboard.h>

//wire
#define I2CADDR_OLED (0x3c)
#define I2CADDR_SLAVE (8)

//oled
#define OLED_WIDTH (128)
#define OLED_HEIGHT (32)

//keymatrix
#define ST_PRESS (0x01)
#define ST_RELEASE (0xff)
#define ST_NOCHANGE (0x00)

#define LAYERS (3)
#define ROWS (5)
#define ALLROWS (ROWS * 2)
#define COLUMNS (8)

#define LAYER_DEFAULT (0)
#define LAYER_RAISE (1)
#define LAYER_LOWER (2)
#define LAYER_MEDIA (3)

#define KC_n1 '1'
#define KC_n2 '2'
#define KC_n3 '3'
#define KC_n4 '4'
#define KC_n5 '5'
#define KC_n6 '6'
#define KC_n7 '7'
#define KC_n8 '8'
#define KC_n9 '9'
#define KC_n0 '0'
#define KC_A 'a'
#define KC_B 'b'
#define KC_C 'c'
#define KC_D 'd'
#define KC_E 'e'
#define KC_F 'f'
#define KC_G 'g'
#define KC_H 'h'
#define KC_I 'i'
#define KC_J 'j'
#define KC_K 'k'
#define KC_L 'l'
#define KC_M 'm'
#define KC_N 'n'
#define KC_O 'o'
#define KC_P 'p'
#define KC_Q 'q'
#define KC_R 'r'
#define KC_S 's'
#define KC_T 't'
#define KC_U 'u'
#define KC_V 'v'
#define KC_W 'w'
#define KC_X 'x'
#define KC_Y 'y'
#define KC_Z 'z'
#define KC_BQT '`'
#define KC_HYP '-'
#define KC_EQL '='
#define KC_RBRK ']'
#define KC_LBRK '['
#define KC_BSLH '\\'
#define KC_SCLN ';'
#define KC_SQT '\''
#define KC_CMMA ','
#define KC_PRD '.'
#define KC_SLH '/'

#define KC_ESC KEY_ESC
#define KC_TAB KEY_TAB
#define KC_INS KEY_INSERT
#define KC_DEL KEY_DELETE
#define KC_BS KEY_BACKSPACE
#define KC_ENTER KEY_RETURN
#define KC_PGUP KEY_PAGE_UP
#define KC_PGDN KEY_PAGE_DOWN
#define KC_HOME KEY_HOME
#define KC_END KEY_END
#define KC_PAUSE (0xD0)
#define KC_PRSCN (0xCE)
#define KC_SCLCK (0xCF)

#define KC_LCTR KEY_LEFT_CTRL
#define KC_LSHT KEY_LEFT_SHIFT
#define KC_LALT KEY_LEFT_ALT
#define KC_LGUI KEY_LEFT_GUI
#define KC_RCTR KEY_RIGHT_CTRL
#define KC_RSHT KEY_RIGHT_SHIFT
#define KC_RALT KEY_RIGHT_ALT
#define KC_RGUI KEY_RIGHT_GUI
#define KC_SPC ' '
#define KC_UP KEY_UP_ARROW
#define KC_DOWN KEY_DOWN_ARROW
#define KC_LEFT KEY_LEFT_ARROW
#define KC_RIGHT KEY_RIGHT_ARROW

#define KC_VLUP 0xff
#define KC_VLDN 0xff
#define KC_MUTE 0xff
#define KC_BLUP 0xff
#define KC_BLDN 0xff

#define KC_F1 KEY_F1
#define KC_F2 KEY_F2
#define KC_F3 KEY_F3
#define KC_F4 KEY_F4
#define KC_F5 KEY_F5
#define KC_F6 KEY_F6
#define KC_F7 KEY_F7
#define KC_F8 KEY_F8
#define KC_F9 KEY_F9
#define KC_F10 KEY_F10
#define KC_F11 KEY_F11
#define KC_F12 KEY_F12

#define KC_DFLT 0xf0
#define KC_RAISE 0xf1
#define KC_LOWER 0xf2
#define KC_MEDIA 0xf3
#define KC_NONE 0xff

extern int rowPins[ROWS];
extern int columnPins[COLUMNS];
extern uint8_t keyMap[LAYERS][ALLROWS][COLUMNS];

#endif