#ifdef COMBO_ENABLE
#define COMBO_LEN MAX_COMBO;
#include "shared.h"
#include "handlers.h"

//═══════════════════════════════════════════════════════════════
//   KEY COMBOS (simultaneous key presses)
//═══════════════════════════════════════════════════════════════

enum combos {
    C_TD, // [
    C_NB, // ]
    //-------

    C_SDT,  //
    /*
    C_XKC,  //
    C_NBL,  //
    C_UIO,  //
    C_M10,  //
    */
    //-------
    C_M1,   // "
    C_DB,   // '
    C_K1,   // `
    //-------
    /*
    C_CM, // X
    C_GH, // X
    C_YF, // X
    C_JW, // X
    //-------
    C_CW, //X
    C_JM, //X
    */
    //C_HN, // •
    /*
    C_TG, //X
    */
    //-------
    C_ALTQ,  // untab,
    C_PULG,  // num lock
    C_M2S1,  // caps word
    C_FLASH, // flash close
    //-------
    //C_SDT,  // start
    C_NBL,  // end
    //-------
    MAX_COMBO
};

enum comboState{
    CS_TAP_INIT,
    CS_TAP_END,
    CS_HOLD_INIT,
    CS_HOLD_END
};


extern bool caps_word;
extern bool kb_lock;

deferred_token timer;

// Combos definition
const uint16_t PROGMEM SEC_TD [] = 	{ES_T,	 ES_D,	   COMBO_END};
const uint16_t PROGMEM SEC_NB [] = 	{ES_N,	 ES_B,	   COMBO_END};
//-------

const uint16_t PROGMEM SEC_SDT [] = {ES_S,	 ES_D,  ES_T,   COMBO_END};
/*
const uint16_t PROGMEM SEC_XKC [] = {ES_X,	 ES_K,  ES_C,   COMBO_END};
const uint16_t PROGMEM SEC_NBL [] = {ES_N,	 ES_B,  ES_L,   COMBO_END};
const uint16_t PROGMEM SEC_UIO [] = {ES_U,	 ES_I,  ES_O,   COMBO_END};
const uint16_t PROGMEM SEC_M10 [] = {ES_M,	 ES_COMM,   ES_DOT,   COMBO_END};
*/
//-------
const uint16_t PROGMEM SEC_M1 [] =  {ES_M,	 ES_COMM,   COMBO_END};
const uint16_t PROGMEM SEC_DB [] =  {ES_D,	 ES_B,      COMBO_END};
const uint16_t PROGMEM SEC_K1 [] =  {ES_K,	 ES_COMM,   COMBO_END};
//-------
/*
const uint16_t PROGMEM SEC_CM [] =  {ES_C,	 ES_M,  COMBO_END};
const uint16_t PROGMEM SEC_GH [] = 	{ES_G,	 ES_H,  COMBO_END};
const uint16_t PROGMEM SEC_YF [] = 	{ES_Y,	 ES_F,  COMBO_END};
const uint16_t PROGMEM SEC_JW [] = 	{ES_J,	 ES_W,  COMBO_END};
//-------
const uint16_t PROGMEM SEC_CW [] = 	{ES_C,	 ES_W,  COMBO_END};
const uint16_t PROGMEM SEC_JM [] = 	{ES_J,	 ES_M,  COMBO_END};
*/
//const uint16_t PROGMEM SEC_HN [] = 	{ES_H,	 ES_N,  COMBO_END};
/*
const uint16_t PROGMEM SEC_TG [] = 	{ES_T,	 ES_G,	   COMBO_END};
*/
//-------
//const uint16_t PROGMEM SEC_PDEL [] =  {DEL,	 ES_P,  COMBO_END};
const uint16_t PROGMEM SEC_ALTQ [] =    {ALT,   ES_Q,   COMBO_END};
const uint16_t PROGMEM SEC_PULG [] =    {KC_SPC,M3,     COMBO_END};
const uint16_t PROGMEM SEC_M2S1 [] =    {M2,    S1,     COMBO_END};
const uint16_t PROGMEM SEC_FLASH [] =   {M7,    TILDE,  COMBO_END};
//-------
//const uint16_t PROGMEM SEC_SDT [] =  {ES_S,	 ES_D,  ES_T,   COMBO_END};
const uint16_t PROGMEM SEC_NBL [] =  {ES_N,	 ES_B,  ES_L,   COMBO_END};


