/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#pragma once

#include "types.h"

//---------------------------------------------------------------------------------------
inline void replaceAll(string& result, const string& find, const string& replace) {
    size_t pos = result.find(find);
    while (pos != string::npos) {
        result.replace(pos, find.length(), replace);
        pos = result.find(find, pos + replace.length());
    }
}

//---------------------------------------------------------------------------------------
inline string substitute(const string& text, const string& fnd, const string& rep) {
    string result = text;
    replaceAll(result, fnd, rep);
    return result;
}
