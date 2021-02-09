#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// #include <stdlib.h>

#define BCM2708_PERI_BASE 0x20000000
#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000)
#define BLOCK_SIZE (4 * 1024)

struct bcm2835_peripheral {
    unsigned long addr_p;
    int mem_fd;
    void* map;
    volatile unsigned int* addr;
};

// struct bcm2835_peripheral gpio = {
//     GPIO_BASE
// };

int map_peripheral(struct bcm2835_peripheral* p) {
    // p->mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    p->mem_fd = open("/dev/gpiomem", O_RDWR | O_SYNC);

    if (p->mem_fd < 0) {
        // printf("Failed to open /dev/mem, try checking permissions.\n");
        printf("Failed to open /dev/gpiomem, try checking permissions.\n");
        return -1;
    }

    p->map = mmap(
        NULL,
        BLOCK_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        p->mem_fd,
        p->addr_p);

    if (p->map == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    p->addr = (volatile unsigned int*)p->map;

    return 0;
}

void unmap_peripheral(struct bcm2835_peripheral* p) {
    munmap(p->map, BLOCK_SIZE);
    close(p->mem_fd);
}

// #define INP_GPIO(g) *(gpio.addr + ((g) / 10)) &= ~(7 << (((g) % 10) * 3))
// #define OUT_GPIO(g) *(gpio.addr + ((g) / 10)) |= (1 << (((g) % 10) * 3))
// #define SET_GPIO_ALT(g, a) *(gpio.addr + (((g) / 10))) |= (((a) <= 3 ? (a) + 4 : (a) == 4 ? 3 : 2) << (((g) % 10) * 3))

// #define GPIO_SET *(gpio.addr + 7)   // sets   bits which are 1 ignores bits which are 0
// #define GPIO_CLR *(gpio.addr + 10)  // clears bits which are 1 ignores bits which are 0

// #define GPIO_READ(g) *(gpio.addr + 13) &= (1 << (g))

#define INPUT 0
#define OUTPUT 1

#define LOW 0
#define HIGH 1

class Gpio {
   private:
    // struct bcm2835_peripheral periph = {GPIO_BASE}; // offset for /dev/mem
    struct bcm2835_peripheral periph = {0}; // offset for /dev/gpiomem

   public:
    Gpio() {
        map_peripheral(&periph);
    }

    void pin_mode(int pin, int mode) {
        switch (mode) {
            case INPUT:
                // set mode input
                *(periph.addr + ((pin) / 10)) &= ~(7 << (((pin) % 10) * 3));
                break;

            case OUTPUT:
                // set mode output
                pin_mode(pin, INPUT);
                *(periph.addr + ((pin) / 10)) |= (1 << (((pin) % 10) * 3));
                break;

            default:
                break;
        }
    }

    void pin_set(int pin, int level) {
        switch (level) {
            case LOW:
                // clear pin
                *(periph.addr + 10) = 1 << pin;
                break;

            case HIGH:
                // set pin
                *(periph.addr + 7) = 1 << pin;
                break;

            default:
                break;
        }
    }

    int pin_read(int pin) {
        return (*(periph.addr + 13) &= (1 << pin)) >> pin;
    }

    void pin_toggle(int pin) {
        pin_set(pin, !pin_read(pin));
    }
};
