/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
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
