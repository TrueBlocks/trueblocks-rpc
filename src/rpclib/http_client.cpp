/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#include <cstdlib>
#include <curl/curl.h>
#include <string.h>
#include <string>
#include <iostream>

#include <rpclib/http_client.h>

using namespace jsonrpc;

//---------------------------------------------------------------------------------------
class curl_initializer {
  public:
    curl_initializer() {
        curl_global_init(CURL_GLOBAL_ALL);
    }
    ~curl_initializer() {
        curl_global_cleanup();
    }
};

//---------------------------------------------------------------------------------------
// See here: http://curl.haxx.se/libcurl/c/curl_global_init.html
static curl_initializer _curl_init = curl_initializer();

//---------------------------------------------------------------------------------------
// http:// stackoverflow.com/questions/2329571/c-libcurl-get-output-into-a-string

//---------------------------------------------------------------------------------------
struct loc_str {
    char* ptr;
    size_t len;
};

//---------------------------------------------------------------------------------------
static size_t writefunc(void* ptr, size_t size, size_t nmemb, struct loc_str* s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = (char*)realloc(s->ptr, new_len + 1);
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    return size * nmemb;
}

//---------------------------------------------------------------------------------------
void init_string(struct loc_str* s) {
    s->len = 0;
    s->ptr = static_cast<char*>(malloc(s->len + 1));
    s->ptr[0] = '\0';
}

//---------------------------------------------------------------------------------------
HttpClient::HttpClient(const string& u) : url(u) {
    timeout = 10000;
    curl = curl_easy_init();
}

//---------------------------------------------------------------------------------------
HttpClient::~HttpClient() {
    curl_easy_cleanup(curl);
}

//---------------------------------------------------------------------------------------
void HttpClient::SendRPCMessage(const string& message, string& result) {
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);

    CURLcode res;

    struct loc_str s;
    init_string(&s);

    struct curl_slist* headersPtr = NULL;

    for (std::map<string, string>::iterator header = headers.begin(); header != headers.end(); ++header) {
        headersPtr = curl_slist_append(headersPtr, (header->first + ": " + header->second).c_str());
    }
    headersPtr = curl_slist_append(headersPtr, "Content-Type: application/json");
    headersPtr = curl_slist_append(headersPtr, "charsets: utf-8");

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headersPtr);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout);

    res = curl_easy_perform(curl);

    result = s.ptr;
    free(s.ptr);
    curl_slist_free_all(headersPtr);
    if (res != CURLE_OK) {
        stringstream str;
        str << "libcurl error: " << res;

        if (res == 7)
            str << " -> Could not connect to " << url << " (is server running?)";
        else if (res == 28)
            str << " -> Operation timed out";
        throw JsonRpcException(ERROR_CLIENT_CONNECTOR, str.str());
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code / 100 != 2) {
        throw JsonRpcException(ERROR_RPC_INTERNAL_ERROR, result);
    }
}

//---------------------------------------------------------------------------------------
void HttpClient::SetUrl(const string& u) {
    url = u;
}

//---------------------------------------------------------------------------------------
void HttpClient::SetTimeout(long t) {
    timeout = t;
}

//---------------------------------------------------------------------------------------
void HttpClient::AddHeader(const string& attr, const string& val) {
    headers[attr] = val;
}

//---------------------------------------------------------------------------------------
void HttpClient::RemoveHeader(const string& attr) {
    headers.erase(attr);
}