//Combos binding
combo_t key_combos[MAX_COMBO] = {
    [ C_TD ] = COMBO( SEC_TD, ES_LBRC), // [
    [ C_NB ] = COMBO( SEC_NB, ES_RBRC), // ]
    //-------
    /*
    [ C_XKC ] = COMBO( SEC_XKC , ES_X), //
    [ C_NBL ] = COMBO( SEC_NBL , ES_X), //
    [ C_UIO ] = COMBO( SEC_UIO , ES_X), //
    [ C_M10 ] = COMBO( SEC_M10 , ES_X), //
    */
    //-------
    [ C_M1 ] = COMBO( SEC_M1, ES_DQUO), // "
    [ C_DB ] = COMBO( SEC_DB, ES_QUOT), // '
    [ C_K1 ] = COMBO( SEC_K1, BCKQT),   // `
    //-------
    /*
    [ C_CM ] = COMBO( SEC_CM , ES_X),
    [ C_GH ] = COMBO( SEC_GH , ES_X),
    [ C_YF ] = COMBO( SEC_YF , ES_X),
    [ C_JW ] = COMBO( SEC_JW , ES_X),
    //-------
    [ C_CW ] = COMBO( SEC_CW , ES_X),.
    [ C_JM ] = COMBO( SEC_JM , ES_X),
    */
   // [ C_HN ] = COMBO( SEC_HN , BULLET),
    /*
    [ C_TG ] = COMBO( SEC_TG , SCRSHT1),
    */
    //-------
    [ C_ALTQ ] = COMBO( SEC_ALTQ, UNTAB ),
    [ C_PULG ] = COMBO( SEC_PULG, NLOCK),
    [ C_M2S1 ] = COMBO_ACTION( SEC_M2S1 ),
    [ C_FLASH ] = COMBO_ACTION( SEC_FLASH ),
    //-------
    [ C_SDT ] = COMBO( SEC_SDT, KC_HOME), // START
    [ C_NBL ] = COMBO( SEC_NBL, KC_END),  // END
};
//SCRSHT1



// Prevent combos in some cases
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    // xprintf("shouldtrigger %d\n", combo_index);
    return true;
}


static uint16_t t = 0;
static uint16_t dt = 0;
static uint16_t current_combo;

// TO-DO: mejorar, aumentar y comentar esto
void on_combo_step(uint16_t combo_index, uint8_t combo_step){
    switch(combo_step){
        case CS_TAP_INIT:
            //xprintf("tap init\n");
        break;
        case CS_TAP_END:
            //xprintf("tap end\n");
            if(combo_index == C_M2S1) set_caps(!caps_word, false, true);
        break;
        case CS_HOLD_INIT:
            //xprintf("hold init\n");
            if(combo_index == C_M2S1) trigger_mod_combo(M2_M + S1_M, true);
        break;
        case CS_HOLD_END:
            //xprintf("hold end\n");
            if(combo_index == C_M2S1) trigger_mod_combo(M2_M + S1_M, false);
        break;
    }
}

uint32_t timer_over(uint32_t trigger_time, void *cb_arg) {
    on_combo_step(current_combo, CS_HOLD_INIT);
    return 0;
}

void process_combo_event(uint16_t combo_index, bool pressed) {
    if(kb_lock) return;
    // TO-DO: esto lo tengo que reescribir
    dt = timer_elapsed(t);
    t = timer_read();
    if(pressed){
        interrupt_mods();
        timer = defer_exec( COMBO_HOLD_TIME, timer_over, NULL );
        current_combo = combo_index;
    }else{
        cancel_deferred_exec( timer );
    }
    on_combo_step(combo_index, pressed ? CS_TAP_INIT : (dt < COMBO_HOLD_TIME ? CS_TAP_END : CS_HOLD_END));

/*
    switch(combo_index) {

        case C_SDT:
            //tap_code16( getOSKey( COPY ) );
            //tap_os_dependent_key( SCRSHT1 );
            break;
        case C_M2S1:
            if(!pressed){
                cancel_deferred_exec(timer);
                if(dt < COMBO_HOLD_TIME){
                    //xprintf("combo end m2s1!\n");
                    //xprintf("t: %d!\n", dt);
                    interrupt_mods();
                    set_caps(true, false, true);
                    xprintf("end caps! \n");
                }else{
                    xprintf("se te pasó el arroz\n");
                }
            }else{
                xprintf("init caps..\n");
                timer = defer_exec(COMBO_HOLD_TIME, timer_over, NULL);
            }
            break;

        case C_FLASH:
            if(!pressed) return;
            tap_code16( XN(KC_F) );
            break;

    }
    */

}


// Per-combo timing
uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    switch (index) {
        case C_PULG:
            return 30;
    }
    return COMBO_TERM;
}
#endif
