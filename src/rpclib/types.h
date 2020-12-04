/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    errors.h
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/
#pragma once

#include <string>
#include <json/json.h>
using std::string;
using std::stringstream;
using StringVector = std::vector<string>;

typedef enum { PARAMS_BY_NAME, PARAMS_BY_POSITION } parameterDeclaration_t;

enum jsontype_t {
    JSON_STRING = Json::stringValue,
    JSON_BOOLEAN = Json::booleanValue,
    JSON_INTEGER = Json::uintValue,
    JSON_REAL = Json::realValue,
    JSON_OBJECT = Json::objectValue,
    JSON_ARRAY = Json::arrayValue,
    JSON_NUMERIC = Json::intValue
};
typedef std::map<string, jsontype_t> parameterNameList_t;
typedef std::vector<jsontype_t> parameterPositionList_t;
