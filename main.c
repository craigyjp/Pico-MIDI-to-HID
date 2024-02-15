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

#define LED_PIN PICO_DEFAULT_LED_PIN // Onboard LED pin

struct midi_cc {
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
        handle_midi_cc();
    }

    return 0;
}

void handle_midi_cc(void)
{
    struct midi_cc cc_msg;

    if (uart_is_readable(UART_ID)) {
        uart_read_blocking(UART_ID, &cc_msg.status, 1);

        // Check if it's a MIDI CC message
        if ((cc_msg.status & 0xF0) == 0xB0) { // MIDI CC status byte starts with 0xB0
            uart_read_blocking(UART_ID, &cc_msg.data1, 1); // Read CC number
            uart_read_blocking(UART_ID, &cc_msg.data2, 1); // Read CC value

            // Send ASCII codes for CC number and value
            char ascii_cc_num[4];
            char ascii_cc_val[4];
            sprintf(ascii_cc_num, "%d", cc_msg.data1);
            sprintf(ascii_cc_val, "%d", cc_msg.data2);
            
            tud_hid_report(0x00, (uint8_t*)ascii_cc_num, strlen(ascii_cc_num));
            tud_hid_report(0x00, (uint8_t*)ascii_cc_val, strlen(ascii_cc_val));

                        // Check if it's the correct MIDI CC
            if (cc_msg.data1 == 0x07) { // Example: MIDI CC number 0x07
                // Flash the LED
                    gpio_put(LED_PIN, 1); // Turn on LED
                    sleep_ms(100); // Wait 100 milliseconds
                    gpio_put(LED_PIN, 0); // Turn off LED
                    sleep_ms(100); // Wait 100 milliseconds
            }
        }
    }
}

// USB HID Callbacks
uint16_t tud_hid_get_report_cb(uint8_t index, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    // Not used
    return 0;
}

void tud_hid_set_report_cb(uint8_t index, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    // Not used
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
    (void) remote_wakeup_en;
}

void tud_resume_cb(void)
{
}
