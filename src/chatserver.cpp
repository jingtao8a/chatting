#include "../include/chatserver.hpp"


namespace CHATTING {

static std::shared_ptr<YXTWebCpp::Logger> g_logger = YXTWebCpp_LOG_NAME("system");

ChatServer::ChatServer() {
    m_nameTable.reset(new OnlineNameTable);
}

void ChatServer::handleClient(std::shared_ptr<YXTWebCpp::Socket> sock) {
    std::string buf;
    std::shared_ptr<MessageBox> m_recvBox(new MessageBox);
    while (true) {
        int ret = sock->recv(m_recvBox->m_buf, 1024, 0);
        if (ret <= 0) {//对端关闭连接
            YXTWebCpp_LOG_DEBUG(g_logger) << "client close";
            return;
        }
        buf = m_recvBox->toString();
        std::shared_ptr<MessageBody> request = MessageBody::fromString(buf);
        if (request) {
            analyzeRequest(sock, request);
        }
    }
}

void ChatServer::analyzeRequest(std::shared_ptr<YXTWebCpp::Socket> sock, std::shared_ptr<MessageBody> request) {
    switch(request->getMessageTypeLabel()) {
        case MessageType::MessageTypeLabel::ONLINEMESSAGE:
            dealWithOnlineMessage(sock, request);
            break;
        case MessageType::MessageTypeLabel::OFFLINEMESSAGE:
            dealWithOfflineMessage(sock, request);
            break;
        case MessageType::MessageTypeLabel::PUBLICMESSAGE:
            dealtWithPublicMessage(sock, request);
            break;
        case MessageType::MessageTypeLabel::PRIVATEMESSAGE:
            dealWithPrivateMessage(sock, request);
            break;
        default:
            return;
    }
}


void ChatServer::dealWithOnlineMessage(std::shared_ptr<YXTWebCpp::Socket> sock, std::shared_ptr<MessageBody> request) {
    std::shared_ptr<MessageBody> body;
    std::string buf;
    std::shared_ptr<MessageBox> m_sendBox(new MessageBox);
    std::shared_ptr<MessageBox> m_recvBox(new MessageBox);
    if (m_nameTable->isOnline(request->getLocalUser()) ) {
        body.reset(new MessageBody(MessageType::MessageTypeLabel::SYSTEMMESSAGE,"system", "", "you are online"));
        buf = body->toString();
        m_sendBox->fromString(buf);
        sock->send(m_sendBox->m_buf, 1024, 0);//发消息给客户端，不可以重复上线
        return;
    }
    m_nameTable->Online(request->getLocalUser(), sock);//记录客户端的消息
    body.reset(new MessageBody(MessageType::MessageTypeLabel::SYSTEMMESSAGE,"system", "", "success"));
    buf = body->toString();
    m_sendBox->fromString(buf);
    sock->send(m_sendBox->m_buf, 1024, 0);//发消息给客户端，已经上线了
    //告诉所有上线的客户，某某上线了
    auto nameTable = m_nameTable->getNameTable();
    body.reset(new MessageBody(MessageType::MessageTypeLabel::SYSTEMMESSAGE,"system", "", request->getLocalUser() + " is onLine"));
    buf = body->toString();
    m_sendBox->fromString(buf);
    for (auto& i : nameTable) {
        if (i.first != request->getLocalUser()) {
            i.second->send(m_sendBox->m_buf, 1024, 0);
        }
    }
    return;
}

void ChatServer::dealWithOfflineMessage(std::shared_ptr<YXTWebCpp::Socket> sock, std::shared_ptr<MessageBody> request) {
    std::shared_ptr<MessageBody> body;
    std::string buf;
    std::shared_ptr<MessageBox> m_sendBox(new MessageBox);
    std::shared_ptr<MessageBox> m_recvBox(new MessageBox);
    m_nameTable->offOnline(request->getLocalUser());//删除某个客户端的记录，并且关闭TCP连接
    //告诉所有上线的客户，某某下线了
    auto nameTable = m_nameTable->getNameTable();
    body.reset(new MessageBody(MessageType::MessageTypeLabel::SYSTEMMESSAGE,"system", "", request->getLocalUser() + " is offLine"));
    buf = body->toString();
    m_sendBox->fromString(buf);
    for (auto& i : nameTable) {
        i.second->send(m_sendBox->m_buf, 1024, 0);
    }
    return;
}

void ChatServer::dealtWithPublicMessage(std::shared_ptr<YXTWebCpp::Socket> sock, std::shared_ptr<MessageBody> request) {
    std::string buf;
    std::shared_ptr<MessageBox> m_sendBox(new MessageBox);
    std::shared_ptr<MessageBox> m_recvBox(new MessageBox);
    //发送给所有客户的公聊消息
    auto nameTable = m_nameTable->getNameTable();
    buf = request->toString();
    m_sendBox->fromString(buf);
    for (auto& i : nameTable) {
        if (i.first != request->getLocalUser()) {
            i.second->send(m_sendBox->m_buf, 1024, 0);
        }
    }
    return;
}

void ChatServer::dealWithPrivateMessage(std::shared_ptr<YXTWebCpp::Socket> sock, std::shared_ptr<MessageBody> request) {
    std::shared_ptr<MessageBody> body;
    std::string buf;
    std::shared_ptr<MessageBox> m_sendBox(new MessageBox);
    std::shared_ptr<MessageBox> m_recvBox(new MessageBox);
    auto anotherSock = m_nameTable->isOnline(request->getRemoteUser());
    if (!anotherSock) {
        body.reset(new MessageBody(MessageType::MessageTypeLabel::SYSTEMMESSAGE,"system", "", request->getRemoteUser() + " is not onLine"));
        buf = body->toString();
        m_sendBox->fromString(buf);
        sock->send(m_sendBox->m_buf, 1024, 0);
        return;
    }
    buf = request->toString();
    m_sendBox->fromString(buf);
    anotherSock->send(m_sendBox->m_buf, 1024, 0);
    return;
}

}