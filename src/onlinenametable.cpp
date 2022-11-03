#include "../include/onlinenametable.hpp"

namespace CHATTING {

bool OnlineNameTable::isOnline(std::string name) {
    auto res = m_nameTable.find(name);
    if (res == m_nameTable.end()) {
        return false;
    }
    return true;
}

void OnlineNameTable::goOnline(std::string name) {
    m_nameTable.insert(name);
}

void OnlineNameTable::offOnline(std::string name) {
    m_nameTable.erase(name);
}

}