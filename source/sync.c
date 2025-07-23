#include "sync.h"
#include "transactions.h"
#include "rgb.h"
#include "split_util.h"

//═══════════════════════════════════════════════════════════════
//	SYNC MODULE ( used for communication between master and slave )
//═══════════════════════════════════════════════════════════════


#define MAX_QUEUE_SIZE 8

extern bool master;
extern bool kb_synced;
extern bool kb_lock;
extern bool reactive_rgb;

uint8_t sync_queue_index = 0;
uint16_t sync_queue[MAX_QUEUE_SIZE ];
uint8_t head = 0, tail = 0;

// Adds a message to the circular queue if space is available.
void enqueue(uint16_t msg) {
    if ((tail + 1) % MAX_QUEUE_SIZE != head) { //check if queue is full
        sync_queue[tail] = msg;
        tail = (tail + 1) % MAX_QUEUE_SIZE;
    }
}

// Retrieves the next message from the circular queue; removes it only if should_delete is true.
uint16_t dequeue(bool should_delete) {
    if (head == tail) return 0; // check if queue is empty
    uint16_t msg = sync_queue[head];
    if (should_delete) head = (head + 1) % MAX_QUEUE_SIZE;
    return msg;
}

// An attempt is made to send the message to the slave.
// If it's not successful, it remains enqueued.
void master_sync(uint16_t msg){
    uint8_t ack = 0;
    if (transaction_rpc_exec(RPC_ID_CUSTOM_SYNC, sizeof(msg), &msg, sizeof(ack), &ack)) {
        if (ack == 1) {
            dequeue(true);
        }
    }
}

// Synchronizes an event, with optional value and modifier, to the slave
// by encoding the message into a single 16-bit integer (max value: 65535).
void sync(enum sync_events type, uint8_t value, uint8_t modifier) {
    if (!master) return;
    // Encode the message:
    // - type: bits 15-12 (4 bits, range 0-15)
    // - value: bits 11-5 (7 bits, range 0-127)
    // - modifier: bits 4-0 (5 bits, range 0-31)
    uint16_t msg = ((type & 0x0F) << 12) | ((value & 0x7F) << 5) | (modifier & 0x1F);
    enqueue(msg);
}

// Decodes a 16-bit synchronization message (max value: 65535).
void retrieve_sync_msg(uint16_t msg) {
    // - type: bits 15-12 (4 bits, range 0-15)
    uint8_t type = (msg >> 12) & 0x0F;
    // - value: bits 11-5 (7 bits, range 0-127)
    uint8_t value = (msg >> 5) & 0x7F;
    // - modifier: bits 4-0 (5 bits, range 0-31)
    uint8_t modifier = msg & 0x1F;
    process_sync_slave(type, value, modifier);
}

// Captures a synchronization message from the master half
void slave_sync(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    uint16_t msg;
    if (initiator2target_buffer_size == sizeof(msg)) {
        memcpy(&msg, initiator2target_buffer, initiator2target_buffer_size);
        retrieve_sync_msg(msg);
        if (target2initiator_buffer_size >= sizeof(uint8_t)) {
            uint8_t *ack = (uint8_t *)target2initiator_buffer;
            *ack = 1;
        }
    }
}

// Init sync (keyboard_post_init_user)
void init_sync(void) {
    if(!master){
        transaction_register_rpc(RPC_ID_CUSTOM_SYNC, slave_sync);
    }
}

// Check if the keyboard is synced. Display a notification when paired
bool kb_is_synced(void) {
    if (!master || kb_synced)  return kb_synced;
    if (is_transport_connected()) {
        kb_synced = true;
        sync(SYNC_KB_PAIRED, 1, 0);
        rgb_set_notification(RGB_BOOT_NOTIFICATION);
    }
    return kb_synced;
}

//	 MAIN SYNC LOOP
//–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

// Currently, communication is directional only: master → slave
void process_sync_queue(void){
    if(master){
        uint16_t msg = dequeue( false );
        if(msg) master_sync(msg);
    }
}

//   SYNC EVENTS
//–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
// Executed on the slave when a synchronization message is received.
void process_sync_slave(enum sync_events type, uint8_t value, uint8_t modifier){
    if(master) return;
    // debug_slave(value, modifier + type*8);
    // return;
    switch(type){

        // – – – – KB EVENTS – – – – – – – – – – – – – – – – –
        case SYNC_KB_PAIRED:
            kb_synced = value;
            break;
        case SYNC_KB_LOCK:
            kb_lock = value;
            break;
        case SYNC_KB_KEYPRESS:
            #ifdef RGB_MATRIX_ENABLE
            if( reactive_rgb ){
                const uint8_t row = value >> 3;
                const uint8_t col = value & 0x07;
                rgb_keyboard_event( col, row, modifier );
            }
            #endif
            break;
        case SYNC_KB_SUSPEND:
            #ifdef RGB_MATRIX_ENABLE
            rgb_set_suspend( value );
            #endif
            break;

        // – – – – RGB EVENTS – – – – – – – – – – – – – – – – –
        #ifdef RGB_MATRIX_ENABLE
        case SYNC_RGB_BRIGHTNESS: break;
        case SYNC_RGB_INDICATOR:
            rgb_set_indicator( value, (bool) modifier );
            break;
        case SYNC_RGB_NOTIFICATION:
            rgb_set_notification( value);
            break;
        case SYNC_RGB_THEME:
            rgb_set_theme( value );
            break;
        case SYNC_RGB_REACTIVE:
            rgb_set_reactive( value );
            break;
        case SYNC_RGB_ANIMATION:
            rgb_set_animation( value );
            break;
        #endif
        default: break;
    }

}

