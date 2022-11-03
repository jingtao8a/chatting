#include "../include/messageprotocol.hpp"

namespace CHATTING {

MessageType::MessageTypeLabel MessageType::stringToMessageTypeLabel(std::string str) {
#define XX(str, labelname)\
        if (str ==  #labelname) {\
            return labelname;\
        }
    XX(str, GOONLINE);
    XX(str, OFFLINE);
    XX(str, PUBLICMESSAGE);
    XX(str, PRIVATEMESSAGE);
    return UNKNOWN;
#undef XX
}
    
std::string MessageType::messageTypeLabelToString(MessageTypeLabel label) {
#define XX(labelname)\
        case labelname:\
            return #labelname;

    switch(label) {
        XX(GOONLINE);
        XX(OFFLINE);
        XX(PUBLICMESSAGE);
        XX(PRIVATEMESSAGE);
        default:
            return "unknown";
    }

#undef XX
}

}