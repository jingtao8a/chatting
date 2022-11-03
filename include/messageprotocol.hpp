#include <string>

namespace CHATTING {

class MessageType {
public:
    enum MessageTypeLabel {
        UNKNOWN,
        GOONLINE,//上线
        OFFLINE,//下线
        PUBLICMESSAGE,//公聊消息
        PRIVATEMESSAGE,//私聊消息
    };

    static MessageTypeLabel stringToMessageTypeLabel(std::string str);

    static std::string messageTypeLabelToString(MessageTypeLabel label);
};

class MessageProtocol {

public:
    

private:

};

}