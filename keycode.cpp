#include "keycode.h"
#include "stdbool.h"


static const keycode scancodeSet1Table[256] = {
    [0x00] = None,   [0x01] = Escape, [0x02] = Alpha1,    [0x03] = Alpha2,
    [0x04] = Alpha3, [0x05] = Alpha4, [0x06] = Alpha5,    [0x07] = Alpha6,
    [0x08] = Alpha7, [0x09] = Alpha8, [0x0A] = Alpha9,    [0x0B] = Alpha0,
    [0x0C] = Minus,  [0x0D] = Equals, [0x0E] = Backspace, [0x0F] = Tab,

    [0x10] = KeyQ, [0x11] = KeyW, [0x12] = KeyE, [0x13] = KeyR,
    [0x14] = KeyT, [0x15] = KeyY, [0x16] = KeyU, [0x17] = KeyI,
    [0x18] = KeyO, [0x19] = KeyP, [0x1a] = LeftBracket, [0x1b] = RightBracket,
    [0x1c] = Enter, [0x1d] = LeftControl, [0x1e] = KeyA, [0x1f] = KeyS,
    
    [0x20] = KeyD, [0x21] = KeyF, [0x22] = KeyG, [0x23] = KeyH,
    [0x24] = KeyJ, [0x25] = KeyK, [0x26] = KeyL, [0x27] = Semicolon,
    [0x28] = Quote, [0x29] = BackQuote, [0x2a] = LeftShift, [0x2b] = Backslash,
    [0x2c] = KeyZ, [0x2d] = KeyX, [0x2e] = KeyC, [0x2f] = KeyV,
    
    [0x30] = KeyB, [0x31] = KeyN, [0x32] = KeyM, [0x33] = Comma,
    [0x34] = Period, [0x35] = Slash, [0x36] = RightShift, [0x37] = KeypadMultiply,
    [0x38] = LeftAlt, [0x39] = Space, [0x3a] = CapsLock, [0x3b] = F1,
    [0x3c] = F2, [0x3d] = F3, [0x3e] = F4, [0x3f] = F5,
    
    [0x40] = F6, [0x41] = F7, [0x42] = F8, [0x43] = F9,
    [0x44] = F10, [0x45] = Numlock, [0x46] = ScrollLock, [0x47] = Keypad7,
    [0x48] = Keypad8, [0x49] = Keypad9, [0x4a] = KeypadMinus, [0x4b] = Keypad4,
    [0x4c] = Keypad5, [0x4d] = Keypad6, [0x4e] = KeypadPlus, [0x4f] = Keypad1,
    
    [0x50] = Keypad2, [0x51] = Keypad3, [0x52] = Keypad0, [0x53] = KeypadPeriod,
    [0x54] = None, [0x55] = None, [0x56] = None, [0x57] = F11,
    [0x58] = F12, [0x59] = None, [0x5a] = None, [0x5b] = None,
    [0x5c] = None, [0x5d] = None, [0x5e] = None, [0x5f] = None,
    
    [0x60] = None, [0x61] = None, [0x62] = None, [0x63] = None,
    [0x64] = None, [0x65] = None, [0x66] = None, [0x67] = None,
    [0x68] = None, [0x69] = None, [0x6a] = None, [0x6b] = None,
    [0x6c] = None, [0x6d] = None, [0x6e] = None, [0x6f] = None,
    
    [0x70] = None, [0x71] = None, [0x72] = None, [0x73] = None,
    [0x74] = None, [0x75] = None, [0x76] = None, [0x77] = None,
    [0x78] = None, [0x79] = None, [0x7a] = None, [0x7b] = None,
    [0x7c] = None, [0x7d] = None, [0x7e] = None, [0x7f] = None,
    
    [0x80] = None,   [0x81] = Escape, [0x82] = Alpha1,    [0x83] = Alpha2,
    [0x84] = Alpha3, [0x85] = Alpha4, [0x86] = Alpha5,    [0x87] = Alpha6,
    [0x88] = Alpha7, [0x89] = Alpha8, [0x8A] = Alpha9,    [0x8B] = Alpha0,
    [0x8C] = Minus,  [0x8D] = Equals, [0x8E] = Backspace, [0x8F] = Tab,

    [0x90] = KeyQ, [0x91] = KeyW, [0x92] = KeyE, [0x93] = KeyR,
    [0x94] = KeyT, [0x95] = KeyY, [0x96] = KeyU, [0x97] = KeyI,
    [0x98] = KeyO, [0x99] = KeyP, [0x9a] = LeftBracket, [0x9b] = RightBracket,
    [0x9c] = Enter, [0x9d] = LeftControl, [0x9e] = KeyA, [0x9f] = KeyS,
    
    [0xa0] = KeyD, [0xa1] = KeyF, [0xa2] = KeyG, [0xa3] = KeyH,
    [0xa4] = KeyJ, [0xa5] = KeyK, [0xa6] = KeyL, [0xa7] = Semicolon,
    [0xa8] = Quote, [0xa9] = BackQuote, [0xaa] = LeftShift, [0xab] = Backslash,
    [0xac] = KeyZ, [0xad] = KeyX, [0xae] = KeyC, [0xaf] = KeyV,
    
    [0xb0] = KeyB, [0xb1] = KeyN, [0xb2] = KeyM, [0xb3] = Comma,
    [0xb4] = Period, [0xb5] = Slash, [0xb6] = RightShift, [0xb7] = KeypadMultiply,
    [0xb8] = LeftAlt, [0xb9] = Space, [0xba] = CapsLock, [0xbb] = F1,
    [0xbc] = F2, [0xbd] = F3, [0xbe] = F4, [0xbf] = F5,
    
    [0xc0] = F6, [0xc1] = F7, [0xc2] = F8, [0xc3] = F9,
    [0xc4] = F10, [0xc5] = Numlock, [0xc6] = ScrollLock, [0xc7] = Keypad7,
    [0xc8] = Keypad8, [0xc9] = Keypad9, [0xca] = KeypadMinus, [0xcb] = Keypad4,
    [0xcc] = Keypad5, [0xcd] = Keypad6, [0xce] = KeypadPlus, [0xcf] = Keypad1,
    
    [0xd0] = Keypad2, [0xd1] = Keypad3, [0xd2] = Keypad0, [0xd3] = KeypadPeriod,
    [0xd4] = None, [0xd5] = None, [0xd6] = None, [0xd7] = F11,
    [0xd8] = F12, [0xd9] = None, [0xda] = None, [0xdb] = None,
    [0xdc] = None, [0xdd] = None, [0xde] = None, [0xdf] = None,
    
    [0xe0] = None, [0xe1] = None, [0xe2] = None, [0xe3] = None,
    [0xe4] = None, [0xe5] = None, [0xe6] = None, [0xe7] = None,
    [0xe8] = None, [0xe9] = None, [0xea] = None, [0xeb] = None,
    [0xec] = None, [0xed] = None, [0xee] = None, [0xef] = None,
    
    [0xf0] = None, [0xf1] = None, [0xf2] = None, [0xf3] = None,
    [0xf4] = None, [0xf5] = None, [0xf6] = None, [0xf7] = None,
    [0xf8] = None, [0xf9] = None, [0xfa] = None, [0xfb] = None,
    [0xfc] = None, [0xfd] = None, [0xfe] = None, [0xff] = None,
};

