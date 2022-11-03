#include <string>
#include <memory>

namespace CHATTING {

class MessageType {
public:
    enum MessageTypeLabel {
        UNKNOWN,
        GOONLINE,//上线
        OFFLINE,//下线
        PUBLICMESSAGE,//公聊消息
        PRIVATEMESSAGE,//私聊消息
        SYSTEM,//系统消息
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
    
    void setRemoteUser(std::string remoteUser) { m_remoteUser = remoteUser; }
    
    void setContent(std::string content) { m_content = content; }
    
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

}