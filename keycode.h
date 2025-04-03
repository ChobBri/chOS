#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    None,

    Escape,                 F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,                                   Insert, Print, Delete,
    BackQuote,            Alpha0, Alpha1, Alpha2, Alpha3, Alpha4, Alpha5, Alpha6, Alpha7, Alpha8, Alpha9,          Minus, Equals, Backspace,
    Tab,                  KeyQ, KeyW, KeyE, KeyR, KeyT, KeyY, KeyU, KeyI, KeyO, KeyP,                        LeftBracket, RightBracket, Backslash,
    CapsLock,               KeyA, KeyS, KeyD, KeyF, KeyG, KeyH, KeyJ, KeyK, KeyL,                                Semicolon, Quote, Enter,
    LeftShift,              KeyZ, KeyX, KeyC, KeyV, KeyB, KeyN, KeyM,                                    Comma, Period, Slash, RightShift,
    LeftControl,    LeftAlt,                    Space,                                  RightAlt, RightCtrl,

                UpArrow,
    LeftArrow, DownArrow, RightArrow,

    ScrollLock,

    Home, End, PageUp, PageDown,
    Numlock, KeypadDivide, KeypadMultiply, KeypadMinus,
    Keypad7, Keypad8, Keypad9, KeypadPlus,
    Keypad4, Keypad5, Keypad6,
    Keypad1, Keypad2, Keypad3,
    Keypad0, KeypadPeriod,     KeypadEnter,

    KeycodeLen
} keycode;

keycode scancodeToKeycode(uint8_t scancode);
bool scancodeToPressed(uint8_t scancode);
keycode scancodeToKeycode2(uint8_t scancode);
bool scancodeToPressed2(uint8_t scancode);
char keycodeToChar(keycode kc, bool shift);
