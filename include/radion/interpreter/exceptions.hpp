#include <string>

class RuntimeException {
public:
    RuntimeException(std::string message);
    void print();
private:
    std::string message;
};
