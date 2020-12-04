/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    errors.h
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef JSONRPC_CPP_COMMON_TYPES_H_
#define JSONRPC_CPP_COMMON_TYPES_H_

#include <string>
#include <json/json.h>
using std::string;
using std::stringstream;
using StringVector = std::vector<string>;

typedef enum { PARAMS_BY_NAME, PARAMS_BY_POSITION } parameterDeclaration_t;

enum jsontype_t {
    JSON_STRING = 1,
    JSON_BOOLEAN = 2,
    JSON_INTEGER = 3,
    JSON_REAL = 4,
    JSON_OBJECT = 5,
    JSON_ARRAY = 6,
    JSON_NUMERIC = 7
};
typedef std::map<string, jsontype_t> parameterNameList_t;
typedef std::vector<jsontype_t> parameterPositionList_t;

#endif /* JSONRPC_CPP_COMMON_TYPES_H_ */
