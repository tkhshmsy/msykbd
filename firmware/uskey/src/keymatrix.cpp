#include <Arduino.h>

#include "globals.h"
#include "keymatrix.h"

KeyMatrix::KeyMatrix()
    : m_currentBuffer(0)
    , m_prevBuffer(1)
{
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < ALLROWS; j++) {
            m_buffer[i][j] = 0x00;
        }
    }
}

KeyMatrix::~KeyMatrix()
{}

void KeyMatrix::begin()
{
    for (int i = 0; i < ROWS; i++) {
        pinMode(rowPins[i], OUTPUT);
    }
    for (int i = 0; i < COLUMNS; i++) {
        pinMode(columnPins[i], INPUT_PULLUP);
    }
}

void KeyMatrix::end()
{
}

void KeyMatrix::storeRow(int row, uint8_t data)
{
    m_buffer[m_currentBuffer][row] = data;
}

uint8_t KeyMatrix::getRow(int row)
{
    return m_buffer[m_currentBuffer][row];
}

void KeyMatrix::getRowState(int row, uint8_t *state)
{
    uint8_t diff = m_buffer[0][row] ^ m_buffer[1][row];
    uint8_t curr = m_buffer[m_currentBuffer][row];

    for (int i = 0; i < COLUMNS; i++) {
        if (diff & 0x80) {
            if (curr & 0x80) {
                state[i] = ST_PRESS;
            } else {
                state[i] = ST_RELEASE;
            }
        } else {
            state[i] = ST_NOCHANGE;
        }
        diff = diff << 1;
        curr = curr << 1;
    }
}

void KeyMatrix::swapBuffers()
{
    if (m_currentBuffer == 0) {
        m_currentBuffer = 1;
        m_prevBuffer = 0;
    } else {
        m_currentBuffer = 0;
        m_prevBuffer = 1;
    }
}

uint8_t KeyMatrix::scanRow(int row)
{
    for (int i = 0; i < ROWS; i++) {
        if (i != row) {
            digitalWrite(rowPins[i], HIGH);
        } else {
            digitalWrite(rowPins[i], LOW);
        }
    }
    uint8_t result = 0x00;
    for (int i = 0; i < COLUMNS; i++) {
        result = result >> 1;
        if (digitalRead(columnPins[i]) == LOW) {
            result |= 0x80;
        }
    }
    return result;
}