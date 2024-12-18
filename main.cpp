#include <iostream>
#include <string.h>
#include "uart_utils.h"

uint8_t nav_pvt_msg[] = {
        0xB5, 0x62, // header
        0x01, 0x07, // class
        0x5C, 0x00, // length
        0x70, 0x2D, 0xB4, 0x12, 0xE8, 0x07, 0x0A, 0x10, 0x0F, 0x09,
        0x28, 0x37, 0x83, 0x00, 0x00, 0x00, 0x5E, 0xCB, 0xFB, 0xFF, 0x03, 0x03, 0xEA, 0x06, 0x05, 0xCB,
        0xA4, 0x16, 0x57, 0xBC, 0x1D, 0x21, 0x6B, 0x68, 0x02, 0x00, 0xB0, 0x35, 0x02, 0x00, 0x65, 0x4A,
        0x00, 0x00, 0x99, 0x55, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0xC9, 0xFF, 0xFF, 0xFF, 0x84, 0x00,
        0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x80, 0x6A, 0xF0, 0x01, 0x23, 0x02, 0x00, 0x00, 0x80, 0xA8,
        0x12, 0x01, 0xE7, 0x00, 0x00, 0x00, 0x80, 0x6C, 0x4A, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
        0xD7, 0xD9
};

uint8_t nav_sat_msg[] = {
        0xB5,0x62,0x01,0x35,0x70,0x01,0xD0,0x4E,0x55,0x1D,0x01,0x1E,0x00,0x00,0x00,0x02,
        0x10,0x1F,0x84,0x00,0x35,0xFF,0x14,0x59,0x00,0x00,0x00,0x07,0x13,0x56,0xC0,0x00,
        0x6C,0xFF,0x1C,0x59,0x00,0x00,0x00,0x08,0x20,0x34,0x36,0x00,0x03,0x00,0x1F,0x59,
        0x00,0x00,0x00,0x09,0x10,0x12,0xC7,0x00,0x92,0xFF,0x1C,0x59,0x00,0x00,0x00,0x0D,
        0x16,0x0F,0x40,0x01,0x9A,0xFF,0x1C,0x59,0x00,0x00,0x00,0x0E,0x1C,0x25,0x17,0x01,
        0xFC,0xFF,0x1F,0x59,0x00,0x00,0x00,0x11,0x08,0x0F,0xD7,0x00,0x5A,0x00,0x14,0x54,
        0x00,0x00,0x00,0x15,0x0F,0x24,0x71,0x00,0x3E,0x00,0x1C,0x59,0x00,0x00,0x00,0x16,
        0x0F,0x12,0x0E,0x01,0x24,0x00,0x1C,0x59,0x00,0x00,0x00,0x1B,0x20,0x10,0x2E,0x00,
        0xF2,0xFF,0x1F,0x59,0x00,0x00,0x00,0x1E,0x1B,0x39,0x31,0x01,0x44,0x00,0x1F,0x59,
        0x00,0x00,0x01,0x83,0x00,0x22,0xDB,0x00,0x00,0x00,0x11,0x12,0x00,0x00,0x01,0x85,
        0x00,0x1B,0xE8,0x00,0x00,0x00,0x11,0x12,0x00,0x00,0x01,0x8A,0x00,0x26,0xCF,0x00,
        0x00,0x00,0x01,0x07,0x00,0x00,0x02,0x04,0x0A,0x11,0xC1,0x00,0xC4,0x00,0x1C,0x59,
        0x00,0x00,0x02,0x05,0x14,0x19,0x33,0x01,0x7A,0xFF,0x1C,0x59,0x00,0x00,0x02,0x09,
        0x16,0x29,0xF4,0x00,0x60,0x00,0x1C,0x59,0x00,0x00,0x02,0x0C,0x00,0x04,0xA8,0x00,
        0x00,0x00,0x11,0x52,0x00,0x00,0x02,0x0D,0x0A,0x01,0x15,0x00,0xC5,0x02,0x14,0x59,
        0x00,0x00,0x02,0x0E,0x22,0x1F,0x38,0x00,0x00,0x00,0x27,0x19,0x00,0x00,0x02,0x18,
        0x09,0x1F,0x15,0x01,0x77,0x00,0x1C,0x59,0x00,0x00,0x02,0x1A,0x1A,0x1C,0x41,0x00,
        0x18,0x00,0x1F,0x59,0x00,0x00,0x02,0x1F,0x23,0x49,0x56,0x01,0xF2,0xFF,0x1F,0x59,
        0x00,0x00,0x02,0x21,0x11,0x1E,0x79,0x00,0x80,0xFF,0x1C,0x54,0x00,0x00,0x03,0x08,
        0x00,0xA5,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x03,0x13,0x12,0x0D,0x14,0x01,
        0xC2,0xFF,0x1C,0x59,0x00,0x00,0x03,0x14,0x00,0x06,0xE7,0x00,0x00,0x00,0x11,0x59,
        0x00,0x00,0x03,0x17,0x00,0x0E,0x6D,0x00,0x00,0x00,0x11,0x59,0x00,0x00,0x03,0x2B,
        0x19,0x0E,0x28,0x00,0x63,0x00,0x1C,0x49,0x00,0x00,0x03,0x2E,0x0A,0x33,0x33,0x01,
        0x16,0x01,0x13,0x49,0x00,0x00,0x85,0x0F };


