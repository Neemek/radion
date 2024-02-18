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

    std::string line;

    int linepos = 0;

    int i;
    for (i = 0; i < this->error_causer->start; ++i) {
        char c = src.at(i);

        if (c == '\n') {
            linepos = i;
        }
    }

    while (src.at(i) != '\n' && src.size() < i) i++;

    line = src.substr(linepos, i);

    std::string position_descriptor = get_position_descriptor(src, this->error_causer->start);

    std::cout << position_descriptor << "\t " << line << std::endl;
    std::cout << "\t ";

    for (int j = 0; j < this->error_causer->start - linepos; ++j) std::cout << " ";
    for (int j = 0; j < this->error_causer->end - this->error_causer->end; ++j) std::cout << "^";

    std::cout << std::endl;
}
