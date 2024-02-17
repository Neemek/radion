#include <any>
#include <iostream>

#include "radion/interpreter/exceptions.hpp"

RuntimeException::RuntimeException(std::string message) {
    this->message = message;
}

void RuntimeException::print() {
    std::cout << "\u001b[31m" << "RuntimeError: " << message << "\u001b[37m" << std::endl;
}


Returning::Returning(std::any value) {
    this->value = value;
}
