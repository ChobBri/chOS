
#include "keyboard.h"

namespace keyboard {
    
struct keyboard_state {
    bool isKeyPressed[keycode::KeycodeLen] = {};
    bool capslock = false;
    bool e0 = false;
};
keyboard_state input_state;

struct inputEventHandler_state {
    static constexpr int MAX_HANDLERS = 8;
    inputEventHandler handlers[MAX_HANDLERS] = {};
};
inputEventHandler_state event_state;

int subscribeToInputEvent(inputEventHandler handler) {
    if (handler == nullptr) {
        return -1;
    }

    for (int i = 0; i < inputEventHandler_state::MAX_HANDLERS; i++) {
        if (event_state.handlers[i] == nullptr) {
            event_state.handlers[i] = handler;
            return i;
        }
    }
    return -1;
}

void unsubscribeToInputEvent(int handlerId) {
    event_state.handlers[handlerId] = nullptr;
    return;
}

void handleScanCode(uint8_t scancode) {
    if (scancode == 0xe0) {
        input_state.e0 = true;
        return;
    }
    
    keycode kc;
    bool pressed;

    if (!input_state.e0) {
        kc = scancodeToKeycode(scancode);
        pressed = scancodeToPressed(scancode);
    }
    else {
        kc = scancodeToKeycode2(scancode);
        pressed = scancodeToPressed2(scancode);
    }
    input_state.e0 = false;
    
    if (pressed && kc == CapsLock) {
        input_state.capslock = !input_state.capslock;
    }

    input_state.isKeyPressed[kc] = pressed;

    if (kc != keycode::None) {
        for (int i = 0; i < inputEventHandler_state::MAX_HANDLERS; i++) {
            if (event_state.handlers[i] != nullptr) {
                event_state.handlers[i](kc, pressed);
            }
        }
    }
}

bool isKeyPressed(keycode kc) {
    return input_state.isKeyPressed[kc];
}

bool isCapsLockOn() {
    return input_state.capslock;
}

}