#include "../include/chatclient.hpp"
#include <signal.h>

static std::shared_ptr<CHATTING::ChatClient> client(new CHATTING::ChatClient);
static std::shared_ptr<YXTWebCpp::Logger> g_logger = YXTWebCpp_LOG_ROOT();

void clientChat() {
    YXTWebCpp_LOG_INFO(g_logger) << "please input your username: ";
    std::string username;
    std::cin >> username;
    client->setLocalUser(username);
    client->setIOManager(YXTWebCpp::IOManager::GetThis());
    client->start();
}

void sighandler(int signum) {
    client->end();
}

int main() {
    std::shared_ptr<YXTWebCpp::IOManager> iom(new YXTWebCpp::IOManager(2, true, "client chat"));
    iom->schedule(clientChat);
}