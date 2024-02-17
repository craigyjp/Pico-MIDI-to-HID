#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

#define UART_ID uart0
#define BAUD_RATE 31250 // MIDI baud rate
#define UART_RX_PIN 1
#define UART_TX_PIN 0

// Add more keycodes as needed

#define LED_PIN PICO_DEFAULT_LED_PIN // Onboard LED pin

struct midi_cc
{
    uint8_t status; // MIDI status byte
    uint8_t data1;  // MIDI data byte 1 (CC number)
    uint8_t data2;  // MIDI data byte 2 (CC value)
};

void handle_midi_cc(void);

int main(void)
{

    // Example MIDI CC message
    uint8_t midi_cc_msg[] = {0xB0, 0x07, 0x64};

    // Interpret the MIDI CC message
    struct midi_cc cc_msg;
    cc_msg.status = midi_cc_msg[0];
    cc_msg.data1 = midi_cc_msg[1];
    cc_msg.data2 = midi_cc_msg[2];

    board_init();
    tusb_init();

    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);

    // Initialize LED pin
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (1)
    {
        tud_task(); // tinyusb device task
        if (tud_hid_n_ready(0))
        {
            handle_midi_cc();
        }
    }

    return 0;
}

void handle_midi_cc(void)
{
    struct midi_cc cc_msg;

    if (uart_is_readable(UART_ID))
    {
        uart_read_blocking(UART_ID, &cc_msg.status, 1);
        uint8_t kcode[6] = {0};
        // Check if it's a MIDI CC message
        if ((cc_msg.status & 0xF0) == 0xB0)
        {                                                  // MIDI CC status byte starts with 0xB0
            uart_read_blocking(UART_ID, &cc_msg.data1, 1); // Read CC number
            uart_read_blocking(UART_ID, &cc_msg.data2, 1); // Read CC value

            switch (cc_msg.data1)
            {
            case 0x01:
                // Send 'A' key as keyboard input
                kcode[0] = 0x04;
                break;
            case 0x02:
                // Send 'B' key as keyboard input
                kcode[0] = 0x05;
                break;
            case 0x03:
                // Send 'C' key as keyboard input
                kcode[0] = 0x06;
                break;
            case 0x04:
                // Send 'D' key as keyboard input
                kcode[0] = 0x07;
                break;
            case 0x05:
                // Send 'E' key as keyboard input
                kcode[0] = 0x08;
                break;
            case 0x06:
                // Send 'A' key as keyboard input
                kcode[0] = 0x09;
                break;
            case 0x07:
                // Send 'B' key as keyboard input
                kcode[0] = 0x0A;
                break;
            case 0x08:
                // Send 'C' key as keyboard input
                kcode[0] = 0x0B;
                break;
            case 0x09:
                // Send 'D' key as keyboard input
                kcode[0] = 0x0C;
                break;
            case 0x0A:
                // Send 'E' key as keyboard input
                kcode[0] = 0x0D;
                break;
            case 0x0B:
                // Send 'A' key as keyboard input
                kcode[0] = 0x0E;
                break;
            case 0x0C:
                // Send 'B' key as keyboard input
                kcode[0] = 0x0F;
                break;
            case 0x0D:
                // Send 'C' key as keyboard input
                kcode[0] = 0x10;
                break;
            case 0x0E:
                // Send 'D' key as keyboard input
                kcode[0] = 0x11;
                break;
            case 0x0F:
                // Send 'E' key as keyboard input
                kcode[0] = 0x12;
                break;
            case 0x10:
                // Send 'A' key as keyboard input
                kcode[0] = 0x13;
                break;
            case 0x11:
                // Send 'B' key as keyboard input
                kcode[0] = 0x14;
                break;
            case 0x12:
                // Send 'C' key as keyboard input
                kcode[0] = 0x15;
                break;
            case 0x13:
                // Send 'D' key as keyboard input
                kcode[0] = 0x16;
                break;
            case 0x14:
                // Send 'E' key as keyboard input
                kcode[0] = 0x17;
                break;
            }
            tud_hid_n_keyboard_report(0, 0, 0, kcode);

            // Check if it's the correct MIDI CC
            if ((cc_msg.status & 0xF0) == 0xB0)
            {
                // Flash the LED
                gpio_put(LED_PIN, 1); // Turn on LED
                sleep_ms(100);        // Wait 100 milliseconds
                gpio_put(LED_PIN, 0); // Turn off LED
                sleep_ms(100);        // Wait 100 milliseconds
            }
        }
    }
    // Release the key after sending
    tud_hid_n_keyboard_report(0, 0, 0, NULL);
    // cc_msg.status = 0;
    // cc_msg.data1 = 0;
    // cc_msg.data2 = 0;
}

// USB HID Callbacks
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    (void)itf;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;
    return 0;
}

void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    (void)itf;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)bufsize;
}

// USB Device Callbacks
void tud_mount_cb(void)
{
}

void tud_umount_cb(void)
{
}

void tud_suspend_cb(bool remote_wakeup_en)
{
    (void)remote_wakeup_en;
}

void tud_resume_cb(void)
{
}
