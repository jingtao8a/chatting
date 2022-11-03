#include <yxtwebcpp/yxtwebcpp.hpp>

namespace CHATTING {

class ChatServer : public YXTWebCpp::TCPServer {

protected:
    void handleClient(std::shared_ptr<YXTWebCpp::Socket> sock) override;
private:
    
};

}