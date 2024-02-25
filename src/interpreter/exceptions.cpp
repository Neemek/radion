#include <iostream>
#include <utility>

#include "radion/interpreter/exceptions.hpp"
#include "radion/parser/parser.hpp"

RuntimeException::RuntimeException(std::string message) {
    this->message = std::move(message);
    this->error_causer = nullptr;
}

RuntimeException::RuntimeException(std::string message, Node *error_causer) {
    this->message = std::move(message);
    this->error_causer = error_causer;
}

const char *RuntimeException::what() const noexcept {
    return this->message.c_str();
}

void RuntimeException::print(std::string src) {
    std::cout << "\u001b[31m" << "RuntimeError: " << this->what() << "\u001b[0m" << std::endl;

    if (this->error_causer == nullptr) return;

    std::string line;

    int linepos = 0;

    int i;
    for (i = 0; i < this->error_causer->start; ++i) {
        char c = src.at(i);

        if (c == '\n') {
            linepos = i;
        }
    }

    while (i < src.size() && src.at(i) != '\n') i++;

    line = src.substr(linepos, i-linepos);

    std::string position_descriptor = get_position_descriptor(src, this->error_causer->start);

    std::cout << position_descriptor << " " << line << std::endl;

    for (int j = 0; j < this->error_causer->start - linepos + position_descriptor.size() + 1; ++j) std::cout << " ";
    for (int j = 0; j < this->error_causer->end - this->error_causer->start; ++j) std::cout << "^";

    std::cout << std::endl;
}
