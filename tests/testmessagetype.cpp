#include "../include/messageprotocol.hpp"
#include <iostream>

int main() {
    std::cout << CHATTING::MessageType::stringToMessageTypeLabel("GOONLINE") << std::endl;
    std::cout << CHATTING::MessageType::messageTypeLabelToString(CHATTING::MessageType::MessageTypeLabel::OFFLINEMESSAGE);
    return 0;
}