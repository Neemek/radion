#include <any>
#include <string>
#include "radion/parser/node.hpp"

class RuntimeException {
public:
    RuntimeException(std::string message, Node *error_causer);
    //virtual ~RuntimeException();
    void print(std::string src);
private:
    std::string message;
    Node *error_causer;
};
