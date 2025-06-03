#include "shared.h"

//═══════════════════════════════════════════════════════════════
//   SEND STRING (for OS with ISO-ES input method)
//═══════════════════════════════════════════════════════════════

// Sends a string on an OS with ISO-ES keyboard input method
void send_string_spanish(const char *str) {
    while (*str) {
        uint32_t code = decode_utf8_string(&str);

        switch (code) {
            // Accents
            case 0xE1: tap_code16(ES_ACUT); tap_code(ES_A); break;      // á
            case 0xC1: tap_code16(ES_ACUT); tap_code16(S(ES_A)); break; // Á
            case 0xE9: tap_code16(ES_ACUT); tap_code(ES_E); break;      // é
            case 0xC9: tap_code16(ES_ACUT); tap_code16(S(ES_E)); break; // É
            case 0xED: tap_code16(ES_ACUT); tap_code(ES_I); break;      // í
            case 0xCD: tap_code16(ES_ACUT); tap_code16(S(ES_I)); break; // Í
            case 0xF3: tap_code16(ES_ACUT); tap_code(ES_O); break;      // ó
            case 0xD3: tap_code16(ES_ACUT); tap_code16(S(ES_O)); break; // Ó
            case 0xFA: tap_code16(ES_ACUT); tap_code(ES_U); break;      // ú
            case 0xDA: tap_code16(ES_ACUT); tap_code16(S(ES_U)); break; // Ú

            // Diaeresis
            case 0xFC: tap_code16(S(ES_ACUT)); tap_code(ES_U); break;       // ü
            case 0xDC: tap_code16(S(ES_ACUT)); tap_code16(S(ES_U)); break;  // Ü

            // Ñ/ñ y Ç/ç
            case 0xF1: tap_code(ES_NTIL); break;        // ñ
            case 0xD1: tap_code16(S(ES_NTIL)); break;   // Ñ
            case 0xE7: tap_code(ES_CCED); break;        // ç
            case 0xC7: tap_code16(S(ES_CCED)); break;   // Ç

            // Special symbols
            case 0xA1: tap_code16(ES_IEXL); break;  // ¡
            case 0xBF: tap_code16(ES_IQUE); break;  // ¿
            case 0xAA: tap_code16(ES_FORD); break;  // ª
            case 0xBA: tap_code(ES_MORD); break;    // º

            // Simbols that depend on the operating system
            case 0x20AC: tap_os_dependent_key(EURO); break;  // €
            case 0x7E:   tap_os_dependent_key(WAVE); break;  // ~

            // Shift + numbers
            case 0x21: tap_code16(ES_EXLM); break;  // !
            case 0x22: tap_code16(ES_DQUO); break;  // "
            case 0xB7: tap_code16(ES_BULT); break;  // ·
            case 0x24: tap_code16(ES_DLR);  break;  // $
            case 0x25: tap_code16(ES_PERC); break;  // %
            case 0x26: tap_code16(ES_AMPR); break;  // &
            case 0x2F: tap_code16(ES_SLSH); break;  // /
            case 0x28: tap_code16(ES_LPRN); break;  // (
            case 0x29: tap_code16(ES_RPRN); break;  // )
            case 0x3D: tap_code16(ES_EQL);  break;  // =
            case 0x3F: tap_code16(ES_QUES); break;  // ?

            // AltGr
            case 0x5C: tap_code16(ES_BSLS); break;  // backslash
            case 0x7C: tap_code16(ES_PIPE); break;  // |
            case 0x40: tap_code16(ES_AT);   break;  // @
            case 0x23: tap_code16(ES_HASH); break;  // #
            case 0xAC: tap_code16(ES_NOT);  break;  // ¬
            case 0x5B: tap_code16(ES_LBRC); break;  // [
            case 0x5D: tap_code16(ES_RBRC); break;  // ]
            case 0x7B: tap_code16(ES_LCBR); break;  // {
            case 0x7D: tap_code16(ES_RCBR); break;  // }
            case 0x3B: tap_code16(ES_SCLN); break;  // ;
            case 0x3A: tap_code16(ES_COLN); break;  // :
            case 0x5F: tap_code16(ES_UNDS); break;  // _

            default:
                send_char(code);
            break;
        }
    }
}
