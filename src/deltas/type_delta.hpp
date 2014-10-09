#pragma once

#include "delta.hpp"
#include "../diff/member_path.hpp"
#include "../diff/type_identity.hpp"

namespace zizany {
    class unity_type;

    class type_delta : public delta {

    public:
        type_identity identity;
        member_path path;
        const unity_type &old_definition;
        const unity_type &new_definition;

        type_delta(type_identity identity_, const member_path &path_, const unity_type &old_definition_, const unity_type &new_definition_);

        virtual void print_details(json_writer &writer) const;
    };
}
