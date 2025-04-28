#pragma once
#include <cstdint>
#include "x86.h"
#include "keycode.h"

namespace keyboard {
typedef void (*inputEventHandler)(keycode kc, bool pressed);
int subscribeToInputEvent(inputEventHandler handler);
void unsubscribeToInputEvent(int handlerId);
void handleScanCode(uint8_t scancode);
bool isKeyPressed(keycode kc);
bool isCapsLockOn();
}
