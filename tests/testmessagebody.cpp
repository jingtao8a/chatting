#include "../include/messageprotocol.hpp"
#include <iostream>

int main() {
   std::shared_ptr<CHATTING::MessageBody> body = CHATTING::MessageBody::fromString("[PRIVATEMESSAGE]BOB:hello@james");
   std::cout << body->toString();
}