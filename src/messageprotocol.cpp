#include "../include/messageprotocol.hpp"
#include "../include/string.h"

namespace CHATTING {

MessageType::MessageTypeLabel MessageType::stringToMessageTypeLabel(std::string str) {
#define XX(str, labelname)\
        if (str ==  #labelname) {\
            return labelname;\
        }
    XX(str, ONLINEMESSAGE);
    XX(str, OFFLINEMESSAGE);
    XX(str, PUBLICMESSAGE);
    XX(str, PRIVATEMESSAGE);
    XX(str, SYSTEMMESSAGE);
    return UNKNOWNMESSAGE;
#undef XX
}
    
std::string MessageType::messageTypeLabelToString(MessageTypeLabel label) {
#define XX(labelname)\
        case labelname:\
            return #labelname;

    switch(label) {
        XX(ONLINEMESSAGE);
        XX(OFFLINEMESSAGE);
        XX(PUBLICMESSAGE);
        XX(PRIVATEMESSAGE);
        XX(SYSTEMMESSAGE);
        default:
            return "unknown";
    }

#undef XX
}


MessageBody::MessageBody(MessageType::MessageTypeLabel label, std::string localUser, std::string remoteUser, std::string content)
            : m_messageTypeLabel(label), m_localUser(localUser), m_remoteUser(remoteUser), m_content(content) {}

bool MessageBody::isWrong() {
    switch(m_messageTypeLabel) {
        case MessageType::MessageTypeLabel::ONLINEMESSAGE:
        case MessageType::MessageTypeLabel::OFFLINEMESSAGE:
        case MessageType::MessageTypeLabel::PUBLICMESSAGE:
            if (m_localUser.empty()) {
                return true;
            }
            break;
        case MessageType::MessageTypeLabel::PRIVATEMESSAGE:
            if (m_localUser.empty() || m_remoteUser.empty()) {
                return true;
            }
            break;
    }
    return false;
}

std::string MessageBody::toString() {
    std::string message;
    if (isWrong()) {
        return message;
    }
    message.append("[" + MessageType::messageTypeLabelToString(m_messageTypeLabel) + "]");
    message.append(m_localUser + ":");
    message.append(m_content);
    if (!m_remoteUser.empty()) {
        message.append("@");
    }
    message.append(m_remoteUser);
    return message;
}

std::shared_ptr<MessageBody> MessageBody::fromString(std::string messagebody) {
    std::shared_ptr<MessageBody> body = std::make_shared<MessageBody>();
    int n = 0;
    std::string str;
    if (messagebody[n] != '[') {
        return nullptr;
    }
    for (int i = n + 1; i < messagebody.size(); ++i) {
        if (messagebody[i] == ']') {
            str = messagebody.substr(n + 1, i - n - 1);
            n = i + 1;
            break;
        }
    }
    if (str.empty()) {
        return nullptr;
    }
    MessageType::MessageTypeLabel label = MessageType::stringToMessageTypeLabel(str);
    if (label == MessageType::MessageTypeLabel::UNKNOWNMESSAGE) {
        return nullptr;
    }
    body->setMessageTypeLabel(label);
    str = "";
    for (int i = n ; i < messagebody.size(); ++i) {
        if (messagebody[i] == ':') {
            str = messagebody.substr(n, i - n);
            n = i + 1;
            break;
        }
    }
    if (str.empty()) {
        return nullptr;
    }
    body->setLocalUser(str);
    str = "";
    if (body->getMessageTypeLabel() != MessageType::MessageTypeLabel::PRIVATEMESSAGE) {
        str = messagebody.substr(n);
        body->setContent(str);
        return body;
    }

    for (int i = n; i < messagebody.size(); ++i) {
        if (messagebody[i] == '@') {
            str = messagebody.substr(n, i - n);
            n = i + 1;
            break;
        }
    }
    if (str.empty()) {
        return nullptr;
    }
    body->setContent(str);
    body->setRemoteUser(messagebody.substr(n));
    return body;
}

void MessageBox::fromString(std::string str) {
    memcpy(m_buf, &str[0], str.size());
    m_size = str.size();
}

std::string MessageBox::toString() {
    std::string str;
    str.resize(m_size);
    memcpy(&str[0], m_buf, m_size);
    return str;
}
}