static const keycode scancodeSet1TableE0[256] = {
    [0x00] = None, [0x01] = None, [0x02] = None, [0x03] = None,
    [0x04] = None, [0x05] = None, [0x06] = None, [0x07] = None,
    [0x08] = None, [0x09] = None, [0x0A] = None, [0x0B] = None,
    [0x0C] = None, [0x0D] = None, [0x0E] = None, [0x0F] = None,

    [0x10] = None, [0x11] = None, [0x12] = None, [0x13] = None,
    [0x14] = None, [0x15] = None, [0x16] = None, [0x17] = None,
    [0x18] = None, [0x19] = None, [0x1A] = None, [0x1B] = None,
    [0x1C] = KeypadEnter, [0x1D] = RightCtrl, [0x1E] = None, [0x1F] = None,

    [0x20] = None, [0x21] = None, [0x22] = None, [0x23] = None,
    [0x24] = None, [0x25] = None, [0x26] = None, [0x27] = None,
    [0x28] = None, [0x29] = None, [0x2a] = None, [0x2b] = None,
    [0x2c] = None, [0x2d] = None, [0x2e] = None, [0x2f] = None,
    
    [0x30] = None, [0x31] = None, [0x32] = None, [0x33] = None,
    [0x34] = None, [0x35] = KeypadDivide, [0x36] = None, [0x37] = None,
    [0x38] = RightAlt, [0x39] = None, [0x3a] = None, [0x3b] = None,
    [0x3c] = None, [0x3d] = None, [0x3e] = None, [0x3f] = None,
    
    [0x40] = None, [0x41] = None, [0x42] = None, [0x43] = None,
    [0x44] = None, [0x45] = None, [0x46] = None, [0x47] = Home,
    [0x48] = UpArrow, [0x49] = PageUp, [0x4a] = None, [0x4b] = LeftArrow,
    [0x4c] = None, [0x4d] = RightArrow, [0x4e] = None, [0x4f] = End,
    
    [0x50] = DownArrow, [0x51] = PageDown, [0x52] = Insert, [0x53] = Delete,
    [0x54] = None, [0x55] = None, [0x56] = None, [0x57] = None,
    [0x58] = None, [0x59] = None, [0x5a] = None, [0x5b] = None,
    [0x5c] = None, [0x5d] = None, [0x5e] = None, [0x5f] = None,
    
    [0x60] = None, [0x61] = None, [0x62] = None, [0x63] = None,
    [0x64] = None, [0x65] = None, [0x66] = None, [0x67] = None,
    [0x68] = None, [0x69] = None, [0x6a] = None, [0x6b] = None,
    [0x6c] = None, [0x6d] = None, [0x6e] = None, [0x6f] = None,
    
    [0x70] = None, [0x71] = None, [0x72] = None, [0x73] = None,
    [0x74] = None, [0x75] = None, [0x76] = None, [0x77] = None,
    [0x78] = None, [0x79] = None, [0x7a] = None, [0x7b] = None,
    [0x7c] = None, [0x7d] = None, [0x7e] = None, [0x7f] = None,
    
    [0x80] = None, [0x81] = None, [0x82] = None, [0x83] = None,
    [0x84] = None, [0x85] = None, [0x86] = None, [0x87] = None,
    [0x88] = None, [0x89] = None, [0x8A] = None, [0x8B] = None,
    [0x8C] = None, [0x8D] = None, [0x8E] = None, [0x8F] = None,

    [0x90] = None, [0x91] = None, [0x92] = None, [0x93] = None,
    [0x94] = None, [0x95] = None, [0x96] = None, [0x97] = None,
    [0x98] = None, [0x99] = None, [0x9A] = None, [0x9B] = None,
    [0x9C] = KeypadEnter, [0x9D] = RightCtrl, [0x9E] = None, [0x9F] = None,

    [0xa0] = None, [0xa1] = None, [0xa2] = None, [0xa3] = None,
    [0xa4] = None, [0xa5] = None, [0xa6] = None, [0xa7] = None,
    [0xa8] = None, [0xa9] = None, [0xaa] = None, [0xab] = None,
    [0xac] = None, [0xad] = None, [0xae] = None, [0xaf] = None,
    
    [0xb0] = None, [0xb1] = None, [0xb2] = None, [0xb3] = None,
    [0xb4] = None, [0xb5] = KeypadDivide, [0xb6] = None, [0xb7] = None,
    [0xb8] = RightAlt, [0xb9] = None, [0xba] = None, [0xbb] = None,
    [0xbc] = None, [0xbd] = None, [0xbe] = None, [0xbf] = None,
    
    [0xc0] = None, [0xc1] = None, [0xc2] = None, [0xc3] = None,
    [0xc4] = None, [0xc5] = None, [0xc6] = None, [0xc7] = Home,
    [0xc8] = UpArrow, [0xc9] = PageUp, [0xca] = None, [0xcb] = LeftArrow,
    [0xcc] = None, [0xcd] = RightArrow, [0xce] = None, [0xcf] = End,
    
    [0xd0] = DownArrow, [0xd1] = PageDown, [0xd2] = Insert, [0xd3] = Delete,
    [0xd4] = None, [0xd5] = None, [0xd6] = None, [0xd7] = None,
    [0xd8] = None, [0xd9] = None, [0xda] = None, [0xdb] = None,
    [0xdc] = None, [0xdd] = None, [0xde] = None, [0xdf] = None,
    
    [0xe0] = None, [0xe1] = None, [0xe2] = None, [0xe3] = None,
    [0xe4] = None, [0xe5] = None, [0xe6] = None, [0xe7] = None,
    [0xe8] = None, [0xe9] = None, [0xea] = None, [0xeb] = None,
    [0xec] = None, [0xed] = None, [0xee] = None, [0xef] = None,
    
    [0xf0] = None, [0xf1] = None, [0xf2] = None, [0xf3] = None,
    [0xf4] = None, [0xf5] = None, [0xf6] = None, [0xf7] = None,
    [0xf8] = None, [0xf9] = None, [0xfa] = None, [0xfb] = None,
    [0xfc] = None, [0xfd] = None, [0xfe] = None, [0xff] = None,
};

