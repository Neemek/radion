#include <any>
#include <string>

class RuntimeException {
public:
    RuntimeException(std::string message);
    //virtual ~RuntimeException();
    void print();
private:
    std::string message;
};

class Returning {
public:
    Returning(std::any value);
    std::any value;
};
