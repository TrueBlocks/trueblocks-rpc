#ifndef JSONRPC_CPP_COMMON_UTILS_H_
#define JSONRPC_CPP_COMMON_UTILS_H_

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

#endif /* JSONRPC_CPP_COMMON_UTILS_H_ */