keycode scancodeToKeycode(uint8_t scancode) {
    return scancodeSet1Table[scancode];
}

keycode scancodeToKeycode2(uint8_t scancode) {
    return scancodeSet1TableE0[scancode];
}

/* If scancode maps to None, return value is undefined */
bool scancodeToPressed(uint8_t scancode) {
    if (scancode < 0x80) {
        return true;
    }
    return false;
}

/* If scancode maps to None, return value is undefined */
bool scancodeToPressed2(uint8_t scancode) {
    if (scancode < 0x80) {
        return true;
    }
    /* TODO: handle print screen and pause */

    return false;
}


char keycodeToChar(keycode kc, bool shift) {
    if (!shift) {
        switch (kc) {
            case BackQuote: return '`';
            case Alpha0: return '0';
            case Alpha1: return '1';
            case Alpha2: return '2';
            case Alpha3: return '3';
            case Alpha4: return '4';
            case Alpha5: return '5';
            case Alpha6: return '6';
            case Alpha7: return '7';
            case Alpha8: return '8';
            case Alpha9: return '9';
            case Minus: return '-';
            case Equals: return '=';
            case Backspace: return '\b';
    
            case Tab: return '\t';
            case KeyA: return 'a';
            case KeyB: return 'b';
            case KeyC: return 'c';
            case KeyD: return 'd';
            case KeyE: return 'e';
            case KeyF: return 'f';
            case KeyG: return 'g';
            case KeyH: return 'h';
            case KeyI: return 'i';
            case KeyJ: return 'j';
            case KeyK: return 'k';
            case KeyL: return 'l';
            case KeyM: return 'm';
            case KeyN: return 'n';
            case KeyO: return 'o';
            case KeyP: return 'p';
            case KeyQ: return 'q';
            case KeyR: return 'r';
            case KeyS: return 's';
            case KeyT: return 't';
            case KeyU: return 'u';
            case KeyV: return 'v';
            case KeyW: return 'w';
            case KeyX: return 'x';
            case KeyY: return 'y';
            case KeyZ: return 'z';
            case LeftBracket: return '[';
            case RightBracket: return ']';
            case Backslash: return '\\';
    
            case Semicolon: return ';';
            case Quote: return '\'';
            case Enter: return '\n';
    
            case Comma: return ',';
            case Period: return '.';
            case Slash: return '/';
    
            case Space: return ' ';
    
            case KeypadDivide: return '/';
            case KeypadMultiply: return '*';
            case KeypadMinus: return '-';
            case KeypadPlus: return '+';
            case KeypadPeriod: return '.';
            case KeypadEnter: return '\n';
    
            case Keypad0: return '0';
            case Keypad1: return '1';
            case Keypad2: return '2';
            case Keypad3: return '3';
            case Keypad4: return '4';
            case Keypad5: return '5';
            case Keypad6: return '6';
            case Keypad7: return '7';
            case Keypad8: return '8';
            case Keypad9: return '9';
            default: return '\0';
        }
    }
    else {
        switch (kc) {
            case BackQuote: return '~';
            case Alpha0: return ')';
            case Alpha1: return '!';
            case Alpha2: return '@';
            case Alpha3: return '#';
            case Alpha4: return '$';
            case Alpha5: return '%';
            case Alpha6: return '^';
            case Alpha7: return '&';
            case Alpha8: return '*';
            case Alpha9: return '(';
            case Minus: return '_';
            case Equals: return '+';
            case Backspace: return '\b';
    
            case Tab: return '\t';
            case KeyA: return 'A';
            case KeyB: return 'B';
            case KeyC: return 'C';
            case KeyD: return 'D';
            case KeyE: return 'E';
            case KeyF: return 'F';
            case KeyG: return 'G';
            case KeyH: return 'H';
            case KeyI: return 'I';
            case KeyJ: return 'J';
            case KeyK: return 'K';
            case KeyL: return 'L';
            case KeyM: return 'M';
            case KeyN: return 'N';
            case KeyO: return 'O';
            case KeyP: return 'P';
            case KeyQ: return 'Q';
            case KeyR: return 'R';
            case KeyS: return 'S';
            case KeyT: return 'T';
            case KeyU: return 'U';
            case KeyV: return 'V';
            case KeyW: return 'W';
            case KeyX: return 'X';
            case KeyY: return 'Y';
            case KeyZ: return 'Z';
            case LeftBracket: return '{';
            case RightBracket: return '}';
            case Backslash: return '|';
    
            case Semicolon: return ':';
            case Quote: return '"';
    
            case Comma: return '<';
            case Period: return '>';
            case Slash: return '?';
    
            case Space: return ' ';
    
            case KeypadDivide: return '/';
            case KeypadMultiply: return '*';
            case KeypadMinus: return '-';
            case KeypadPlus: return '+';
            case KeypadPeriod: return '.';
    
            case Keypad0: return '0';
            case Keypad1: return '1';
            case Keypad2: return '2';
            case Keypad3: return '3';
            case Keypad4: return '4';
            case Keypad5: return '5';
            case Keypad6: return '6';
            case Keypad7: return '7';
            case Keypad8: return '8';
            case Keypad9: return '9';
            default: return '\0';
        }
    }
}
