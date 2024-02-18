#include <any>
#include <iostream>

#include "radion/interpreter/exceptions.hpp"
#include "radion/parser/parser.hpp"

RuntimeException::RuntimeException(std::string message, Node *error_causer) {
    this->message = message;
    this->error_causer = error_causer;
}

void RuntimeException::print(std::string src) {
    std::cout << "\u001b[31m" << "RuntimeError: " << message << "\u001b[37m" << std::endl;
    std::cout << get_position_descriptor(src, this->error_causer->start) << " " << "" << std::endl;
}
