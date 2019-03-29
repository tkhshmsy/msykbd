#ifndef __KEYMATRIX__
#define __KEYMATRIX__

#include "globals.h"

class KeyMatrix {
public:
    KeyMatrix();
    ~KeyMatrix();

    void begin();
    void end();

    uint8_t scanRow(int row);
    void storeRow(int row, uint8_t data);    
    uint8_t getRow(int row);
    void getRowState(int row, uint8_t *state);
    void swapBuffers();

private:
    uint8_t m_buffer[2][ALLROWS];
    int m_currentBuffer;
    int m_prevBuffer;
};

#endif