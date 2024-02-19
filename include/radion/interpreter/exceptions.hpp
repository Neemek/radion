#include <string>
#include "radion/parser/node.hpp"

class RuntimeException : public std::exception {
public:
    explicit RuntimeException(std::string message);
    RuntimeException(std::string message, Node *error_causer);
    //virtual ~RuntimeException();
    const char* what() const noexcept override;
    void print(std::string src);
private:
    std::string message;
    Node *error_causer;
};
