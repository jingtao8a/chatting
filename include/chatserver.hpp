#ifndef _CHATSERVER_HPP_
#define _CHATSERVER_HPP_

#include "messageprotocol.hpp"
#include "onlinenametable.hpp"

namespace CHATTING {

class ChatServer : public YXTWebCpp::TCPServer {
public:
    ChatServer();
protected:
    void handleClient(std::shared_ptr<YXTWebCpp::Socket> sock) override;
private:
    void analyzeRequest(std::shared_ptr<YXTWebCpp::Socket> sock, std::shared_ptr<MessageBody> request);
    void dealWithOnlineMessage(std::shared_ptr<YXTWebCpp::Socket> sock, std::shared_ptr<MessageBody> request);
    void dealWithOfflineMessage(std::shared_ptr<YXTWebCpp::Socket> sock, std::shared_ptr<MessageBody> request);
    void dealtWithPublicMessage(std::shared_ptr<YXTWebCpp::Socket> sock, std::shared_ptr<MessageBody> request);
    void dealWithPrivateMessage(std::shared_ptr<YXTWebCpp::Socket> sock, std::shared_ptr<MessageBody> request);

private:
    std::shared_ptr<OnlineNameTable> m_nameTable;
};

}

#endif