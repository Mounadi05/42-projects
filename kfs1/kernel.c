#include <stddef.h>
#include <stdint.h>

static volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;
static const int VGA_COLS = 80;
static const int VGA_ROWS = 25;


int strlen(const char *str)
{
    int i = 0;
    while(str[i]) i++;
    return i;
}

void kernel_main(void) {
    const char* msg = "hello from mounadi , kfs1 is done";
    int row = 12, col = 40; 
    int idx = row * VGA_COLS + col;
    int len = strlen(msg);

    for (int i = 0; i < len ; i++) {
        vga_buffer[idx++] = (uint8_t)msg[i] | 0x0200; 
    }

    while (1);
}