uint8_t msg1[] = {
        0xB5, 0x62, // header
        0x01, 0x07, // class
        0x5C, 0x00, // length
        0x28, 0x39, 0xB4, 0x12, 0xE8, 0x07, 0x0A, 0x10, 0x0F, 0x09,
        0x2B, 0x37, 0x83, 0x00, 0x00, 0x00, 0x6C, 0xCC, 0xFB, 0xFF, 0x03, 0x03, 0xEA, 0x06, 0xFC, 0xCA,
        0xA4, 0x16, 0x56, 0xBC, 0x1D, 0x21, 0xAA, 0x68, 0x02, 0x00, 0xEF, 0x35, 0x02, 0x00, 0x8D, 0x4A,
        0x00, 0x00, 0xC6, 0x55, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00, 0xD0, 0xFF, 0xFF, 0xFF, 0x84, 0x00,
        0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x80, 0x6A, 0xF0, 0x01, 0x25, 0x02, 0x00, 0x00, 0x80, 0xA8,
        0x12, 0x01, 0xE7, 0x00, 0x00, 0x00, 0x80, 0x6C, 0x4A, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
        0x83, 0xF3
};

uint8_t msg2[] = {
        0xB5, 0x62, // header
        0x01, 0x07, // class
        0x5C, 0x00, // length
        0x58, 0x31, 0xB4, 0x12, 0xE8, 0x07, 0x0A, 0x10, 0x0F, 0x09,
        0x29, 0x37, 0x83, 0x00, 0x00, 0x00, 0xB8, 0xCB, 0xFB, 0xFF, 0x03, 0x03, 0xEA, 0x06, 0x03, 0xCB,
        0xA4, 0x16, 0x57, 0xBC, 0x1D, 0x21, 0x8A, 0x68, 0x02, 0x00, 0xD0, 0x35, 0x02, 0x00, 0x71, 0x4A,
        0x00, 0x00, 0xA6, 0x55, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0xDC, 0xFF, 0xFF, 0xFF, 0x7F, 0x00,
        0x00, 0x00, 0x69, 0x00, 0x00, 0x00, 0x80, 0x6A, 0xF0, 0x01, 0x26, 0x02, 0x00, 0x00, 0x80, 0xA8,
        0x12, 0x01, 0xE7, 0x00, 0x00, 0x00, 0x80, 0x6C, 0x4A, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
        0x64, 0x1B
};


uint8_t fake_msg_1[] = {
        0xB5, 0x62, // header
        0x01, 0x07, // class
        0x50, 0x00, // length
        0x70, 0x2D, 0xB4, 0x12, 0xE8, 0x07, 0x0A, 0x10, 0x0F, 0x09,
        0x28, 0x37, 0x83, 0x00, 0x00, 0x00, 0x5E, 0xCB, 0xFB, 0xFF, 0x03, 0x03, 0xEA, 0x06, 0x05, 0xCB,
        0xA4, 0x16, 0x57, 0xBC, 0x1D, 0x21, 0x6B, 0x68, 0x02, 0x00, 0xB0, 0x35, 0x02, 0x00, 0x65, 0x4A,
        0x00, 0x00, 0x99, 0x55, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0xC9, 0xFF, 0xFF, 0xFF, 0x84, 0x00,
        0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x80, 0x6A, 0xF0, 0x01, 0x23, 0x02, 0x00, 0x00, 0x80, 0xA8,
        0x12, 0x01, 0xE7, 0x00, 0x00, 0x00, 0x80, 0x6C, 0x4A, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
        0xD7, 0xD9
};

uint8_t fake_msg_2[] = {
        0xB5, 0x60, // header
        0x01, 0x07, // class
        0x5C, 0x00, // length
        0x70, 0x2D, 0xB4, 0x12, 0xE8, 0x07, 0x0A, 0x10, 0x0F, 0x09,
        0x28, 0x37, 0x83, 0x00, 0x00, 0x00, 0x5E, 0xCB, 0xFB, 0xFF, 0x03, 0x03, 0xEA, 0x06, 0x05, 0xCB,
        0xA4, 0x16, 0x57, 0xBC, 0x1D, 0x21, 0x6B, 0x68, 0x02, 0x00, 0xB0, 0x35, 0x02, 0x00, 0x65, 0x4A,
        0x00, 0x00, 0x99, 0x55, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0xC9, 0xFF, 0xFF, 0xFF, 0x84, 0x00,
        0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x80, 0x6A, 0xF0, 0x01, 0x23, 0x02, 0x00, 0x00, 0x80, 0xA8,
        0x12, 0x01, 0xE7, 0x00, 0x00, 0x00, 0x80, 0x6C, 0x4A, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
        0xD7, 0xD9
};



int main(int argc, char* argv[]) {

    ubx_receiver receiver;

    for (int i = 0; i < sizeof (nav_pvt_msg); ++i) {
        receiver.add_byte(nav_pvt_msg[i]);
    }

    for (int i = 0; i < sizeof (nav_sat_msg); ++i) {
        receiver.add_byte(nav_sat_msg[i]);
    }

    if(receiver.data_ready())
        std::cout << "OK!!!" << std::endl;

    receiver.get_msg_buffer().data_used();

    for (int i = 0; i < sizeof (nav_sat_msg); ++i) {
        receiver.add_byte(nav_sat_msg[i]);
    }

    for (int i = 0; i < sizeof (nav_pvt_msg); ++i) {
        receiver.add_byte(nav_pvt_msg[i]);
    }

    if(! receiver.data_ready())
        std::cout << "OK!!!" << std::endl;

    for (int i = 0; i < sizeof (nav_sat_msg); ++i) {
        receiver.add_byte(nav_sat_msg[i]);
    }

    if(receiver.data_ready())
        std::cout << "OK!!!" << std::endl;

    return 0;
}
