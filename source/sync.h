#pragma once
#include <stdbool.h>
#include <stdint.h>

// Types of events to notify the slave half (max value: 15)
enum sync_events{
    SYNC_RGB_BRIGHTNESS = 0,
    SYNC_RGB_INDICATOR,
    SYNC_RGB_NOTIFICATION,
    SYNC_RGB_THEME,
    SYNC_RGB_REACTIVE,
    SYNC_KB_PAIRED,
    SYNC_KB_LOCK,
    SYNC_KB_SUSPEND,
    SYNC_KB_KEYPRESS,
    SYNC_RGB_ANIMATION
};

void init_sync(void);
bool kb_is_synced(void);
void process_sync_queue(void);
void process_sync_slave(enum sync_events type, uint8_t value, uint8_t modifier);
void sync(enum sync_events type, uint8_t value, uint8_t modifier);
