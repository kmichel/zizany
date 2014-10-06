#include "delta.hpp"

#include "../diff/type_identity.hpp"
#include "../unity_type.hpp"

namespace zizany {

    class type_added_delta : public delta {
    public:
        type_identity identity;
        unity_type type;

        type_added_delta(const type_identity &identity_, const unity_type &type_);

        virtual void print_details(json_writer &writer) const;
    };
}
