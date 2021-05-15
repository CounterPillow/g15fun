#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "hidraw_lib.h"
#include "miku.h"
#include "fumo.h"

#define G15_LCD_OFFSET 32
#define NUM_FRAMES 2
#define FRAME_SIZE 960

static const struct lib_hidraw_id hidraw_ids[] = {
    /* G15 */
    { { BUS_USB, 0x046d, 0xc222 } },
    /* G15 v2 */
    { { BUS_USB, 0x046d, 0xc227 } },
    /* G510 without a headset plugged in */
    { { BUS_USB, 0x046d, 0xc22d },
      { 0x05, 0x0c, 0x09, 0x01, 0xa1, 0x01, 0x85, 0x02,
        0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x07 } },
    /* G510 with headset plugged in / with extra USB audio interface */
    { { BUS_USB, 0x046d, 0xc22e },
      { 0x05, 0x0c, 0x09, 0x01, 0xa1, 0x01, 0x85, 0x02,
        0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x07 } },
    /* Z-10 */
    { { BUS_USB, 0x046d, 0x0a07 },
      { 0x06, 0x00, 0xff, 0x09, 0x00, 0xa1, 0x01, 0x15,
        0x00, 0x26, 0xff, 0x00, 0x75, 0x08, 0x95, 0x08 } },
    /* Terminator */
    {}
};

/* LCD buffer: first G15_LCD_OFFSET (=32) bytes are for control things, remaining
 * 960 bytes are for the data the user wants to send.
 *
 * Each byte is an 8 pixel column, whereby the least significant bit is the top
 * most pixel and the most significant bit is the bottom most pixel. See
 * convimg.py for some code on how to convert from a monochrome bitmap to that.
 *
 * The first byte of lcdbuf sets the mode to put the lcd in; we want 0x03.
 */
static uint8_t lcdbuf[G15_LCD_OFFSET + FRAME_SIZE];

void init_lcdbuf() {
    // Note: lcdbuf[1] through lcdbuf[G15_LCD_OFFSET - 1] need to be 0
    memset(lcdbuf, 0, sizeof(lcdbuf));
    // Which mode we want to enter, in this case, pixel buffer
    lcdbuf[0] = 0x03;
}

/* Copies len bytes of buf into the right position of the target
 * LCD buffer to be sent to the device
 */
void submit_buf(uint8_t* buf, size_t len) {
    memcpy(lcdbuf + G15_LCD_OFFSET, buf, len);
}

/* Sends the LCD buffer to the device, displaying it.
 */
void present_lcd(struct lib_hidraw_handle* h) {
    lib_hidraw_send_output_report(h, lcdbuf, sizeof(lcdbuf));
}

int main(int argc, char** argv) {
    uint8_t* frames[] = {miku, fumo};
    // Open the correct /dev/hidraw* device, i.e. with one of those USB IDs.
    struct lib_hidraw_handle* hid_handle = lib_hidraw_open(hidraw_ids);
    if(!hid_handle) {
        printf("No G15 detected :(\n");
        return -1;
    }
    init_lcdbuf();
    unsigned int cur_frame = 0;
    while(1) {
        submit_buf(frames[cur_frame], FRAME_SIZE);
        present_lcd(hid_handle);
        sleep(2);
        cur_frame = (cur_frame + 1) % NUM_FRAMES;
    }
    lib_hidraw_close(hid_handle);
    return 0;
}
