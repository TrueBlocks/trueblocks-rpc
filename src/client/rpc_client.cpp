/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    rpc_client.cpp
 * @date    01.05.2013
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <jsonrpccpp/client/httpclient.h>

#include <iostream>

#include "gen/rpc_client.h"

using namespace jsonrpc;
using namespace std;

int main() {
    HttpClient httpclient("http://localhost:8383");
    RpctClient client(httpclient);

    try {
#define CALL_ONE(a)                                                                                                    \
    {                                                                                                                  \
        cerr << "--> " << #a << endl;                                                                                  \
        cout << (a) << endl;                                                                                           \
    }

#define CALL_ONE1(a)

        CALL_ONE1(client.web3_clientVersion());
        CALL_ONE1(client.web3_sha3("0x68656c6c6f20776f726c64"));
        CALL_ONE1(client.net_listening());
        CALL_ONE1(client.net_version());
        CALL_ONE1(client.net_peerCount());
        CALL_ONE1(client.eth_getBlockByNumber("0xf4629", false));
        CALL_ONE1(
            client.eth_getBlockByHash("0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53", false));
        CALL_ONE1(client.eth_getBlockTransactionCountByNumber("0xf4629"));
        CALL_ONE1(client.eth_getBlockTransactionCountByHash(
            "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53"));
        CALL_ONE1(
            client.eth_getTransactionByHash("0xb2fea9c4b24775af6990237aa90228e5e092c56bdaee74496992a53c208da1ee"));
        CALL_ONE1(client.eth_getTransactionByBlockHashAndIndex(
            "0x785b221ec95c66579d5ae14eebe16284a769e948359615d580f02e646e93f1d5", "0x25"));
        CALL_ONE1(client.eth_getTransactionByBlockNumberAndIndex("0x52a90b", "0x25"));
        CALL_ONE(
            client.eth_getTransactionReceipt("0xb2fea9c4b24775af6990237aa90228e5e092c56bdaee74496992a53c208da1ee"));
        CALL_ONE1(client.eth_getUncleByBlockNumberAndIndex("0x12", "0x12"));
        CALL_ONE1(client.eth_getUncleByBlockHashAndIndex("0x12", "0x12"));
        CALL_ONE1(client.eth_getUncleCountByBlockNumber("0x12"));
        CALL_ONE1(client.eth_getUncleCountByBlockHash("0x12"));
        CALL_ONE1(client.eth_newPendingTransactionFilter());
        CALL_ONE1(client.eth_newBlockFilter());
        Json::Value v1;
        Json::Value v2;
        // CALL_ONE(client.eth_newFilter("0x12", "0x12", "0x12", v1, v2))
        // CALL_ONE(client.eth_newFilter("0x12", "0x2", "0x2", v1, v2);
        CALL_ONE1(client.eth_uninstallFilter("0x12"));
        // CALL_ONE(client.eth_getFilterChanges("0x12"));
        // CALL_ONE(client.eth_getLogs("0x12"));
        CALL_ONE1(client.eth_getBalance("0x12", "0x2"));
        CALL_ONE1(client.eth_getTransactionCount("0x12", "0x2"));
        CALL_ONE1(client.eth_getCode("0x12", "0x2"));
        CALL_ONE1(client.eth_getStorageAt("0x12", "0x2", "0x2"));
        CALL_ONE1(client.eth_blockNumber());
        // CALL_ONE(client.eth_syncing());
        CALL_ONE1(client.eth_chainId());
        CALL_ONE1(client.eth_protocolVersion());
        CALL_ONE1(client.eth_gasPrice());
        CALL_ONE1(client.eth_call("0x12", "0x2", "0x2", "0x2", "0x2", "0x2", "0x2"));
        CALL_ONE1(client.eth_estimateGas("0x12"));
        CALL_ONE1(client.eth_sendTransaction("0x12", "0x2", "0x2", "0x2", "0x2", "0x2", "0x2"));
        CALL_ONE1(client.eth_sendRawTransaction("0x12"));
        CALL_ONE1(client.eth_getProof("0x12"));
        CALL_ONE1(client.eth_coinbase());
        CALL_ONE1(client.eth_hashrate());
        CALL_ONE1(client.eth_mining());
        // CALL_ONE(client.eth_getWork());
        CALL_ONE1(client.eth_submitWork("0x12", "0x2", "0x2"));
        CALL_ONE1(client.eth_submitHashrate("0x12", "0x2"));
        CALL_ONE1(client.trace_call("0x12"));
        CALL_ONE1(client.trace_callMany("0x12"));
        CALL_ONE1(client.trace_rawTransaction("0x12"));
        CALL_ONE1(client.trace_replayBlockTransactions("0x12"));
        CALL_ONE1(client.trace_replayTransaction("0x12"));
        CALL_ONE1(client.trace_transaction("0x12"));
        CALL_ONE1(client.trace_get("0x12"));
        CALL_ONE1(client.trace_block("0x12"));
        CALL_ONE1(client.trace_filter("0x12"));
        CALL_ONE1(client.tg_forks());
        CALL_ONE1(client.tg_getHeaderByNumber("0x3"));
        CALL_ONE1(client.tg_getHeaderByHash("0x12"));
        CALL_ONE1(client.tg_getLogsByHash("0x12"));
        CALL_ONE1(client.tg_issuance("0x12"));
        CALL_ONE1(client.debug_storageRangeAt("0x12", "0x2", "0x2", "0x2", "0x2"));
        CALL_ONE1(client.debug_accountRange("0x12", "0x2", "0x2", true, true, true));
        CALL_ONE1(client.debug_getModifiedAccountsByNumber("0x12", "0x2"));
        CALL_ONE1(client.debug_getModifiedAccountsByHash("0x12", "0x2"));
        CALL_ONE1(client.debug_traceTransaction("0x12"));
        CALL_ONE1(client.eth_accounts());
        // CALL_ONE(client.eth_getCompilers());
        CALL_ONE1(client.eth_compileLLL("0x12"));
        CALL_ONE1(client.eth_compileSolidity("0x12"));
        CALL_ONE1(client.eth_compileSerpent("0x12"));
#if 0
        CALL_ONE1(client.eth_sign(strs));
#endif
        CALL_ONE1(client.db_getString("testDB", "myKey"));
        CALL_ONE1(client.db_putString("0x12", "0x2", "0x2"));
        CALL_ONE1(client.db_getHex("0x12", "0x2"));
        CALL_ONE1(client.db_putHex("0x12", "0x2", "0x2"));
        CALL_ONE1(client.shh_post("0x12"));
        CALL_ONE1(client.shh_version());
        CALL_ONE1(client.shh_newIdentity());
        CALL_ONE1(client.shh_hasIdentity("0x12"));
        CALL_ONE1(client.shh_newGroup());
        CALL_ONE1(client.shh_addToGroup("0x12"));
        CALL_ONE1(client.shh_newFilter("0x12"));
        CALL_ONE1(client.shh_uninstallFilter("0x12"));
        CALL_ONE1(client.shh_getFilterChanges("0x12"));
        CALL_ONE1(client.shh_getMessages("0x12"));
        cout << endl;
    } catch (JsonRpcException& e) {
        cerr << e.what() << endl;
    }
}
