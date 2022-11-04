#include "../include/chatserver.hpp"

static std::shared_ptr<YXTWebCpp::Logger> g_logger = YXTWebCpp_LOG_ROOT();

void server() {
    std::shared_ptr<CHATTING::ChatServer> server(new CHATTING::ChatServer);
    std::shared_ptr<YXTWebCpp::Address> addr = YXTWebCpp::IPAddress::Create("127.0.0.1", 7766);
    while (!server->bind(addr) ) {
        YXTWebCpp_LOG_ERROR(g_logger) << "bind again";
    }
    server->setName("chatServer");
    server->setRecvTimeout(-1);
    server->start();
}

int main() {
    std::shared_ptr<YXTWebCpp::IOManager> iom(new YXTWebCpp::IOManager(1, true, "chat server main"));
    iom->schedule(server);
}
