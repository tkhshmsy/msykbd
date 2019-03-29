#include <Arduino.h>

#include <globals.h>
#include <analogstick.h>

AnalogStick::AnalogStick()
    : m_currentBuffer(0)
{
    m_buffer[0] = 0x00;
    m_buffer[1] = 0x00;
}

AnalogStick::~AnalogStick()
{}

void AnalogStick::begin()
{
    pinMode(19, INPUT_PULLUP);
}

void AnalogStick::end()
{}

uint8_t AnalogStick::getRow()
{
    return m_buffer[m_currentBuffer];
}

void AnalogStick::getState(uint8_t *state)
{
    uint8_t diff = m_buffer[0] ^ m_buffer[1];
    uint8_t curr = m_buffer[m_currentBuffer];
    for (int i = 0; i < 8; i++) {
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

void AnalogStick::swapBuffers()
{
    if (m_currentBuffer == 0) {
        m_currentBuffer = 1;
    } else {
        m_currentBuffer = 0;
    }
}

void AnalogStick::scan()
{
    int h = analogRead(A3);
    int v = analogRead(A2);
    m_buffer[m_currentBuffer] = 0x00;
    if (h < 256 + 64) {
        m_buffer[m_currentBuffer] |= 0x80;
    }
    if (h > 768 - 64) {
        m_buffer[m_currentBuffer] |= 0x40;
    }
    if (v < 256) {
        m_buffer[m_currentBuffer] |= 0x20;
    }
    if (v > 768) {
        m_buffer[m_currentBuffer] |= 0x10;
    }
    if (digitalRead(19) == LOW) {
        m_buffer[m_currentBuffer] |= 0x08;
    }
}
