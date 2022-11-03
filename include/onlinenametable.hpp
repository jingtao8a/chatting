#include <unordered_set>
#include <string>

namespace CHATTING {

class OnlineNameTable {
public:
    bool isOnline(std::string name);
    void goOnline(std::string name);
    void offOnline(std::string name);

private:
    std::unordered_set<std::string> m_nameTable;
};

}