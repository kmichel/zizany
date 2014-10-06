#include "parser_exception.hpp"

namespace zizany {
    parser_exception::parser_exception(const std::string &what_)
            : runtime_error(what_) {
    }

    parser_exception::parser_exception(const char *what_)
            : runtime_error(what_) {
    }

    const char *
    parser_exception::what() const noexcept {
        return runtime_error::what();
    }
}
