#include <yxtwebcpp/yxtwebcpp.hpp>
#include "messageprotocol.hpp"

namespace CHATTING {
class ChatClient : public std::enable_shared_from_this<ChatClient>{
public:
    ChatClient();
    void start();
    void end();
    void setLocalUser(std::string name) { m_localUser = name; }
    void setIOManager(YXTWebCpp::IOManager *iom) { m_iom = iom; }  

private:
    void recvFun();
    void sendFun();
    void OnlineFun();
    void offOnlineFun();
private:
    std::shared_ptr<YXTWebCpp::Socket> m_sock;
    std::shared_ptr<YXTWebCpp::IPAddress> m_remoteAddr;
    std::shared_ptr<MessageBox> m_recvBox;
    std::shared_ptr<MessageBox> m_sendBox;
    std::string m_localUser;
    YXTWebCpp::IOManager *m_iom;
};

}