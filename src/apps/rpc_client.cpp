/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#include <iostream>

#include <rpclib/http_client.h>
#include <gen/rpc_client.h>

using namespace jsonrpc;
using namespace std;

//---------------------------------------------------------------------------------------
int main() {
    HttpClient httpclient("http://localhost:8383");
    RpctClient client(httpclient);

#define TRY_ONE(a)                                                                                                     \
    try {                                                                                                              \
        cout << a << endl;                                                                                             \
    } catch (JsonRpcException & e) {                                                                                   \
        cerr << e.what() << endl;                                                                                      \
    }

    TRY_ONE(client.web3_clientVersion());
    TRY_ONE(client.web3_sha3("0x68656c6c6f20776f726c64"));
    TRY_ONE(client.net_listening());
    TRY_ONE(client.net_version());
    TRY_ONE(client.net_peerCount());
    TRY_ONE(client.eth_getBlockByNumber("0xf4629", false));
    TRY_ONE(client.eth_getBlockByHash("0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53", false));
    TRY_ONE(client.eth_getBlockTransactionCountByNumber("0xf4629"));
    TRY_ONE(client.eth_getBlockTransactionCountByHash(
        "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53"));
    TRY_ONE(client.eth_getTransactionByHash("0xb2fea9c4b24775af6990237aa90228e5e092c56bdaee74496992a53c208da1ee"));
    TRY_ONE(client.eth_getTransactionByBlockHashAndIndex(
        "0x785b221ec95c66579d5ae14eebe16284a769e948359615d580f02e646e93f1d5", "0x25"));
    TRY_ONE(client.eth_getTransactionByBlockNumberAndIndex("0x52a90b", "0x25"));
    TRY_ONE(client.eth_getTransactionReceipt("0xb2fea9c4b24775af6990237aa90228e5e092c56bdaee74496992a53c208da1ee"));
    TRY_ONE(client.eth_getUncleByBlockNumberAndIndex("0x12", "0x12"));
    TRY_ONE(client.eth_getUncleByBlockHashAndIndex("0x12", "0x12"));
    TRY_ONE(client.eth_getUncleCountByBlockNumber("0x12"));
    TRY_ONE(client.eth_getUncleCountByBlockHash("0x12"));
    // // client.eth_newPendingTransactionFilter();
    // // client.eth_newBlockFilter();
    // jsonval_t v1;
    // jsonval_t v2;
    // // client.eth_newFilter("0x12", "0x12", "0x12", v1, v2))
    // // client.eth_newFilter("0x12", "0x2", "0x2", v1, v2);
    // // client.eth_uninstallFilter("0x12");
    // // client.eth_getFilterChanges("0x12");
    // // client.eth_getLogs("0x12");
    TRY_ONE(client.eth_getBalance("0x12", "0x2"));
    TRY_ONE(client.eth_getTransactionCount("0x12", "0x2"));
    TRY_ONE(client.eth_getCode("0x12", "0x2"));
    TRY_ONE(client.eth_getStorageAt("0x12", "0x2", "0x2"));
    TRY_ONE(client.eth_blockNumber());
    // // client.eth_syncing();
    // client.eth_chainId();
    // client.eth_protocolVersion();
    // client.eth_gasPrice();
    // v1.clear();
    // v1[0] = "0x12";
    // v1[1] = "0x12";
    // v1[2] = "0x12";
    // v1[3] = "0x12";
    // v1[4] = "0x12";
    // // client.eth_call(v1, "0x2");
    // // client.eth_estimateGas("0x12");
    // // client.eth_sendTransaction(v1);
    // // client.eth_sendRawTransaction("0x12");
    // // client.eth_getProof("0x12");
    // // client.eth_coinbase();
    // // client.eth_hashrate();
    // // client.eth_mining();
    // // client.eth_getWork();
    // // client.eth_submitWork("0x12", "0x2", "0x2");
    // // client.eth_submitHashrate("0x12", "0x2");
    // // client.trace_call("0x12");
    // // client.trace_callMany("0x12");
    // // client.trace_rawTransaction("0x12", "latest");
    // // client.trace_replayBlockTransactions("0x12");
    // // client.trace_replayTransaction("0x12");
    // // client.trace_transaction("0x12");
    // // client.trace_get("0x12");
    // // client.trace_block("0x12");
    // // client.trace_filter("0x12");
    // // client.tg_forks();
    // // client.tg_getHeaderByNumber("0x3");
    // // client.tg_getHeaderByHash("0x12");
    // // client.tg_getLogsByHash("0x12");
    // // client.tg_issuance("0x12");
    // // client.debug_storageRangeAt("0x12", "0x2", "0x2", "0x2", "0x2");
    // // client.debug_accountRange("0x12", "0x2", "0x2", true, true, true);
    // // client.debug_getModifiedAccountsByNumber("0x12", "0x2");
    // // client.debug_getModifiedAccountsByHash("0x12", "0x2");
    // // client.debug_traceTransaction("0x12");
    // // client.eth_accounts();
    // // client.eth_getCompilers();
    // // client.eth_compileLLL("0x12");
    // // client.eth_compileSolidity("0x12");
    // // client.eth_compileSerpent("0x12");
    // // client.eth_sign(strs);
    // // client.db_getString("testDB", "myKey");
    // // client.db_putString("0x12", "0x2", "0x2");
    // // client.db_getHex("0x12", "0x2");
    // // client.db_putHex("0x12", "0x2", "0x2");
    // // client.shh_post("0x12");
    // // client.shh_version();
    // // client.shh_newIdentity();
    // // client.shh_hasIdentity("0x12");
    // // client.shh_newGroup();
    // // client.shh_addToGroup("0x12");
    // // client.shh_newFilter("0x12");
    // // client.shh_uninstallFilter("0x12");
    // // client.shh_getFilterChanges("0x12");
    // // client.shh_getMessages("0x12");
}
