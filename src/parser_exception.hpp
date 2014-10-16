#pragma once

#include <stdexcept>
#include <string>

namespace zizany {
    class parser_exception : public std::runtime_error {
    public:
        explicit parser_exception(const std::string &what_);

        explicit parser_exception(const char *const what_);

        virtual const char *what() const noexcept;
    };
}
