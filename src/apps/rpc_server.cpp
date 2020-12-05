/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#include <stdio.h>
#include <iostream>

#include <rpclib/http_server.h>
#include <gen/rpc_server.h>

using namespace jsonrpc;
using namespace std;

//---------------------------------------------------------------------------------------
extern Json::Value getTestBlock(const string& id, bool fullTx);
extern Json::Value getTestHeader(const string& id);
extern Json::Value getTestTrans(const string& id, bool fullTx);
extern Json::Value getTestUncle(const string& id, const string& index);
extern Json::Value getTestReceipt(const string& id);
extern Json::Value getTestLogByHash(void);
extern Json::Value getTestTrace(uint32_t which = 0);

//---------------------------------------------------------------------------------------
class EthRpcServer : public AbstractServer {
  public:
    EthRpcServer(HttpServer& connector);

    string web3_clientVersion();
    string web3_sha3(const string& param01);

    bool net_listening();
    string net_version();
    string net_peerCount();

    Json::Value eth_getBlockByNumber(const string& param01, bool param02);
    Json::Value eth_getBlockByHash(const string& param01, bool param02);
    string eth_getBlockTransactionCountByNumber(const string& param01);
    string eth_getBlockTransactionCountByHash(const string& param01);

    Json::Value eth_getTransactionByHash(const string& param01);
    Json::Value eth_getTransactionByBlockHashAndIndex(const string& param01, const string& param02);
    Json::Value eth_getTransactionByBlockNumberAndIndex(const string& param01, const string& param02);
    Json::Value eth_getTransactionReceipt(const string& param01);

    Json::Value eth_getUncleByBlockNumberAndIndex(const string& param01, const string& param02);
    Json::Value eth_getUncleByBlockHashAndIndex(const string& param01, const string& param02);
    string eth_getUncleCountByBlockNumber(const string& param01);
    string eth_getUncleCountByBlockHash(const string& param01);

    string eth_newPendingTransactionFilter();
    string eth_newBlockFilter();
    string eth_newFilter(const Json::Value& param01);
    bool eth_uninstallFilter(const string& param01);
    Json::Value eth_getFilterChanges(const string& param01);
    Json::Value eth_getLogs(const Json::Value& param01);

    string eth_getBalance(const string& param01, const string& param02);
    string eth_getTransactionCount(const string& param01, const string& param02);
    string eth_getCode(const string& param01, const string& param02);
    string eth_getStorageAt(const string& param01, const string& param02, const string& param03);

    string eth_blockNumber();
    Json::Value eth_syncing();
    string eth_chainId();
    string eth_protocolVersion();
    string eth_gasPrice();

    string eth_call(const Json::Value& param01, const string& param02);
    string eth_estimateGas(const Json::Value& param01);
    string eth_sendTransaction(const Json::Value& param01);
    string eth_sendRawTransaction(const string& param01);
    string eth_getProof(const string& param01, const Json::Value& param02, const string& param03);

    string eth_coinbase();
    string eth_hashrate();
    bool eth_mining();
    Json::Value eth_getWork();
    bool eth_submitWork(const string& param01, const string& param02, const string& param03);
    bool eth_submitHashrate(const string& param01, const string& param02);

    Json::Value trace_call(const Json::Value& param01, const Json::Value& param02, const string& param03);
    Json::Value trace_callMany(const Json::Value& param01, const string& param02);
    Json::Value trace_rawTransaction(const string& param01, const Json::Value& param02);
    Json::Value trace_replayBlockTransactions(const string& param01, const Json::Value& param02);
    Json::Value trace_replayTransaction(const string& param01, const Json::Value& param02);
    Json::Value trace_transaction(const string& param01);
    Json::Value trace_get(const string& param01, const Json::Value& param02);
    Json::Value trace_block(const string& param01);
    Json::Value trace_filter(const Json::Value& param01);

    Json::Value tg_forks();
    Json::Value tg_getHeaderByNumber(const string& param01);
    Json::Value tg_getHeaderByHash(const string& param01);
    Json::Value tg_getLogsByHash(const string& param01);
    Json::Value tg_issuance(const string& param01);

    Json::Value debug_storageRangeAt(const string& param01, int param02, const string& param03, const string& param04,
                                     int param05);
    Json::Value debug_accountRange(const string& param01, const Json::Value& param02, int param03, bool param04,
                                   bool param05, bool param06);
    string debug_getModifiedAccountsByNumber(const string& param01, const string& param02);
    string debug_getModifiedAccountsByHash(const string& param01, const string& param02);
    string debug_traceTransaction(const string& param01);
    string eth_accounts();

    Json::Value eth_getCompilers();
    string eth_compileLLL(const string& param01);
    string eth_compileSolidity(const string& param01);
    string eth_compileSerpent(const string& param01);
    string eth_sign(const string& param01, const string& param02);

    string db_getString(const string& param01, const string& param02);
    bool db_putString(const string& param01, const string& param02, const string& param03);
    string db_getHex(const string& param01, const string& param02);
    bool db_putHex(const string& param01, const string& param02, const string& param03);

    bool shh_post(const Json::Value& param01);
    string shh_version();
    string shh_newIdentity();
    bool shh_hasIdentity(const string& param01);
    string shh_newGroup();
    bool shh_addToGroup(const string& param01);
    string shh_newFilter(const Json::Value& param01);
    bool shh_uninstallFilter(const string& param01);
    Json::Value shh_getFilterChanges(const string& param01);
    Json::Value shh_getMessages(const string& param01);
};

