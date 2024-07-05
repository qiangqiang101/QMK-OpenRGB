/* Copyright 2019 Jason Williams (Wilba)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "eeconfig.h" // for EECONFIG_SIZE

// Keyboard level code can change where VIA stores the magic.
// The magic is the build date YYMMDD encoded as BCD in 3 bytes,
// thus installing firmware built on a different date to the one
// already installed can be detected and the EEPROM data is reset.
// The only reason this is important is in case EEPROM usage changes
// and the EEPROM was not explicitly reset by bootmagic lite.
#ifndef VIA_EEPROM_MAGIC_ADDR
#    define VIA_EEPROM_MAGIC_ADDR (EECONFIG_SIZE)
#endif

#define VIA_EEPROM_LAYOUT_OPTIONS_ADDR (VIA_EEPROM_MAGIC_ADDR + 3)

// Changing the layout options size after release will invalidate EEPROM,
// but this is something that should be set correctly on initial implementation.
// 1 byte is enough for most uses (i.e. 8 binary states, or 6 binary + 1 ternary/quaternary )
#ifndef VIA_EEPROM_LAYOUT_OPTIONS_SIZE
#    define VIA_EEPROM_LAYOUT_OPTIONS_SIZE 1
#endif

// Allow override of the layout options via value.
// This requires advanced knowledge of how VIA stores layout options
// and is only really useful for setting a boolean layout option
// state to true by via.
#ifndef VIA_EEPROM_LAYOUT_OPTIONS_via
#    define VIA_EEPROM_LAYOUT_OPTIONS_via 0x00000000
#endif

// The end of the EEPROM memory used by VIA
// By via, dynamic keymaps will start at this if there is no
// custom config
#define VIA_EEPROM_CUSTOM_CONFIG_ADDR (VIA_EEPROM_LAYOUT_OPTIONS_ADDR + VIA_EEPROM_LAYOUT_OPTIONS_SIZE)

#ifndef VIA_EEPROM_CUSTOM_CONFIG_SIZE
#    define VIA_EEPROM_CUSTOM_CONFIG_SIZE 0
#endif

#define VIA_EEPROM_CONFIG_END (VIA_EEPROM_CUSTOM_CONFIG_ADDR + VIA_EEPROM_CUSTOM_CONFIG_SIZE)

// This is changed only when the command IDs change,
// so VIA Configurator can detect compatible firmware.
#define VIA_PROTOCOL_VERSION 0x000A

enum via_command_id {
    id_get_protocol_version                 = 0x01, // always 0x01
    id_get_keyboard_value                   = 0x02,
    id_set_keyboard_value                   = 0x03,
    id_dynamic_keymap_get_keycode           = 0x04,
    id_dynamic_keymap_set_keycode           = 0x05,
    id_dynamic_keymap_reset                 = 0x06,
    id_lighting_set_value                   = 0x07,
    id_lighting_get_value                   = 0x08,
    id_lighting_save                        = 0x09,
    id_eeprom_reset                         = 0x0A,
    id_bootloader_jump                      = 0x0B,
    id_dynamic_keymap_macro_get_count       = 0x0C,
    id_dynamic_keymap_macro_get_buffer_size = 0x0D,
    id_dynamic_keymap_macro_get_buffer      = 0x0E,
    id_dynamic_keymap_macro_set_buffer      = 0x0F,
    id_dynamic_keymap_macro_reset           = 0x10,
    id_dynamic_keymap_get_layer_count       = 0x11,
    id_dynamic_keymap_get_buffer            = 0x12,
    id_dynamic_keymap_set_buffer            = 0x13,
    id_dynamic_keymap_get_encoder           = 0x14,
    id_dynamic_keymap_set_encoder           = 0x15,
    id_signalrgb_qmk_version                = 0x21,
    id_signalrgb_protocol_version           = 0x22,
    id_signalrgb_unique_identifier          = 0x23,
    id_signalrgb_stream_leds                = 0x24,
    id_signalrgb_effect_enable              = 0x25,
    id_signalrgb_effect_disable             = 0x26,
    id_signalrgb_total_leds                 = 0x27,
    id_signalrgb_firmware_type              = 0x28,
    id_unhandled                            = 0xFF,
};

enum signalrgb_responses {
    PROTOCOL_VERSION_BYTE_1 = 1,
    PROTOCOL_VERSION_BYTE_2 = 0,
    PROTOCOL_VERSION_BYTE_3 = 5,
    DEVICE_UNIQUE_IDENTIFIER_BYTE_1 = 0,
    DEVICE_UNIQUE_IDENTIFIER_BYTE_2 = 0,
    DEVICE_UNIQUE_IDENTIFIER_BYTE_3 = 0,
    FIRMWARE_TYPE_BYTE = 2, 
    DEVICE_ERROR_LED_BOUNDS = 253,
    DEVICE_ERROR_LED_COUNT = 254
};

//Changelogs for Firmware Versions------------------------------------
//V1.0.1 added detection for the total number of LEDs a board has. Plugins need a rewrite to make use of this change. Rewritten plugins will not function with older firmware.
//V1.0.2 added another detection byte for which fork of SignalRGB firmware device is running. This means we can keep overlap Unique Identifiers between Sonix and Mainline.
//V1.0.3 unifies the command IDs between Mainline, Sonix, and VIA. All commands have a 0x20 offset now.
//V1.0.4 improves detection handling, and has a complete rewrite of the plugins. Also merges Mainline and VIA branches. VIA Branch is deprecated.
//V1.0.5 adds support for RGBLight and adds proper bounds checking for leds to ensure we don't crash the firmware.

/*
DEVICE, DEVICE_UNIQUE_IDENTIFIER_BYTE_1,DEVICE_UNIQUE_IDENTIFIER_BYTE_2,DEVICE_UNIQUE_IDENTIFIER_BYTE_3
gmmk/pro/rev1/ansi:via                  0 0 5
gmmk/pro/rev2/ansi:via                  0 0 5
gmmk/pro/rev1/iso:via                   0 0 6
gmmk/pro/rev2/iso:via                   0 0 6
gmmk/gmmk2/p65/ansi:via                 0 0 7
gmmk/gmmk2/p65/iso:via                  0 0 8
gmmk/gmmk2/p96/ansi:via                 0 0 9
gmmk/gmmk2/p96/iso:via                  0 0 10
redragon/k556:via                       0 1 3
redragon/k580:via                       0 1 4
*/

