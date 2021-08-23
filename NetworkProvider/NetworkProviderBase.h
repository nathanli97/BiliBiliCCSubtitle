//
// Created by lixingru on 8/23/2021.
//

#ifndef BILIBILICCSUBTITLE_NETWORKPROVIDERBASE_H
#define BILIBILICCSUBTITLE_NETWORKPROVIDERBASE_H
#include <iostream>
#include <functional>
#include <vector>
enum NetworkProviderMethod{
    GET,
    POST,
    PUT,
    DELETE,
    HEAD
};
class NetworkProviderBase {
public:
    virtual bool initialize() = 0;
    virtual bool shutdown() = 0;

    virtual bool setupProxy(const std::string &proxyURL) = 0;
    virtual std::string getProxy() = 0;
    virtual bool clearProxy() = 0;

    virtual int fetch(
            const std::string& url,
            NetworkProviderMethod method = NetworkProviderMethod::GET,
            bool forceWait = false,
            std::function<bool(std::vector<uint8_t> &vec, uint32_t startOffset, uint32_t size)> callback = nullptr,
            std::vector<std::pair<std::string, std::string>> override_headers = std::vector<std::pair<std::string, std::string>>(),
            std::vector<std::pair<std::string, std::string>> override_cookies = std::vector<std::pair<std::string, std::string>>()
            ) = 0;

    int blockingFetch(const std::string& url,
                       std::string &output,
                       NetworkProviderMethod method = NetworkProviderMethod::GET,
                       std::vector<std::pair<std::string, std::string>> override_headers = std::vector<std::pair<std::string, std::string>>(),
                       std::vector<std::pair<std::string, std::string>> override_cookies = std::vector<std::pair<std::string, std::string>>()
                       );
};


#endif //BILIBILICCSUBTITLE_NETWORKPROVIDERBASE_H
