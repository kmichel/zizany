#pragma once

#include "delta.hpp"

#include <string>

namespace zizany {
    class unity_version_delta : public delta {
    public:
        std::string old_version;
        std::string new_version;

        unity_version_delta(const std::string &old_version_, const std::string &new_version_);

        virtual void print_action(json_writer &writer) const;

        virtual void print_details(json_writer &writer) const;
    };
}
