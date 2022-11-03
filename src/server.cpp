#include "../include/server.hpp"

namespace CHATTING {

void ChatServer::handleClient(std::shared_ptr<YXTWebCpp::Socket> sock) {
    std::string buf;
    buf.resize(1024);
    int ret = sock->recv(&buf[0], buf.size(), 0);
    if (ret > 0) {
        
    }
}

}