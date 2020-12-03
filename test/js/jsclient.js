/**
 * This file is generated by jsonrpcstub, DO NOT CHANGE IT MANUALLY!
 */
function JSClient(url) {
    this.url = url;
    var id = 1;
    
    function doJsonRpcRequest(method, params, methodCall, callback_success, callback_error) {
        var request = {};
        if (methodCall)
            request.id = id++;
        request.jsonrpc = "2.0";
        request.method = method;
        if (params !== null) {
            request.params = params;
        }
        JSON.stringify(request);
        
        $.ajax({
            type: "POST",
            url: url,
            data: JSON.stringify(request),
            success: function (response) {
                if (methodCall) {
                    if (response.hasOwnProperty("result") && response.hasOwnProperty("id")) {
                        callback_success(response.id, response.result);
                    } else if (response.hasOwnProperty("error")) {
                        if (callback_error != null)
                            callback_error(response.error.code,response.error.message);
                    } else {
                        if (callback_error != null)
                            callback_error(-32001, "Invalid Server response: " + response);
                    }
                }
            },
            error: function () {
                if (methodCall)
                    callback_error(-32002, "AJAX Error");
            },
            dataType: "json"
        });
        return id-1;
    }
    this.doRPC = function(method, params, methodCall, callback_success, callback_error) {
        return doJsonRpcRequest(method, params, methodCall, callback_success, callback_error);
    }
}

JSClient.prototype.web3_clientVersion = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("web3_clientVersion", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.web3_sha3 = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("web3_sha3", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.net_listening = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("net_listening", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.net_version = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("net_version", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.net_peerCount = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("net_peerCount", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getBlockByNumber = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("eth_getBlockByNumber", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getBlockByHash = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("eth_getBlockByHash", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getBlockTransactionCountByNumber = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_getBlockTransactionCountByNumber", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getBlockTransactionCountByHash = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_getBlockTransactionCountByHash", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getTransactionByHash = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_getTransactionByHash", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getTransactionByBlockHashAndIndex = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("eth_getTransactionByBlockHashAndIndex", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getTransactionByBlockNumberAndIndex = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("eth_getTransactionByBlockNumberAndIndex", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getTransactionReceipt = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_getTransactionReceipt", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getUncleByBlockNumberAndIndex = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("eth_getUncleByBlockNumberAndIndex", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getUncleByBlockHashAndIndex = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("eth_getUncleByBlockHashAndIndex", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getUncleCountByBlockNumber = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_getUncleCountByBlockNumber", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getUncleCountByBlockHash = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_getUncleCountByBlockHash", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_newPendingTransactionFilter = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("eth_newPendingTransactionFilter", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_newBlockFilter = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("eth_newBlockFilter", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_newFilter = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_newFilter", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_uninstallFilter = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_uninstallFilter", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getFilterChanges = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_getFilterChanges", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getLogs = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_getLogs", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getBalance = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("eth_getBalance", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getTransactionCount = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("eth_getTransactionCount", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getCode = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("eth_getCode", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getStorageAt = function(param01, param02, param03, callbackSuccess, callbackError) {
    var params = [param01, param02, param03];
    return this.doRPC("eth_getStorageAt", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_blockNumber = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("eth_blockNumber", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_syncing = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("eth_syncing", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_chainId = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("eth_chainId", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_protocolVersion = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("eth_protocolVersion", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_gasPrice = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("eth_gasPrice", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_call = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("eth_call", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_estimateGas = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_estimateGas", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_sendTransaction = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_sendTransaction", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_sendRawTransaction = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_sendRawTransaction", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getProof = function(param01, param02, param03, callbackSuccess, callbackError) {
    var params = [param01, param02, param03];
    return this.doRPC("eth_getProof", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_coinbase = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("eth_coinbase", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_hashrate = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("eth_hashrate", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_mining = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("eth_mining", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getWork = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("eth_getWork", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_submitWork = function(param01, param02, param03, callbackSuccess, callbackError) {
    var params = [param01, param02, param03];
    return this.doRPC("eth_submitWork", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_submitHashrate = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("eth_submitHashrate", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.trace_call = function(param01, param02, param03, callbackSuccess, callbackError) {
    var params = [param01, param02, param03];
    return this.doRPC("trace_call", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.trace_callMany = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("trace_callMany", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.trace_rawTransaction = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("trace_rawTransaction", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.trace_replayBlockTransactions = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("trace_replayBlockTransactions", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.trace_replayTransaction = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("trace_replayTransaction", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.trace_transaction = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("trace_transaction", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.trace_get = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("trace_get", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.trace_block = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("trace_block", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.trace_filter = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("trace_filter", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.tg_forks = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("tg_forks", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.tg_getHeaderByNumber = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("tg_getHeaderByNumber", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.tg_getHeaderByHash = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("tg_getHeaderByHash", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.tg_getLogsByHash = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("tg_getLogsByHash", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.tg_issuance = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("tg_issuance", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.debug_storageRangeAt = function(param01, param02, param03, param04, param05, callbackSuccess, callbackError) {
    var params = [param01, param02, param03, param04, param05];
    return this.doRPC("debug_storageRangeAt", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.debug_accountRange = function(param01, param02, param03, param04, param05, param06, callbackSuccess, callbackError) {
    var params = [param01, param02, param03, param04, param05, param06];
    return this.doRPC("debug_accountRange", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.debug_getModifiedAccountsByNumber = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("debug_getModifiedAccountsByNumber", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.debug_getModifiedAccountsByHash = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("debug_getModifiedAccountsByHash", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.debug_traceTransaction = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("debug_traceTransaction", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_accounts = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("eth_accounts", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_getCompilers = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("eth_getCompilers", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_compileLLL = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_compileLLL", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_compileSolidity = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_compileSolidity", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_compileSerpent = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("eth_compileSerpent", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.eth_sign = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("eth_sign", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.db_getString = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("db_getString", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.db_putString = function(param01, param02, param03, callbackSuccess, callbackError) {
    var params = [param01, param02, param03];
    return this.doRPC("db_putString", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.db_getHex = function(param01, param02, callbackSuccess, callbackError) {
    var params = [param01, param02];
    return this.doRPC("db_getHex", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.db_putHex = function(param01, param02, param03, callbackSuccess, callbackError) {
    var params = [param01, param02, param03];
    return this.doRPC("db_putHex", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.shh_post = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("shh_post", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.shh_version = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("shh_version", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.shh_newIdentity = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("shh_newIdentity", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.shh_hasIdentity = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("shh_hasIdentity", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.shh_newGroup = function(callbackSuccess, callbackError) {
    var params = null;
    return this.doRPC("shh_newGroup", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.shh_addToGroup = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("shh_addToGroup", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.shh_newFilter = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("shh_newFilter", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.shh_uninstallFilter = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("shh_uninstallFilter", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.shh_getFilterChanges = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("shh_getFilterChanges", params, true, callbackSuccess, callbackError);
};
JSClient.prototype.shh_getMessages = function(param01, callbackSuccess, callbackError) {
    var params = [param01];
    return this.doRPC("shh_getMessages", params, true, callbackSuccess, callbackError);
};