enum via_keyboard_value_id {
    id_uptime              = 0x01, //
    id_layout_options      = 0x02,
    id_switch_matrix_state = 0x03
};

enum via_lighting_value {
    // QMK BACKLIGHT
    id_qmk_backlight_brightness = 0x09,
    id_qmk_backlight_effect     = 0x0A,

    // QMK RGBLIGHT
    id_qmk_rgblight_brightness   = 0x80,
    id_qmk_rgblight_effect       = 0x81,
    id_qmk_rgblight_effect_speed = 0x82,
    id_qmk_rgblight_color        = 0x83,
};

// Can't use SAFE_RANGE here, it might change if someone adds
// new values to enum quantum_keycodes.
// Need to keep checking 0x5F10 is still in the safe range.
// TODO: merge this into quantum_keycodes
// Backlight keycodes are in range 0x5F00-0x5F0F
enum via_keycodes {
    FN_MO13 = 0x5F10,
    FN_MO23,
    MACRO00,
    MACRO01,
    MACRO02,
    MACRO03,
    MACRO04,
    MACRO05,
    MACRO06,
    MACRO07,
    MACRO08,
    MACRO09,
    MACRO10,
    MACRO11,
    MACRO12,
    MACRO13,
    MACRO14,
    MACRO15,
};

enum user_keycodes {
    USER00 = 0x5F80,
    USER01,
    USER02,
    USER03,
    USER04,
    USER05,
    USER06,
    USER07,
    USER08,
    USER09,
    USER10,
    USER11,
    USER12,
    USER13,
    USER14,
    USER15,
};

// Can be called in an overriding via_init_kb() to test if keyboard level code usage of
// EEPROM is invalid and use/save vias.
bool via_eeprom_is_valid(void);

// Sets VIA/keyboard level usage of EEPROM to valid/invalid
// Keyboard level code (eg. via_init_kb()) should not call this
void via_eeprom_set_valid(bool valid);

// Called by QMK core to initialize dynamic keymaps etc.
void eeconfig_init_via(void);
void via_init(void);

// Used by VIA to store and retrieve the layout options.
uint32_t via_get_layout_options(void);
void     via_set_layout_options(uint32_t value);
void     via_set_layout_options_kb(uint32_t value);

//Used to handle SignalRGB Compatibility
void get_qmk_version(void);
void get_signalrgb_protocol_version(void);
void get_unique_identifier(void);
void led_streaming(uint8_t *data);
void signalrgb_mode_enable(void);
void signalrgb_mode_disable(void);
void signalrgb_total_leds(void);
void signalrgb_firmware_type(void);

// Called by QMK core to process VIA-specific keycodes.
bool process_record_via(uint16_t keycode, keyrecord_t *record);
