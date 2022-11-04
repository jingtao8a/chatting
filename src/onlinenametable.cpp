#include "../include/onlinenametable.hpp"

namespace CHATTING {

OnlineNameTable::OnlineNameTable() {}

std::shared_ptr<YXTWebCpp::Socket> OnlineNameTable::isOnline(std::string name) {
    auto res = m_nameTable.find(name);
    if (res == m_nameTable.end()) {
        return nullptr;
    }
    return res->second;
}

void OnlineNameTable::Online(std::string name, std::shared_ptr<YXTWebCpp::Socket> sock) {
    m_nameTable[name] = sock;
}

void OnlineNameTable::offOnline(std::string name) {
    m_nameTable.erase(name);
}

}