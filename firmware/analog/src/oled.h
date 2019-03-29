#ifndef __OLED__
#define __OLED__

class OLED {
public:
    OLED();
    ~OLED();

    void begin();
    void end();

    void display();
    void clear();
    void setSize(int size);
    void locate(int x, int y);
    void print(char *data);

    void printLayer(int layer);
private:
    int m_fontSize;
};


#endif