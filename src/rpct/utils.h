#pragma once

#include "types.h"

inline void replaceAll(string& result, const string& find, const string& replace) {
    size_t pos = result.find(find);
    while (pos != string::npos) {
        result.replace(pos, find.length(), replace);
        pos = result.find(find, pos + replace.length());
    }
}

inline string substitute(const string& text, const string& fnd, const string& rep) {
    string result = text;
    replaceAll(result, fnd, rep);
    return result;
}
