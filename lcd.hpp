#include <assert.h>
#include <cstring>
#include "gpio.hpp"

void sleep_ms(int ms) {
    usleep(1000 * ms);
}

int char_code(char c) {
    switch (c) {
    #pragma region symbols
        case ' ':
            return 0b0010'0000;

        case '!':
            return 0b0010'0001;

        case '"':
            return 0b0010'0010;

        case '#':
            return 0b0010'0011;

        case '$':
            return 0b0010'0100;

        case '%':
            return 0b0010'0101;

        case '&':
            return 0b0010'0110;

        case '\'':
            return 0b0010'0111;

        case '(':
            return 0b0010'1000;

        case ')':
            return 0b0010'1001;

        case '*':
            return 0b0010'1010;

        case '+':
            return 0b0010'1011;

        case ',':
            return 0b0010'1100;

        case '-':
            return 0b0010'1101;

        case '.':
            return 0b0010'1110;

        case '/':
            return 0b0010'1111;

        case ':':
            return 0b0011'1010;

        case ';':
            return 0b0011'1011;

        case '<':
            return 0b0011'1100;

        case '=':
            return 0b0011'1101;

        case '>':
            return 0b0011'1110;

        case '?':
            return 0b0011'1111;

        case '@':
            return 0b0100'0000;

        case '[':
            return 0b0101'1011;

        case ']':
            return 0b0101'1101;

        case '^':
            return 0b0101'1110;

        case '_':
            return 0b0101'1111;

        case '`':
            return 0b0110'0000;

        case '{':
            return 0b0111'1011;

        case '|':
            return 0b0111'1100;

        case '}':
            return 0b0111'1101;
#pragma endregion

    #pragma region numeric
        case '0':
            return 0b0011'0000;

        case '1':
            return 0b0011'0001;

        case '2':
            return 0b0011'0010;

        case '3':
            return 0b0011'0011;

        case '4':
            return 0b0011'0100;

        case '5':
            return 0b0011'0101;

        case '6':
            return 0b0011'0110;

        case '7':
            return 0b0011'0111;

        case '8':
            return 0b0011'1000;

        case '9':
            return 0b0011'1001;
#pragma endregion

    #pragma region alpha_upper
        case 'A':
            return 0b0100'0001;

        case 'B':
            return 0b0100'0010;

        case 'C':
            return 0b0100'0011;

        case 'D':
            return 0b0100'0100;

        case 'E':
            return 0b0100'0101;

        case 'F':
            return 0b0100'0110;

        case 'G':
            return 0b0100'0111;

        case 'H':
            return 0b0100'1000;

        case 'I':
            return 0b0100'1001;

        case 'J':
            return 0b0100'1010;

        case 'K':
            return 0b0100'1011;

        case 'L':
            return 0b0100'1100;

        case 'M':
            return 0b0100'1101;

        case 'N':
            return 0b0100'1110;

        case 'O':
            return 0b0100'1111;

        case 'P':
            return 0b0101'0000;

        case 'Q':
            return 0b0101'0001;

        case 'R':
            return 0b0101'0010;

        case 'S':
            return 0b0101'0011;

        case 'T':
            return 0b0101'0100;

        case 'U':
            return 0b0101'0101;

        case 'V':
            return 0b0101'0110;

        case 'W':
            return 0b0101'0111;

        case 'X':
            return 0b0101'1000;

        case 'Y':
            return 0b0101'1001;

        case 'Z':
            return 0b0101'1010;
#pragma endregion

    #pragma region alpha_lower
        case 'a':
            return 0b0110'0001;

        case 'b':
            return 0b0110'0010;

        case 'c':
            return 0b0110'0011;

        case 'd':
            return 0b0110'0100;

        case 'e':
            return 0b0110'0101;

        case 'f':
            return 0b0110'0110;

        case 'g':
            return 0b0110'0111;

        case 'h':
            return 0b0110'1000;

        case 'i':
            return 0b0110'1001;

        case 'j':
            return 0b0110'1010;

        case 'k':
            return 0b0110'1011;

        case 'l':
            return 0b0110'1100;

        case 'm':
            return 0b0110'1101;

        case 'n':
            return 0b0110'1110;

        case 'o':
            return 0b0110'1111;

        case 'p':
            return 0b0111'0000;

        case 'q':
            return 0b0111'0001;

        case 'r':
            return 0b0111'0010;

        case 's':
            return 0b0111'0011;

        case 't':
            return 0b0111'0100;

        case 'u':
            return 0b0111'0101;

        case 'v':
            return 0b0111'0110;

        case 'w':
            return 0b0111'0111;

        case 'x':
            return 0b0111'1000;

        case 'y':
            return 0b0111'1001;

        case 'z':
            return 0b0111'1010;
#pragma endregion

        default:
            return 0;
    }
}


