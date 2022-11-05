#ifndef _ONLINENAMETABLE_HPP_
#define _ONLINENAMETABLE_HPP_

#include <yxtwebcpp/yxtwebcpp.hpp>
#include <unordered_map>
#include <string>
#include <memory>

namespace CHATTING {

class OnlineNameTable {
public:
    OnlineNameTable();
    std::shared_ptr<YXTWebCpp::Socket> isOnline(std::string name);
    void Online(std::string name, std::shared_ptr<YXTWebCpp::Socket> sock);
    void offOnline(std::string name);
    std::unordered_map<std::string, std::shared_ptr<YXTWebCpp::Socket> > getNameTable() { return m_nameTable; }

private:
    std::unordered_map<std::string, std::shared_ptr<YXTWebCpp::Socket> > m_nameTable;
};

}

#endif