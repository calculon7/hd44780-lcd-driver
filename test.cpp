#include "lcd.hpp"

int main(int argc, char** argv) {
    hd44780 lcd = hd44780(25, 24, 23, 17, 18, 22, 4);
    lcd.send_string("hello world");
}
