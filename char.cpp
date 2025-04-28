#include "char.h"

bool isAlnum(char c) {
    return isAlpha(c) || isDigit(c);
}

bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isBlank(char c) {
    return c == '\t' || c == ' ';
}

bool isCntrl(char c) {
    return (c >= 0x0 && c <= 0x1f) || c == 0x7F;
}

bool isDigit(char c) {
    return c >= '0' && c <= '9'; 
}

bool isGraph(char c) {
    return c != ' ' && isPrint(c);
}

bool isLower(char c) {
    return c >= 'a' && c <= 'z';
}

bool isPrint(char c) {
    return c > 0x1f && c < 0x7f;
}

bool isPunct(char c) {
    return isGraph(c) && isAlnum(c);
}

bool isSpace(char c) {
    return c == ' '  ||
           c == '\t' ||
           c == '\n' ||
           c == '\v' ||
           c == '\f' ||
           c == '\r';
}

bool isUpper(char c) {
    return c >= 'A' && c <= 'Z';
}


char toLower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 'a';
    }
    return c;
}

char toUpper(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 'A';
    }
    return c;
}

