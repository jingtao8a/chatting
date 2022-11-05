#ifndef _MESSAGEPROTOCOL_HPP_
#define _MESSAGEPROTOCOL_HPP_

#include <string>
#include <memory>

namespace CHATTING {

class MessageType {
public:
    enum MessageTypeLabel {
        UNKNOWNMESSAGE,
        ONLINEMESSAGE,//上线
        OFFLINEMESSAGE,//下线
        PUBLICMESSAGE,//公聊消息
        PRIVATEMESSAGE,//私聊消息
        SYSTEMMESSAGE,//系统消息
    };

    static MessageTypeLabel stringToMessageTypeLabel(std::string str);

    static std::string messageTypeLabelToString(MessageTypeLabel label);
};

class MessageBody {
public:
    MessageBody() {}
    MessageBody(MessageType::MessageTypeLabel label, std::string localUser, std::string remoteUser, std::string content);
    
    void setMessageTypeLabel(MessageType::MessageTypeLabel label) {m_messageTypeLabel = label; }
    MessageType::MessageTypeLabel getMessageTypeLabel() { return m_messageTypeLabel; }

    void setLocalUser(std::string localUser) { m_localUser = localUser; }
    std::string getLocalUser() { return m_localUser; }

    void setRemoteUser(std::string remoteUser) { m_remoteUser = remoteUser; }
    std::string getRemoteUser() { return m_remoteUser; }

    void setContent(std::string content) { m_content = content; }
    std::string getContent() { return m_content; }

    bool isWrong();
    
    std::string toString();
public:
    static std::shared_ptr<MessageBody> fromString(std::string messagebody);
private:
    MessageType::MessageTypeLabel m_messageTypeLabel;
    std::string m_localUser;
    std::string m_remoteUser;
    std::string m_content;
};

//解决粘包问题
struct MessageBox {
    void fromString(std::string str);
    std::string toString();

    char m_buf[1020];
    int m_size;
};

}

#endif