//---------------------------------------------------------------------------------------
EthRpcServer::EthRpcServer(HttpServer& connector) : AbstractServer(connector) {
}

//---------------------------------------------------------------------------------------
string EthRpcServer::web3_clientVersion() {
    std::cerr << "Serviced web3_clientVersion()" << std::endl;
    return "TurboGeth/v2020.12.1-alpha/darwin/go1.15.5";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::web3_sha3(const string& param01) {
    std::cerr << "Serviced web3_sha3(" << param01 << ")" << std::endl;
    return "0x47173285a8d7341e5e972fc677286384f802f8ef42a5ec5f03bbfa254cb01fad";
}

//---------------------------------------------------------------------------------------
bool EthRpcServer::net_listening() {
    std::cerr << "Serviced net_listening()" << std::endl;
    return true;
}

//---------------------------------------------------------------------------------------
string EthRpcServer::net_version() {
    std::cerr << "Serviced net_version()" << std::endl;
    return "1";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::net_peerCount() {
    std::cerr << "Serviced net_peerCount()" << std::endl;
    return "0x19";
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::eth_getBlockByNumber(const string& blockNumber, bool fullTx) {
    std::cerr << "Serviced eth_getBlockByNumber(" << blockNumber << ", " << fullTx << ")" << std::endl;
    return getTestBlock(blockNumber, fullTx);
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::eth_getBlockByHash(const string& blockHash, bool fullTx) {
    std::cerr << "Serviced eth_getBlockByHash(" << blockHash << ", " << fullTx << ")" << std::endl;
    return getTestBlock(blockHash, fullTx);
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_getBlockTransactionCountByNumber(const string& param01) {
    std::cerr << "Serviced eth_getBlockTransactionCountByNumber(" << param01 << ")" << std::endl;
    return "0x4";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_getBlockTransactionCountByHash(const string& param01) {
    std::cerr << "Serviced eth_getBlockTransactionCountByHash(" << param01 << ")" << std::endl;
    return "0x4";
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::eth_getTransactionByHash(const string& param01) {
    std::cerr << "Serviced eth_getTransactionByHash(" << param01 << ")" << std::endl;
    return getTestTrans(param01, true);
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::eth_getTransactionByBlockHashAndIndex(const string& param01, const string& param02) {
    std::cerr << "Serviced eth_getTransactionByBlockHashAndIndex(" << param01 << "," << param02 << ")" << std::endl;
    return getTestTrans(param01, true);
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::eth_getTransactionByBlockNumberAndIndex(const string& param01, const string& param02) {
    std::cerr << "Serviced eth_getTransactionByBlockNumberAndIndex(" << param01 << "," << param02 << ")" << std::endl;
    return getTestTrans(param01, true);
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::eth_getTransactionReceipt(const string& param01) {
    std::cerr << "Serviced eth_getTransactionReceipt(" << param01 << ")" << std::endl;
    return getTestReceipt(param01);
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::eth_getUncleByBlockNumberAndIndex(const string& param01, const string& param02) {
    std::cerr << "Serviced eth_getUncleByBlockNumberAndIndex(" << param01 << "," << param02 << ")" << std::endl;
    return getTestUncle(param01, param02);
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::eth_getUncleByBlockHashAndIndex(const string& param01, const string& param02) {
    std::cerr << "Serviced eth_getUncleByBlockHashAndIndex(" << param01 << "," << param02 << ")" << std::endl;
    return getTestUncle(param01, param02);
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_getUncleCountByBlockNumber(const string& param01) {
    std::cerr << "Serviced eth_getUncleCountByBlockNumber(" << param01 << ")" << std::endl;
    return "0x1";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_getUncleCountByBlockHash(const string& param01) {
    std::cerr << "Serviced eth_getUncleCountByBlockHash(" << param01 << ")" << std::endl;
    return "0x1";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_newPendingTransactionFilter() {
    std::cerr << "Serviced eth_newPendingTransactionFilter()" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "eth_newPendingTransactionFilter");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_newBlockFilter() {
    std::cerr << "Serviced eth_newBlockFilter()" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "eth_newBlockFilter");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_newFilter(const Json::Value& param01) {
    std::cerr << "Serviced eth_newFilter(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "eth_newFilter");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
bool EthRpcServer::eth_uninstallFilter(const string& param01) {
    std::cerr << "Serviced eth_uninstallFilter(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "eth_uninstallFilter");
    throw excep;
    return true;
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::eth_getFilterChanges(const string& param01) {
    std::cerr << "Serviced eth_getFilterChanges(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "eth_getFilterChanges");
    throw excep;
    Json::Value v1;
    return v1;
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::eth_getLogs(const Json::Value& param01) {
    std::cerr << "Serviced eth_getLogs(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "eth_getLog");
    throw excep;
    Json::Value v1;
    return v1;
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_getBalance(const string& param01, const string& param02) {
    std::cerr << "Serviced eth_getBalance(" << param01 << "," << param02 << ")" << std::endl;
    return "0x7a69";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_getTransactionCount(const string& param01, const string& param02) {
    std::cerr << "Serviced eth_getTransactionCount(" << param01 << "," << param02 << ")" << std::endl;
    return "0xa";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_getCode(const string& param01, const string& param02) {
    std::cerr << "Serviced eth_getCode(" << param01 << "," << param02 << ")" << std::endl;
    return "0x6060604052361561001f5760e060020a600035046372ea4b8c811461010c575b61011b3460008080670de0b6b3a7640000841"
           "0610"
           "1d557600180548101908190556003805433929081101561000257906000526020600020900160006101000a815481600160a060"
           "020a"
           "0302191690830217905550670de0b6b3a7640000840393508350670de0b6b3a7640000600060008282825054019250508190555"
           "0600"
           "260016000505411151561011d5760038054829081101561000257906000526020600020900160009054906101000a9004600160"
           "a060"
           "020a0316600160a060020a03166000600060005054604051809050600060405180830381858888f150505080555060016002556"
           "101d"
           "5565b60018054016060908152602090f35b005b60018054600354910114156101d5576003805460025460010190910290039250"
           "5b60"
           "03546002549003600119018310156101e357600380548490811015610002579082526040517fc2575a0e9e593c00f959f8c92f1"
           "2db2"
           "869c3395a3b0502d05e2516446f71f85b9190910154600160a060020a03169082906706f05b59d3b200009082818181858883f1"
           "5050"
           "90546706f05b59d3b1ffff1901835550506001929092019161013e565b505060028054600101905550505b60008054850190555"
           "0505"
           "050565b506002548154919250600190810190910460001901905b60035460025490036001190183101561029a57600380548490"
           "8110"
           "156100025760009182526040517fc2575a0e9e593c00f959f8c92f12db2869c3395a3b0502d05e2516446f71f85b91909101546"
           "0016"
           "0a060020a03169190838504600019019082818181858883f1505081548486049003600190810190925550600290830183020460"
           "0019"
           "01841415905061028e576001015b600192909201916101fa565b60038054600254810182018083559190829080158290116101c"
           "7576"
           "0008390526101c7907fc2575a0e9e593c00f959f8c92f12db2869c3395a3b0502d05e2516446f71f85b9081019083015b808211"
           "1561"
           "02fa57600081556001016102e6565b509056";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_getStorageAt(const string& param01, const string& param02, const string& param03) {
    std::cerr << "Serviced eth_getCode(" << param01 << "," << param02 << "," << param03 << ")" << std::endl;
    return "0x0000000000000000000000000000000000000000000000001bc16d674ec80000";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_blockNumber() {
    std::cerr << "Serviced eth_blockNumber()" << std::endl;
    return "from eth_blockNumber";
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::eth_syncing() {
    std::cerr << "Serviced eth_syncing()" << std::endl;
    Json::Value v1;
    v1["currentBlock"] = "0xad7b02";
    v1["highestBlock"] = "0xad7e17";
    return v1;
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_chainId() {
    std::cerr << "Serviced eth_chainId()" << std::endl;
    return "0x1";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_protocolVersion() {
    std::cerr << "Serviced eth_protocolVersion()" << std::endl;
    return "0x41";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_gasPrice() {
    std::cerr << "Serviced eth_gasPrice()" << std::endl;
    return "0x4e3b29200";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_call(const Json::Value& param01, const string& param02) {
    std::cerr << "Serviced eth_getCode(" << param01 << "," << param02 << ")" << std::endl;
    return "0x0000000000000000000000000000000000000000000c685fa11e01ec6f000000";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_estimateGas(const Json::Value& param01) {
    std::cerr << "Serviced eth_estimateGas(" << param01 << ")" << std::endl;
    return "0x5208";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_sendTransaction(const Json::Value& param01) {
    std::cerr << "Serviced eth_getCode(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "eth_sendTransaction");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_sendRawTransaction(const string& param01) {
    std::cerr << "Serviced eth_sendRawTransaction(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "eth_sendRawTransaction");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_getProof(const string& param01, const Json::Value& param02, const string& param03) {
    std::cerr << "Serviced eth_getProof(" << param01 << "," << param02 << "," << param03 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "eth_getProof");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_coinbase() {
    std::cerr << "Serviced eth_coinbase()" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "eth_coinbase");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_hashrate() {
    std::cerr << "Serviced eth_hashrate()" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "eth_hashrate");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
bool EthRpcServer::eth_mining() {
    std::cerr << "Serviced eth_mining()" << std::endl;
    return false;
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::eth_getWork() {
    std::cerr << "Serviced eth_getWork()" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "eth_getWork");
    throw excep;
    Json::Value v1;
    return v1;
}

//---------------------------------------------------------------------------------------
bool EthRpcServer::eth_submitWork(const string& param01, const string& param02, const string& param03) {
    std::cerr << "Serviced eth_getCode(" << param01 << "," << param02 << "," << param03 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "eth_submitWork");
    throw excep;
    return false;
}

//---------------------------------------------------------------------------------------
bool EthRpcServer::eth_submitHashrate(const string& param01, const string& param02) {
    std::cerr << "Serviced eth_submitHashrate(" << param01 << "," << param02 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "eth_submitHashrate");
    throw excep;
    return false;
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::trace_call(const Json::Value& param01, const Json::Value& param02, const string& param03) {
    std::cerr << "Serviced trace_call(" << param01 << "," << param02 << "," << param03 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "trace_call");
    throw excep;
    Json::Value v1;
    return v1;
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::trace_callMany(const Json::Value& param01, const string& param02) {
    std::cerr << "Serviced trace_callMany(" << param01 << "," << param02 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "trace_callMany");
    throw excep;
    Json::Value v1;
    return v1;
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::trace_rawTransaction(const string& param01, const Json::Value& param02) {
    std::cerr << "Serviced trace_rawTransaction(" << param01 << "," << param02 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "trace_rawTransaction");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::trace_replayBlockTransactions(const string& param01, const Json::Value& param02) {
    std::cerr << "Serviced trace_replayBlockTransactions(" << param01 << "," << param02 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "trace_replayBlockTransactions");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::trace_replayTransaction(const string& param01, const Json::Value& param02) {
    std::cerr << "Serviced trace_replayTransaction(" << param01 << "," << param02 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "trace_replayTransaction");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::trace_transaction(const string& param01) {
    std::cerr << "Serviced trace_transaction(" << param01 << ")" << std::endl;
    return getTestTrace(2);
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::trace_get(const string& param01, const Json::Value& param02) {
    std::cerr << "Serviced trace_get(" << param01 << "," << param02 << ")" << std::endl;
    return getTestTrace(1);
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::trace_block(const string& param01) {
    std::cerr << "Serviced trace_block(" << param01 << ")" << std::endl;
    return getTestTrace();
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::trace_filter(const Json::Value& param01) {
    std::cerr << "Serviced trace_filter(" << param01 << ")" << std::endl;
    return getTestTrace();
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::tg_forks() {
    std::cerr << "Serviced tg_forks()" << std::endl;
    Json::Value array;
    array[0] = 1150000;
    array[1] = 1920000;
    array[2] = 2463000;
    array[3] = 2675000;
    array[4] = 4370000;
    array[5] = 7280000;
    array[6] = 9069000;
    array[7] = 9200000;
    Json::Value v1;
    v1["genesis"] = "0xd4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8fa3";
    v1["forks"] = array;
    return v1;
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::tg_getHeaderByNumber(const string& param01) {
    std::cerr << "Serviced tg_getHeaderByNumber(" << param01 << ")" << std::endl;
    return getTestHeader(param01);
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::tg_getHeaderByHash(const string& param01) {
    std::cerr << "Serviced tg_getHeaderByHash(" << param01 << ")" << std::endl;
    return getTestHeader(param01);
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::tg_getLogsByHash(const string& param01) {
    std::cerr << "Serviced tg_getLogsByHash(" << param01 << ")" << std::endl;
    return getTestLogByHash();
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::tg_issuance(const string& param01) {
    std::cerr << "Serviced tg_issuance(" << param01 << ")" << std::endl;
    Json::Value v1;
    v1["blockReward"] = "0x478eae0e571ba000";
    v1["uncleReward"] = "0x340aad21b3b70000";
    v1["issuance"] = "0x7b995b300ad2a000";
    return v1;
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::debug_storageRangeAt(const string& param01, int param02, const string& param03,
                                               const string& param04, int param05) {
    std::cerr << "Serviced eth_getCode(" << param01 << "," << param02 << "," << param03 << "," << param04 << ","
              << param05 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "debug_storageRangeAt");
    throw excep;
    Json::Value v1;
    return v1;
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::debug_accountRange(const string& param01, const Json::Value& param02, int param03,
                                             bool param04, bool param05, bool param06) {
    std::cerr << "Serviced eth_getCode(" << param01 << "," << param02 << "," << param03 << "," << param04 << ","
              << param05 << "," << param06 << ")" << std::endl;
    Json::Value one = Json::objectValue;
    one["balance"] = "0";
    one["nonce"] = 0;
    one["root"] = "0000000000000000000000000000000000000000000000000000000000000000";
    one["codeHash"] = "c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470";

    Json::Value accounts = Json::objectValue;
    accounts["0x0000000000000000000000000000000000000001"] = one;

    Json::Value v1;
    v1["root"] = "0x8d8f6ffa5f2e55c0f8f0b88c3421d647e497f3ee0d66825f3f7433d7e244dde8";
    v1["accounts"] = accounts;
    v1["next"] = "AAAAAAAAAAAAAAAAAAAAAAAAAAI=";
    return v1;
}

//---------------------------------------------------------------------------------------
string EthRpcServer::debug_getModifiedAccountsByNumber(const string& param01, const string& param02) {
    std::cerr << "Serviced debug_getModifiedAccountsByNumber(" << param01 << "," << param02 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "debug_getModifiedAccountsByNumber");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::debug_getModifiedAccountsByHash(const string& param01, const string& param02) {
    std::cerr << "Serviced debug_getModifiedAccountsByHash(" << param01 << "," << param02 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "debug_getModifiedAccountsByHash");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::debug_traceTransaction(const string& param01) {
    std::cerr << "Serviced debug_traceTransaction(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_NOTIMPLEMENTED, "debug_traceTransaction");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_accounts() {
    std::cerr << "Serviced eth_accounts()" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "eth_accounts");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::eth_getCompilers() {
    std::cerr << "Serviced eth_getCompilers()" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "eth_getCompilers");
    throw excep;
    Json::Value v1;
    return v1;
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_compileLLL(const string& param01) {
    std::cerr << "Serviced eth_compileLLL(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "eth_compileLLL");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_compileSolidity(const string& param01) {
    std::cerr << "Serviced eth_compileSolidity(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "eth_compileSolidity");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_compileSerpent(const string& param01) {
    std::cerr << "Serviced eth_compileSerpent(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "eth_compileSerpent");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::eth_sign(const string& param01, const string& param02) {
    std::cerr << "Serviced eth_sign(" << param01 << "," << param02 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "eth_sign");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::db_getString(const string& param01, const string& param02) {
    std::cerr << "Serviced db_getString(" << param01 << "," << param02 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "db_getString");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
bool EthRpcServer::db_putString(const string& param01, const string& param02, const string& param03) {
    std::cerr << "Serviced eth_getCode(" << param01 << "," << param02 << "," << param03 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "db_putString");
    throw excep;
    return true;
}

//---------------------------------------------------------------------------------------
string EthRpcServer::db_getHex(const string& param01, const string& param02) {
    std::cerr << "Serviced db_getHex(" << param01 << "," << param02 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "db_getHex");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
bool EthRpcServer::db_putHex(const string& param01, const string& param02, const string& param03) {
    std::cerr << "Serviced eth_getCode(" << param01 << "," << param02 << "," << param03 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "db_putHex");
    throw excep;
    return true;
}

//---------------------------------------------------------------------------------------
bool EthRpcServer::shh_post(const Json::Value& param01) {
    std::cerr << "Serviced shh_post(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "shh_post");
    throw excep;
    return true;
}

//---------------------------------------------------------------------------------------
string EthRpcServer::shh_version() {
    std::cerr << "Serviced shh_version()" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "shh_version");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
string EthRpcServer::shh_newIdentity() {
    std::cerr << "Serviced shh_newIdentity()" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "shh_newIdentity");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
bool EthRpcServer::shh_hasIdentity(const string& param01) {
    std::cerr << "Serviced shh_hasIdentity(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "shh_hasIdentity");
    throw excep;
    return true;
}

//---------------------------------------------------------------------------------------
string EthRpcServer::shh_newGroup() {
    std::cerr << "Serviced shh_newGroup()" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "shh_newGroup");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
bool EthRpcServer::shh_addToGroup(const string& param01) {
    std::cerr << "Serviced shh_addToGroup(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "shh_addToGroup");
    throw excep;
    return true;
}

//---------------------------------------------------------------------------------------
string EthRpcServer::shh_newFilter(const Json::Value& param01) {
    std::cerr << "Serviced shh_newFilter(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "shh_newFilter");
    throw excep;
    return "";
}

//---------------------------------------------------------------------------------------
bool EthRpcServer::shh_uninstallFilter(const string& param01) {
    std::cerr << "Serviced shh_uninstallFilter(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "shh_uninstallFilter");
    throw excep;
    return true;
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::shh_getFilterChanges(const string& param01) {
    std::cerr << "Serviced shh_getFilterChanges(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "shh_getFilterChanges");
    throw excep;
    Json::Value result;
    return result;
}

//---------------------------------------------------------------------------------------
Json::Value EthRpcServer::shh_getMessages(const string& param01) {
    std::cerr << "Serviced shh_getMessages(" << param01 << ")" << std::endl;
    JsonRpcException excep(TG_ERROR_SERVER_DEPRECATED, "shh_getMessages");
    throw excep;
    Json::Value result;
    return result;
}

//---------------------------------------------------------------------------------------
Json::Value getTestBlock(const string& id, bool txHashes) {
    Json::Value result;
    // result["author"] = "0x0c729be7c39543c3d549282a40395299d987cec2";
    result["difficulty"] = "0xb5708d578a6";
    result["extraData"] = "0xd783010400844765746887676f312e352e31856c696e7578";
    result["gasLimit"] = "0x2fefd8";
    result["gasUsed"] = "0x14820";
    result["hash"] = "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53";
    result["logsBloom"] =
        "0x0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000";
    result["miner"] = "0x0c729be7c39543c3d549282a40395299d987cec2";
    result["mixHash"] = "0x1530cda332d86d5d7462e3a0eb585e22c88348dd796d29e6ef18196a78cdce07";
    result["nonce"] = "0x938e5630b060b7d3";
    result["number"] = id;
    result["parentHash"] = "0x96810a6076e621e311a232468bfd3dcfac08f4803b255af0f00300f47981c10f";
    result["receiptsRoot"] = "0x075608bec75d988c52ea6750f4c2204fd60082eb1df32cf8f4732e8a591eef62";
    result["sha3Uncles"] = "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347";
    result["size"] = "0x3e1";
    result["stateRoot"] = "0xb3f9408d80048b6f206951c4e387f8da37fb8510eccc18527865fa746c47bbc5";
    result["totalDifficulty"] = "0x6332227c16fd7c67";
    result["timestamp"] = "0x56bff9bb";
    result["transactionsRoot"] = "0xb779480508401ddd57f1f1e83a54715dcafc6ccec4e4d842c1b68cb418e6560d";
    Json::Value txs;
    txs[0u] = getTestTrans("0x730724cb08a6eb17bf6b3296359d261570d343ea7944a17a9d7287d77900db08", txHashes);
    txs[1u] = getTestTrans("0xef2ea39c20ba09553b2f3cf02380406ac766039ca56612937eed5e7f3503fb3a", txHashes);
    txs[2u] = getTestTrans("0x5352c80aa2073e21ce6c4aa5488c38455f3519955ece7dca5af3e326797bcc63", txHashes);
    txs[3u] = getTestTrans("0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521", txHashes);
    result["transactions"] = txs;
    result["uncles"] = Json::Value(Json::arrayValue);
    return result;
}

//---------------------------------------------------------------------------------------
Json::Value getTestUncle(const string& id, const string& index) {
    Json::Value result;
    result["difficulty"] = id;     // supress unused param warning
    result["difficulty"] = index;  // supress unused param warning
    result["difficulty"] = "0x3ff800000";
    result["extraData"] = "0x59617465732052616e64616c6c202d2045746865724e696e6a61";
    result["gasLimit"] = "0x1388";
    result["gasUsed"] = "0x0";
    result["hash"] = "0x5cd50096dbb856a6d1befa6de8f9c20decb299f375154427d90761dc0b101109";
    result["logsBloom"] =
        "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000"
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000"
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000"
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000"
        "00000000000000000000000000000000000000000000000000000000000000000000000000";
    result["miner"] = "0xc8ebccc5f5689fa8659d83713341e5ad19349448";
    result["mixHash"] = "0xf8c94dfe61cf26dcdf8cffeda337cf6a903d65c449d7691a022837f6e2d99459";
    result["nonce"] = "0x68b769c5451a7aea";
    result["number"] = "0x1";
    result["parentHash"] = "0xd4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8fa3";
    result["receiptsRoot"] = "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
    result["sha3Uncles"] = "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347";
    result["size"] = "0x21a";
    result["stateRoot"] = "0x1e6e030581fd1873b4784280859cd3b3c04aa85520f08c304cf5ee63d3935add";
    result["timestamp"] = "0x55ba4242";
    result["totalDifficulty"] = "0xffd003ffe";
    result["transactionsRoot"] = "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
    return result;
}

//---------------------------------------------------------------------------------------
Json::Value getTestTrans(const string& id, bool fullTx) {
    Json::Value result;
    if (!fullTx) {
        result = id;
        return result;
    }
    result["blockHash"] = "0x785b221ec95c66579d5ae14eebe16284a769e948359615d580f02e646e93f1d5";
    result["blockNumber"] = "0x52a90b";
    result["from"] = "0x11b6a5fe2906f3354145613db0d99ceb51f604c9", result["gas"] = "0x6b6c";
    result["gasPrice"] = "0x11e1a300";
    result["hash"] = "0xb2fea9c4b24775af6990237aa90228e5e092c56bdaee74496992a53c208da1ee";
    result["input"] =
        "0x80dfa34a000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000"
        "0000000000000000000000000000002e516d556558334448416654747442464a42315454384a617a67765744776a727a7342686973"
        "693473547532613551000000000000000000000000000000000000";
    result["nonce"] = "0x10";
    result["r"] = "0xacdf839bdcb6653da60900f739076a00ecbe0059fa046933348e9b68a62a222";
    result["s"] = "0x132a0517a4c52916e0c6b0e74b0479326891df2a9afd711482c7f3919b335ff6";
    result["to"] = "0xfa28ec7198028438514b49a3cf353bca5541ce1d";
    result["transactionIndex"] = "0x25";
    result["v"] = "0x26";
    result["value"] = "0x0";
    return result;
}

//---------------------------------------------------------------------------------------
Json::Value getTestLog(uint32_t which) {
    Json::Value result;
    Json::Value topics(Json::arrayValue);
    switch (which) {
        case 0:
            result["address"] = "0xd6df5935cd03a768b7b9e92637a01b25e24cb709";
            topics[0u] = "0x8940c4b8e215f8822c5c8f0056c12652c746cbc57eedbd2a440b175971d47a77";
            topics[1u] = "0x000000000000000000000000d907941c8b3b966546fc408b8c942eb10a4f98df";
            result["data"] = "0x0000000000000000000000000000000000000000000000000000008bb2c97000";
            result["blockNumber"] = "0x52a975";
            result["transactionHash"] = "0xa3ece39ae137617669c6933b7578b94e705e765683f260fcfe30eaa41932610f";
            result["transactionIndex"] = "0x29";
            result["blockHash"] = "0xf6084155ff2022773b22df3217d16e9df53cbc42689b27ca4789e06b6339beb2";
            result["logIndex"] = "0x119";
            result["removed"] = false;
            return result;
        case 1:
            result["address"] = "0xd6df5935cd03a768b7b9e92637a01b25e24cb709";
            topics[0u] = "0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef";
            topics[1u] = "0x0000000000000000000000000000000000000000000000000000000000000000";
            topics[2u] = "0x000000000000000000000000d907941c8b3b966546fc408b8c942eb10a4f98df";
            result["data"] = "0x0000000000000000000000000000000000000000000000000000008bb2c97000";
            result["blockNumber"] = "0x52a975";
            result["transactionHash"] = "0xa3ece39ae137617669c6933b7578b94e705e765683f260fcfe30eaa41932610f";
            result["transactionIndex"] = "0x29";
            result["blockHash"] = "0xf6084155ff2022773b22df3217d16e9df53cbc42689b27ca4789e06b6339beb2";
            result["logIndex"] = "0x11a";
            result["removed"] = false;
            return result;
        default:
            return result;
    }
}

//---------------------------------------------------------------------------------------
Json::Value getTestReceipt(const string& id) {
    Json::Value result;
    result["blockHash"] = "0xf6084155ff2022773b22df3217d16e9df53cbc42689b27ca4789e06b6339beb2";
    result["blockNumber"] = "0x52a975";
    result["contractAddress"] = Json::Value(Json::nullValue);
    result["cumulativeGasUsed"] = "0x797db0";
    result["from"] = "0xd907941c8b3b966546fc408b8c942eb10a4f98df";
    result["gasUsed"] = "0x1308c";
    result["logsBloom"] =
        "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000008000000000000000"
        "0000"
        "0000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000002000000000000"
        "0000"
        "0008000000000000000040100000101000000000000000000000000000000000000000000000000000400000800000000000000800"
        "0000"
        "0000000000000000000000000000000000000020000000000000000000000002000000000000000000000000000000000000000000"
        "0000"
        "00000020000000010000000000000000000000000000000000000000000000000000000000";
    result["status"] = "0x1";
    result["to"] = "0xd6df5935cd03a768b7b9e92637a01b25e24cb709";
    result["transactionHash"] = id;
    result["transactionIndex"] = "0x29";
    Json::Value logs = Json::Value(Json::arrayValue);
    logs[0u] = getTestLog(0);
    logs[1u] = getTestLog(1);
    result["logs"] = logs;
    return result;
}

//---------------------------------------------------------------------------------------
Json::Value getTestHeader(const string& id) {
    Json::Value result;
    result["difficulty"] = "0x3fe802ffe";
    result["extraData"] = "0x476574682f76312e302e302d66633739643332642f6c696e75782f676f312e34";
    result["gasLimit"] = "0x1388";
    result["gasUsed"] = "0x0";
    result["hash"] = id.size() > 10 ? id : "0x3d6122660cc824376f11ee842f83addc3525e2dd6756b9bcf0affa6aa88cf741";
    result["logsBloom"] =
        "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000"
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000"
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000"
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "0000"
        "00000000000000000000000000000000000000000000000000000000000000000000000000";
    result["miner"] = "0x5088d623ba0fcf0131e0897a91734a4d83596aa0";
    result["mixHash"] = "0x65e12eec23fe6555e6bcdb47aa25269ae106e5f16b54e1e92dcee25e1c8ad037";
    result["nonce"] = "0x2e9344e0cbde83ce";
    result["number"] = id.size() < 10 ? id : "0x3";
    result["parentHash"] = "0xb495a1d7e6663152ae92708da4843337b958146015a2802f4193a410044698c9";
    result["receiptsRoot"] = "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
    result["sha3Uncles"] = "0x6b17b938c6e4ef18b26ad81b9ca3515f27fd9c4e82aac56a1fd8eab288785e41";
    result["stateRoot"] = "0x76ab0b899e8387436ff2658e2988f83cbf1af1590b9fe9feca3714f8d1824940";
    result["timestamp"] = "0x55ba4260";
    result["transactionsRoot"] = "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
    return result;
}

//---------------------------------------------------------------------------------------
Json::Value getTestLogByHash(void) {
    Json::Value topics = Json::Value(Json::arrayValue);
    topics[0] = "0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef";
    topics[1] = "0x000000000000000000000000001866ae5b3de6caa5a51543fd9fb64f524f5478";
    topics[2] = "0x00000000000000000000000016a9c11e229ce221578a9adb3e7c0a48482e8063";

    Json::Value logs = Json::Value(Json::arrayValue);
    logs[0]["address"] = "0xb8c77482e45f1f44de1745f52c74426c631bdd52";
    logs[0]["topics"] = topics;
    logs[0]["data"] = "0x00000000000000000000000000000000000000000000021ea4a7ecbf3c280000";
    logs[0]["blockNumber"] = "0x3d0cec";
    logs[0]["transactionHash"] = "0x99f91752d50d0c2c92e681fda082843747e8284d846f8b623e4cd280fbd7bb65";
    logs[0]["transactionIndex"] = "0x2";
    logs[0]["blockHash"] = "0x2f244c154cbacb0305581295b80efa6dffb0224b60386a5fc6ae9585e2a140c4";
    logs[0]["logIndex"] = "0x0";
    logs[0]["removed"] = false;

    Json::Value result = Json::Value(Json::arrayValue);
    result[0] = Json::nullValue;
    result[1] = Json::nullValue;
    result[2] = logs;
    return result;
}

//---------------------------------------------------------------------------------------
Json::Value getTestTrace(uint32_t which) {
    if (which == 1) {
        Json::Value action1;
        action1["callType"] = "call";
        action1["from"] = "0x1c39ba39e4735cb65978d4db400ddd70a72dc750";
        action1["gas"] = "0x13e99";
        action1["input"] = "0x16c72721";
        action1["to"] = "0x2bd2326c993dfaef84f696526064ff22eba5b362";
        action1["value"] = "0x0";
        Json::Value res1;
        res1["gasUsed"] = "0x183";
        res1["output"] = "0x0000000000000000000000000000000000000000000000000000000000000001";
        Json::Value trace1;
        trace1["action"] = action1;
        trace1["blockHash"] = "0x7eb25504e4c202cf3d62fd585d3e238f592c780cca82dacb2ed3cb5b38883add";
        trace1["blockNumber"] = 3068185;
        trace1["result"] = res1;
        trace1["subtraces"] = 0;
        trace1["traceAddress"] = Json::arrayValue;
        trace1["traceAddress"][0] = 0;
        trace1["transactionHash"] = "0x17104ac9d3312d8c136b7f44d4b8b47852618065ebfa534bd2d3b5ef218ca1f3";
        trace1["transactionPosition"] = 2;
        trace1["type"] = "call";
        return trace1;

    } else if (which == 2) {
        Json::Value action2;
        action2["callType"] = "call";
        action2["from"] = "0x83806d539d4ea1c140489a06660319c9a303f874";
        action2["gas"] = "0x1a1f8";
        action2["input"] = "0x";
        action2["to"] = "0x1c39ba39e4735cb65978d4db400ddd70a72dc750";
        action2["value"] = "0x7a16c911b4d00000";
        Json::Value res2;
        res2["gasUsed"] = "0x2982";
        res2["output"] = "0x";
        Json::Value trace2;
        trace2["action"] = action2;
        trace2["blockHash"] = "0x7eb25504e4c202cf3d62fd585d3e238f592c780cca82dacb2ed3cb5b38883add";
        trace2["blockNumber"] = 3068185;
        trace2["result"] = res2;
        trace2["subtraces"] = 2;
        trace2["traceAddress"] = Json::arrayValue;
        trace2["transactionHash"] = "0x17104ac9d3312d8c136b7f44d4b8b47852618065ebfa534bd2d3b5ef218ca1f3";
        trace2["transactionPosition"] = 2;
        trace2["type"] = "call";

        Json::Value action3;
        action3["callType"] = "call";
        action3["from"] = "0x1c39ba39e4735cb65978d4db400ddd70a72dc750";
        action3["gas"] = "0x13e99";
        action3["input"] = "0x16c72721";
        action3["to"] = "0x2bd2326c993dfaef84f696526064ff22eba5b362";
        action3["value"] = "0x0";
        Json::Value res3;
        res3["gasUsed"] = "0x183";
        res3["output"] = "0x0000000000000000000000000000000000000000000000000000000000000001";
        Json::Value trace3;
        trace3["action"] = action3;
        trace3["blockHash"] = "0x7eb25504e4c202cf3d62fd585d3e238f592c780cca82dacb2ed3cb5b38883add";
        trace3["blockNumber"] = 3068185;
        trace3["result"] = res3;
        trace3["subtraces"] = 0;
        trace3["traceAddress"] = Json::arrayValue;
        trace3["traceAddress"][0] = 0;
        trace3["transactionHash"] = "0x17104ac9d3312d8c136b7f44d4b8b47852618065ebfa534bd2d3b5ef218ca1f3";
        trace3["transactionPosition"] = 2;
        trace3["type"] = "call";

        Json::Value action4;
        action4["callType"] = "call";
        action4["from"] = "0x1c39ba39e4735cb65978d4db400ddd70a72dc750";
        action4["gas"] = "0x8fc";
        action4["input"] = "0x";
        action4["to"] = "0x70faa28a6b8d6829a4b1e649d26ec9a2a39ba413";
        action4["value"] = "0x7a16c911b4d00000";
        Json::Value res4;
        res4["gasUsed"] = "0x0";
        res4["output"] = "0x";
        Json::Value trace4;
        trace4["action"] = action4;
        trace4["blockHash"] = "0x7eb25504e4c202cf3d62fd585d3e238f592c780cca82dacb2ed3cb5b38883add";
        trace4["blockNumber"] = 3068185;
        trace4["result"] = res4;
        trace4["subtraces"] = 0;
        trace4["traceAddress"] = Json::arrayValue;
        trace4["traceAddress"][0] = 1;
        trace4["transactionHash"] = "0x17104ac9d3312d8c136b7f44d4b8b47852618065ebfa534bd2d3b5ef218ca1f3";
        trace4["transactionPosition"] = 2;
        trace4["type"] = "call";

        Json::Value result = Json::arrayValue;
        result[0] = trace2;
        result[1] = trace3;
        result[2] = trace4;
        return result;
    }
    Json::Value action1;
    action1["author"] = "0x5088d623ba0fcf0131e0897a91734a4d83596aa0";
    action1["rewardType"] = "block";
    action1["value"] = "0x478eae0e571ba000";
    Json::Value res1 = Json::objectValue;
    Json::Value trace1;
    trace1["action"] = action1;
    trace1["result"] = res1;
    trace1["blockHash"] = "0x3d6122660cc824376f11ee842f83addc3525e2dd6756b9bcf0affa6aa88cf741";
    trace1["blockNumber"] = 3;
    trace1["subtraces"] = 0;
    trace1["traceAddress"] = Json::nullValue;
    trace1["transactionHash"] = "0x0000000000000000000000000000000000000000000000000000000000000000";
    trace1["transactionPosition"] = 0;
    trace1["type"] = "reward";

    Json::Value action2;
    action2["author"] = "0xc8ebccc5f5689fa8659d83713341e5ad19349448";
    action2["rewardType"] = "uncle";
    action2["value"] = "0x340aad21b3b70000";
    Json::Value res2 = Json::objectValue;
    Json::Value trace2;
    trace2["action"] = action2;
    trace2["result"] = res2;
    trace2["blockHash"] = "0x3d6122660cc824376f11ee842f83addc3525e2dd6756b9bcf0affa6aa88cf741";
    trace2["blockNumber"] = 3;
    trace2["subtraces"] = 0;
    trace2["traceAddress"] = Json::nullValue;
    trace2["transactionHash"] = "0x0000000000000000000000000000000000000000000000000000000000000000";
    trace2["transactionPosition"] = 0;
    trace2["type"] = "reward";

    Json::Value result;
    result[0] = trace1;
    result[1] = trace2;
    return result;
}

//---------------------------------------------------------------------------------------
int main() {
    HttpServer httpserver(8383);
    EthRpcServer server(httpserver);
    if (server.StartListening()) {
        cout << "Listening on :8383. Hit enter to stop the server" << endl;
        getchar();
        server.StopListening();
    } else {
        cerr << "Could not start listening. Quitting..." << endl;
    }
    return 0;
}
