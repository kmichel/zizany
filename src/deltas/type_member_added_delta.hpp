#include "delta.hpp"

#include "../diff/type_identity.hpp"
#include "../diff/member_path.hpp"

namespace zizany {

    class type_member_added_delta : public delta {
    public:
        type_identity identity;
        member_path path;
        const unity_type &added_type;

        type_member_added_delta(const type_identity &identity_, const member_path &path_, const unity_type &added_type_);

        virtual void print_details(json_writer &writer) const;
    };
}
