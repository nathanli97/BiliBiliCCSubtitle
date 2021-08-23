//
// Created by lixingru on 8/23/2021.
//

#include "NetworkProviderBase.h"

int NetworkProviderBase::blockingFetch(const std::string &url, std::string &output, NetworkProviderMethod method,
                                       std::vector<std::pair<std::string, std::string>> override_headers,
                                       std::vector<std::pair<std::string, std::string>> override_cookies)
{
    return fetch(url,method,true,[&](std::vector<uint8_t> &vec, uint32_t startOffset, uint32_t size)->bool{
        output.append(vec.begin(), vec.begin() + size);
        return true;
        },override_headers,override_cookies);
}
