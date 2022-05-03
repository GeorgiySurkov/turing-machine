#include "helpers.h"

#include <iostream>

bool is_delim(char c, const char *delim) {
    while (*delim != '\0') {
        if (c == *delim)
            return true;
        delim++;
    }
    return false;
}

char *my_strtok(char *s, const char *delim) {
    static char *p; // start of the next search
    if (!s) {
        s = p;
    }
    if (!s) {
        // user is bad user
        return nullptr;
    }

    // handle beginning of the string containing delims
    while (true) {
        if (is_delim(*s, delim)) {
            s++;
            continue;
        }
        if (*s == '\0') {
            return nullptr; // we've reached the end of the string
        }
        // now, we've hit a regular character. Let's exit the
        // loop, and we'd need to give the caller a string
        // that starts here.
        break;
    }

    char *ret = s;
    while (true) {
        if (*s == '\0') {
            p = s; // next exec will return NULL
            return ret;
        }
        if (is_delim(*s, delim)) {
            *s = '\0';
            p = s + 1;
            return ret;
        }
        s++;
    }
}

int my_strcmp(const char *s1, const char *s2) {
    while (*s1) {
        if (*s1 != *s2) {
            break;
        }

        s1++;
        s2++;
    }

    return *(const unsigned char *) s1 - *(const unsigned char *) s2;
}