// 4-bit mode (send high order bits, then low order bits)
// d7 -> d7, d3
// d6 -> d6, d2
// d5 -> d5, d1
// d4 -> d4, d0

#define NO_EFFECT 0

#define DATA_LENGTH_4_BITS 0
#define DATA_LENGTH_8_BITS 1

#define N_LINES_1 0
#define N_LINES_2 1

#define CHAR_FONT_5_x_8 0
#define CHAR_FONT_5_x_10 1

#define NOT_BUSY 0
#define BUSY 1

#define S_DISPLAY_NO_SHIFT 0
#define S_DISPLAY_SHIFT 1

#define ENTRY_MODE_DECREMENT 0
#define ENTRY_MODE_INCREMENT 1

#define RS_MODE_INSTRUCTION 0
#define RS_MODE_DATA 1

#define DISPLAY_SHIFT 1
#define CURSOR_MOVE 0
#define DIRECTION_LEFT 0
#define DIRECTION_RIGHT 1


class hd44780 {
   private:
    int columns = 16;
    int rows = 2;

    int rs;         // register select, 0: instruction, 1: data
    int en;         // busy flag?, 1: start data read/write, 0: end data read/write
    int d0;         // low order data bus 0
    int d1;         // low order data bus 1
    int d2;         // low order data bus 2
    int d3;         // low order data bus 3
    int d4;         // high order data bus 4
    int d5;         // high order data bus 5
    int d6;         // high order data bus 6
    int d7;         // high order data bus 7
    int backlight;  //

    int display;
    int cursor;
    int blink;

   public:
    Gpio gpio;

    hd44780(int rs, int en, int d4, int d5, int d6, int d7, int backlight) {
        this->rs = rs;
        this->en = en;

        this->d7 = d7;
        this->d6 = d6;
        this->d5 = d5;
        this->d4 = d4;

        this->d3 = d7;
        this->d2 = d6;
        this->d1 = d5;
        this->d0 = d4;

        this->backlight = backlight;

        gpio.pin_mode(rs, OUTPUT);
        gpio.pin_mode(en, OUTPUT);
        gpio.pin_mode(d4, OUTPUT);
        gpio.pin_mode(d5, OUTPUT);
        gpio.pin_mode(d6, OUTPUT);
        gpio.pin_mode(d7, OUTPUT);
        gpio.pin_mode(backlight, OUTPUT);
        
        init();
    }

    void write_4bits(int mode, int data7, int data6, int data5, int data4) {
        assert(gpio.pin_read(en) == 0);

        // start write
        gpio.pin_set(en, HIGH);

        // set register select to instruction
        gpio.pin_set(rs, RS_MODE_INSTRUCTION);

        gpio.pin_set(d7, data7);
        gpio.pin_set(d6, data6);
        gpio.pin_set(d5, data5);
        gpio.pin_set(d4, data4);

        // end write
        gpio.pin_set(en, LOW);

        // wait for execution
        sleep_ms(2);
    }

    void write_8bits(int mode, int data7, int data6, int data5, int data4, int data3, int data2, int data1, int data0) {
        // high order bits
        write_4bits(mode, data7, data6, data5, data4);

        // low order bits
        write_4bits(mode, data3, data2, data1, data0);
    }

    void init() {
        write_4bits(RS_MODE_INSTRUCTION, 0, 0, 1, 1);
        sleep_ms(5);
        write_4bits(RS_MODE_INSTRUCTION, 0, 0, 1, 1);
        write_4bits(RS_MODE_INSTRUCTION, 0, 0, 1, 1);
        write_4bits(RS_MODE_INSTRUCTION, 0, 0, 1, 0);

        function_set(DATA_LENGTH_4_BITS, N_LINES_2, CHAR_FONT_5_x_8);

        display_control(true, false, false);

        entry_mode_set(ENTRY_MODE_INCREMENT, S_DISPLAY_NO_SHIFT);

        clear();
    }

    void function_set(int data_length, int n_lines, int char_font) {
        write_8bits(RS_MODE_INSTRUCTION, 0, 0, 1, data_length, n_lines, char_font, NO_EFFECT, NO_EFFECT);
    }

    void display_control(int display, int cursor, int blink) {
        write_8bits(RS_MODE_INSTRUCTION, 0, 0, 0, 0, 1, display, cursor, blink);

        this->display = display;
        this->cursor = cursor;
        this->blink = blink;
    }

    void display_set(int enable) {
        display_control(enable, cursor, blink);
    }

    void cursor_set(int enable) {
        display_control(display, enable, blink);
    }

