#pragma once

#include <string>

namespace zizany {
    class json_writer;

    class delta {
    public:
        std::string action;

        delta(const std::string &action_);

        virtual ~delta();

        void print(json_writer &writer) const;

        virtual void print_details(json_writer &writer) const = 0;
    };
}
