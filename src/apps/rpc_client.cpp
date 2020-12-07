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

    jsonval_t callObj;
    callObj[0] = "0x12";
    callObj[1] = "0x12";
    callObj[2] = "0x12";
    callObj[3] = "0x12";
    callObj[4] = "0x12";

    jsonval_t newfilter_val;
    jsonval_t v2;

    jsonval_t getlogs_val = Json::objectValue;
    jsonval_t topics = Json::arrayValue;
    topics[0] = "0x000000000000000000000000a94f5374fce5edbc8e2a8697c15331677e6ebf0b";
    getlogs_val["topics"] = topics;

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
    TRY_ONE(client.eth_getBlockTransactionCountByHash("0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53"));
    TRY_ONE(client.eth_getTransactionByHash("0xb2fea9c4b24775af6990237aa90228e5e092c56bdaee74496992a53c208da1ee"));
    TRY_ONE(client.eth_getTransactionByBlockHashAndIndex("0x785b221ec95c66579d5ae14eebe16284a769e948359615d580f02e646e93f1d5", "0x25"));
    TRY_ONE(client.eth_getTransactionByBlockNumberAndIndex("0x52a90b", "0x25"));
    TRY_ONE(client.eth_getTransactionReceipt("0xa3ece39ae137617669c6933b7578b94e705e765683f260fcfe30eaa41932610f"));
    TRY_ONE(client.eth_getUncleByBlockNumberAndIndex("0x3", "0x0"));
    TRY_ONE(client.eth_getUncleByBlockHashAndIndex("0x3d6122660cc824376f11ee842f83addc3525e2dd6756b9bcf0affa6aa88cf741", "0x0"));
    TRY_ONE(client.eth_getUncleCountByBlockNumber("0x3"));
    TRY_ONE(client.eth_getUncleCountByBlockHash("0x3d6122660cc824376f11ee842f83addc3525e2dd6756b9bcf0affa6aa88cf741"));
    TRY_ONE(client.eth_newPendingTransactionFilter());
    TRY_ONE(client.eth_newBlockFilter());
    TRY_ONE(client.eth_newFilter(newfilter_val));
    TRY_ONE(client.eth_uninstallFilter("0xdeadbeef"));
    TRY_ONE(client.eth_getFilterChanges("0xdeadbeef"));
    TRY_ONE(client.eth_getLogs(getlogs_val));
    TRY_ONE(client.eth_getBalance("0x5df9b87991262f6ba471f09758cde1c0fc1de734", "0xb443"));
    TRY_ONE(client.eth_getTransactionCount("0xfd2605a2bf58fdbb90db1da55df61628b47f9e8c", "0xc443"));
    TRY_ONE(client.eth_getCode("0x109c4f2ccc82c4d77bde15f306707320294aea3f", "0xc443"));
    TRY_ONE(client.eth_getStorageAt("0x109c4f2ccc82c4d77bde15f306707320294aea3f", "0x0", "0xc443"));
    TRY_ONE(client.eth_blockNumber());
    TRY_ONE(client.eth_syncing());
    TRY_ONE(client.eth_chainId());
    TRY_ONE(client.eth_protocolVersion());
    TRY_ONE(client.eth_gasPrice());
    // TRY_ONE(client.eth_call({"to":"0x08a2e41fb99a7599725190b9c970ad3893fa33cf", "data":"0x18160ddd"}, "0xa2f2e0"));
    // TRY_ONE(client.eth_estimateGas({"to":"0x3d597789ea16054a084ac84ce87f50df9198f415", "from":"0x3d597789ea16054a084ac84ce87f50df9198f415", "value":"0x1"}));
    // TRY_ONE(client.eth_sendTransaction({"from":"0xb60e8dd61c5d32be8058bb8eb970870f07233155", "to":"0xd46e8dd67c5d32be8058bb8eb970870f07244567", "gas":"0x76c0", "gasPrice":"0x9184e72a000", "value":"0x9184e72a", "data":"0xd46e8dd67c5d32be8d46e8dd67c5d32be8058bb8eb970870f072445675058bb8eb970870f072445675"}));
    TRY_ONE(client.eth_sendRawTransaction("0xd46e8dd67c5d32be8d46e8dd67c5d32be8058bb8eb970870f072445675058bb8eb970870f072445675"));
    // TRY_ONE(client.eth_getProof("0x7F0d15C7FAae65896648C8273B6d7E43f58Fa842", ["0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421"], "latest"}));
    TRY_ONE(client.eth_coinbase());
    TRY_ONE(client.eth_hashrate());
    TRY_ONE(client.eth_mining());
    TRY_ONE(client.eth_getWork());
    TRY_ONE(client.eth_submitWork("0x1", "0x1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef", "0xD16E5700000000000000000000000000D16E5700000000000000000000000000"));
    TRY_ONE(client.eth_submitHashrate("0x0000000000000000000000000000000000000000000000000000000000500000", "0x59daa26581d0acd1fce254fb7e85952f4c09d0915afd33d3886cd914bc7d283c"));
    // TRY_ONE(client.trace_call({"from":"0x407d73d8a49eeb85d32cf465507dd71d507100c1", "to":"0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b", "value":"0x186a0"}, ["trace", "vmTrace"], "latest"));
    // TRY_ONE(client.trace_callMany([[{"from":"0x407d73d8a49eeb85d32cf465507dd71d507100c1", "to":"0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b", "value":"0x186a0"}, ["trace"]], [{"from":"0x407d73d8a49eeb85d32cf465507dd71d507100c1", "to":"0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b", "value":"0x186a0"}, ["trace"]]], "latest"));
    // TRY_ONE(client.trace_rawTransaction("0x17104ac9d3312d8c136b7f44d4b8b47852618065ebfa534bd2d3b5ef218ca1f3", ["vmTrace"]));
    // TRY_ONE(client.trace_replayBlockTransactions("0x2", ["trace"]));
    // TRY_ONE(client.trace_replayTransaction("0x02d4a872e096445e80d05276ee756cefef7f3b376bcec14246469c0cd97dad8f", ["trace"]));
    TRY_ONE(client.trace_transaction("0x17104ac9d3312d8c136b7f44d4b8b47852618065ebfa534bd2d3b5ef218ca1f3"));
    // TRY_ONE(client.trace_get("0x17104ac9d3312d8c136b7f44d4b8b47852618065ebfa534bd2d3b5ef218ca1f3", ["0x0"]));
    TRY_ONE(client.trace_block("0x3"));
    // TRY_ONE(client.trace_filter({"fromBlock":"0x3", "toBlock":"0x3"}));
    TRY_ONE(client.tg_forks());
    TRY_ONE(client.tg_getHeaderByNumber("0x3"));
    TRY_ONE(client.tg_getHeaderByHash("0x3d6122660cc824376f11ee842f83addc3525e2dd6756b9bcf0affa6aa88cf741"));
    TRY_ONE(client.tg_getLogsByHash("0x2f244c154cbacb0305581295b80efa6dffb0224b60386a5fc6ae9585e2a140c4"));
    TRY_ONE(client.tg_issuance("0x3"));
    // TRY_ONE(client.debug_storageRangeAt("0xd3f1853788b02e31067f2c6e65cb0ae56729e23e3c92e2393af9396fa182701d", 1, "0xb734c74ff4087493373a27834074f80acbd32827", "0x00", 2));
    // TRY_ONE(client.debug_accountRange("0xaaaaa", [1], 1, true, true, true));
    // TRY_ONE(client.debug_getModifiedAccountsByNumber("0xccccd", "0xcccce"));
    // TRY_ONE(client.debug_getModifiedAccountsByHash("0x2a1af018e33bcbd5015c96a356117a5251fcccf94a9c7c8f0148e25fdee37aec", "0x4e3d3e7eee350df0ee6e94a44471ee2d22cfb174db89bbf8e6c5f6aef7b360c5"));
    // TRY_ONE(client.debug_traceTransaction("0x893c428fed019404f704cf4d9be977ed9ca01050ed93dccdd6c169422155586f"));
    // TRY_ONE(client.eth_accounts());
    // TRY_ONE(client.eth_getCompilers());
    // TRY_ONE(client.eth_compileLLL("(returnlll(suicide(caller)))"));
    // TRY_ONE(client.eth_compileSolidity("contracttest{functionmultiply(uinta)returns(uintd){returna*7;}}"));
    // TRY_ONE(client.eth_compileSerpent("/*someserpent*/"));
    // TRY_ONE(client.eth_sign("0x9b2055d370f73ec7d8a03e965129118dc8f5bf83", "0xdeadbeef"));
    // TRY_ONE(client.db_getString("testDB", "myKey"));
    // TRY_ONE(client.db_putString("testDB", "myKey", "myString"));
    // TRY_ONE(client.db_getHex("testDB", "myKey"));
    // TRY_ONE(client.db_putHex("testDB", "myKey", "0x68656c6c6f20776f726c64"));
    // TRY_ONE(client.shh_post({"from":"0xc931d93e97ab07fe42d923478ba2465f2..", "topics":["0x68656c6c6f20776f726c64"], "payload":"0x68656c6c6f20776f726c64", "ttl":"0x64", "priority":"0x64"}));
    // TRY_ONE(client.shh_version());
    // TRY_ONE(client.shh_newIdentity());
    // TRY_ONE(client.shh_hasIdentity("0x04f96a5e25610293e42a73908e93ccc8c4d4dc0edcfa9fa872f50cb214e08ebf61a03e245533f97284d442460f2998cd41858798ddfd4d661997d3940272b717b1"));
    // TRY_ONE(client.shh_newGroup());
    // TRY_ONE(client.shh_addToGroup("0x04f96a5e25610293e42a73908e93ccc8c4d4dc0edcfa9fa872f50cb214e08ebf61a03e245533f97284d442460f2998cd41858798ddfd4d661997d3940272b717b1"));
    // TRY_ONE(client.shh_newFilter({"topics":["0x12341234bf4b564f"], "to":"0x2341234bf4b2341234bf4b564f..."}));
    // TRY_ONE(client.shh_uninstallFilter("0x7"));
    // TRY_ONE(client.shh_getFilterChanges("0x7"));
    // TRY_ONE(client.shh_getMessages("0x7"));
}