    void blink_set(int enable) {
        display_control(display, cursor, enable);
    }

    void clear() {
        write_8bits(RS_MODE_INSTRUCTION, 0, 0, 0, 0, 0, 0, 0, 1);
    }

    void home() {
        write_8bits(RS_MODE_INSTRUCTION, 0, 0, 0, 0, 0, 0, 1, NO_EFFECT);
    }

    void entry_mode_set(int text_direction, int display_shift) {
        write_8bits(RS_MODE_INSTRUCTION, 0, 0, 0, 0, 0, 1, text_direction, display_shift);
    }

    void cursor_move(int direction) {
        write_8bits(RS_MODE_INSTRUCTION, 0, 0, 0, 1, CURSOR_MOVE, direction, NO_EFFECT, NO_EFFECT);
    }

    void display_shift(int direction) {
        write_8bits(RS_MODE_INSTRUCTION, 0, 0, 0, 1, DISPLAY_SHIFT, direction, NO_EFFECT, NO_EFFECT);
    }

    void send_char_code(int char_code) {
        if (char_code > 0) {
            int c7 = (char_code & 0b1000'0000) >> 7;
            int c6 = (char_code & 0b0100'0000) >> 6;
            int c5 = (char_code & 0b0010'0000) >> 5;
            int c4 = (char_code & 0b0001'0000) >> 4;
            int c3 = (char_code & 0b0000'1000) >> 3;
            int c2 = (char_code & 0b0000'0100) >> 2;
            int c1 = (char_code & 0b0000'0010) >> 1;
            int c0 = (char_code & 0b0000'0001) >> 0;

            assert(gpio.pin_read(en) == 0);

            gpio.pin_set(rs, RS_MODE_DATA);

            // high order bits
            gpio.pin_set(en, HIGH);
            gpio.pin_set(d7, c7);
            gpio.pin_set(d6, c6);
            gpio.pin_set(d5, c5);
            gpio.pin_set(d4, c4);
            gpio.pin_set(en, LOW);

            assert(gpio.pin_read(en) == 0);

            // low order bits
            gpio.pin_set(en, HIGH);
            gpio.pin_set(d3, c3);
            gpio.pin_set(d2, c2);
            gpio.pin_set(d1, c1);
            gpio.pin_set(d0, c0);
            gpio.pin_set(en, LOW);

            sleep_ms(2);
        }
    }

    void send_char(char c) {
        if (c == '\n') {
            cursor_set_pos(2, 0);
            return;
        }

        int code = char_code(c);

        if (code != 0) {
            int c7 = (code & 0b1000'0000) >> 7;
            int c6 = (code & 0b0100'0000) >> 6;
            int c5 = (code & 0b0010'0000) >> 5;
            int c4 = (code & 0b0001'0000) >> 4;
            int c3 = (code & 0b0000'1000) >> 3;
            int c2 = (code & 0b0000'0100) >> 2;
            int c1 = (code & 0b0000'0010) >> 1;
            int c0 = (code & 0b0000'0001) >> 0;

            assert(gpio.pin_read(en) == 0);

            gpio.pin_set(rs, RS_MODE_DATA);

            // high order bits
            gpio.pin_set(en, HIGH);
            gpio.pin_set(d7, c7);
            gpio.pin_set(d6, c6);
            gpio.pin_set(d5, c5);
            gpio.pin_set(d4, c4);
            gpio.pin_set(en, LOW);

            assert(gpio.pin_read(en) == 0);

            // low order bits
            gpio.pin_set(en, HIGH);
            gpio.pin_set(d3, c3);
            gpio.pin_set(d2, c2);
            gpio.pin_set(d1, c1);
            gpio.pin_set(d0, c0);
            gpio.pin_set(en, LOW);

            sleep_ms(2);
        }
    }

    void send_string(const char* str) {
        for (int i = 0; i < strlen(str); i++) {
            this->send_char(str[i]);
        }
    }

    void set_ddram_address(int pos) {
        int c6 = (pos & 0b0100'0000) >> 6;
        int c5 = (pos & 0b0010'0000) >> 5;
        int c4 = (pos & 0b0001'0000) >> 4;
        int c3 = (pos & 0b0000'1000) >> 3;
        int c2 = (pos & 0b0000'0100) >> 2;
        int c1 = (pos & 0b0000'0010) >> 1;
        int c0 = (pos & 0b0000'0001) >> 0;

        write_8bits(RS_MODE_INSTRUCTION, 1, c6, c5, c4, c3, c2, c1, c0);
    }

    void cursor_set_pos(int row, int column) {
        if (row == 1) {
            set_ddram_address(column - 1);
        }
        else if (row == 2) {
            set_ddram_address(column + 64 - 1);
        }
    }
};
