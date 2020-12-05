/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#pragma once

#include <string>
#include <json/json.h>
using std::string;
using std::stringstream;
using StringVector = std::vector<string>;

//---------------------------------------------------------------------------------------
typedef enum { PARAMS_BY_NAME, PARAMS_BY_POSITION } param_t;

//---------------------------------------------------------------------------------------
enum jsontype_t {
    JSON_STRING = Json::stringValue,
    JSON_BOOLEAN = Json::booleanValue,
    JSON_INTEGER = Json::uintValue,
    JSON_REAL = Json::realValue,
    JSON_OBJECT = Json::objectValue,
    JSON_ARRAY = Json::arrayValue,
    JSON_NUMERIC = Json::intValue
};

//---------------------------------------------------------------------------------------
typedef std::map<string, jsontype_t> parameterNameList_t;
typedef std::vector<jsontype_t> parameterPositionList_t;
