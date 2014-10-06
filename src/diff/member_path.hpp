#pragma once

#include <string>
#include <vector>

namespace zizany {
    class json_writer;

    class member_path {
        std::vector<std::string> components;
    public:
        member_path();

        void push(const std::string &component);

        void pop();

        member_path join(const std::string &component) const;

        void print(json_writer &writer) const;
    };
}
