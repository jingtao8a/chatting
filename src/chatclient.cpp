#include "../include/chatclient.hpp"
#include <unistd.h>
#include <signal.h>
extern void sighandler(int sum);
namespace CHATTING {

static std::shared_ptr<YXTWebCpp::Logger> g_logger = YXTWebCpp_LOG_NAME("system");

ChatClient::ChatClient() {
    m_remoteAddr = YXTWebCpp::IPAddress::Create("127.0.0.1", 7766); 
    m_sock.reset(new YXTWebCpp::Socket(AF_INET, SOCK_STREAM, 0));
    m_localUser = "unknown";
    m_recvBox.reset(new MessageBox);
    m_sendBox.reset(new MessageBox);
}

void ChatClient::recvFun() {
    signal(SIGINT, sighandler);
    std::string buf;
    while (true) {
        int ret = m_sock->recv(m_recvBox->m_buf, 1024, 0);
        if (ret <= 0) {
            YXTWebCpp_LOG_DEBUG(g_logger) << "server close";
            exit(0);//直接关闭进程
        }
        buf = m_recvBox->toString();
        std::shared_ptr<CHATTING::MessageBody> body = CHATTING::MessageBody::fromString(buf);
        switch(body->getMessageTypeLabel()) {
            case CHATTING::MessageType::MessageTypeLabel::SYSTEMMESSAGE:
            case CHATTING::MessageType::MessageTypeLabel::PUBLICMESSAGE:
                YXTWebCpp_LOG_INFO(g_logger) << buf;
                break;
            case CHATTING::MessageType::MessageTypeLabel::PRIVATEMESSAGE:
                YXTWebCpp_LOG_INFO(g_logger) <<  "[PRIVATEMessage]" << body->getLocalUser() << " quietly speak to you:" << body->getContent();
                break;
            
        }
    }
}

void ChatClient::sendFun() {
    signal(SIGINT, sighandler);
    std::string buf;
    while (true) {
        std::shared_ptr<CHATTING::MessageBody> body(new CHATTING::MessageBody(CHATTING::MessageType::MessageTypeLabel::PUBLICMESSAGE, m_localUser, "", ""));
        YXTWebCpp_LOG_INFO(g_logger) << ">>>";
        char buffer[1024];
        int ret = read(0, buffer, sizeof(buffer));
        buf.resize(ret - 1);
        memcpy(&buf[0], buffer, ret - 1);//不拷贝'\n'
        int pos = buf.find('@');
        if (pos != std::string::npos) {
            body->setMessageTypeLabel(CHATTING::MessageType::MessageTypeLabel::PRIVATEMESSAGE);
            body->setRemoteUser(buf.substr(pos + 1));
            buf = buf.substr(0, pos);
        }
        body->setContent(buf);
        buf = body->toString();
        m_sendBox->fromString(buf);
        m_sock->send(m_sendBox->m_buf, 1024, 0);
    }
}

void ChatClient::OnlineFun() {
    signal(SIGINT, sighandler);
    std::string buf;
    bool ret = m_sock->connect(m_remoteAddr);
    if (!ret) {
        YXTWebCpp_LOG_ERROR(g_logger) << "server refuse your connection!!!";
        return;
    }
    std::shared_ptr<CHATTING::MessageBody> body(new CHATTING::MessageBody(CHATTING::MessageType::MessageTypeLabel::ONLINEMESSAGE, m_localUser, "", ""));
    buf = body->toString();
    m_sendBox->fromString(buf);
    m_sock->send(m_sendBox->m_buf, 1024, 0);

    m_sock->recv(m_recvBox->m_buf, 1024, 0);
    buf = m_recvBox->toString();
    body = CHATTING::MessageBody::fromString(buf);
    if (body->getContent() == "success") {//上线成功
        YXTWebCpp_LOG_INFO(g_logger) << buf;
        auto iom = YXTWebCpp::IOManager::GetThis();
        m_iom->schedule(std::bind(&ChatClient::recvFun, shared_from_this()));
        m_iom->schedule(std::bind(&ChatClient::sendFun, shared_from_this()));
        return;
    }
    //上线失败
    YXTWebCpp_LOG_ERROR(g_logger) << buf;
}

void ChatClient::offOnlineFun() {
    std::shared_ptr<CHATTING::MessageBody> body(new CHATTING::MessageBody(CHATTING::MessageType::MessageTypeLabel::OFFLINEMESSAGE, m_localUser, "", ""));
    std::string buf = body->toString();
    m_sendBox->fromString(buf);
    m_sock->send(m_sendBox->m_buf, 1024, 0);
    exit(0);//关闭进程
}

void ChatClient::end() {
    m_iom->schedule(std::bind(&ChatClient::offOnlineFun, shared_from_this()));
}

void ChatClient::start() {
    signal(SIGINT, sighandler);
    m_iom->schedule(std::bind(&ChatClient::OnlineFun, shared_from_this()));
}

}