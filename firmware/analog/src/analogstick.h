#ifndef __ANALOGSTICK__
#define __ANALOGSTICK__

class AnalogStick {
public:
    AnalogStick();
    ~AnalogStick();

    void begin();
    void end();

    void scan();
    uint8_t getRow();
    void getState(uint8_t *state);
    void swapBuffers();
private:
    uint8_t m_buffer[2];
    int m_currentBuffer;
};

#endif