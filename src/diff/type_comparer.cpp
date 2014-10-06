#include "type_comparer.hpp"
#include "../delta_store.hpp"
#include "../unity_type.hpp"

namespace zizany {
    void
    compare_types(
            const unity_type &base,
            const unity_type &other,
            delta_store &store) {
        bool types_differ = false;
        if (base.name != other.name)
            types_differ = true;
        if (base.magic_int_1 != other.magic_int_1)
            types_differ = true;
        if (base.magic_bitset_2 != other.magic_bitset_2)
            types_differ = true;
        if (base.is_simple() && other.is_simple() && base.size != other.size)
            types_differ = true;
        if (base.is_simple() != other.is_simple())
            types_differ = true;

        if (types_differ) {
            store.modify_type(base, other);
        } else {
            // members removed and added could be checked for equality
            // in order to merge pairs of delta into a member renamed delta
            if (!base.is_simple() && !other.is_simple()) {
                for (const unity_type_member &base_member : base.members) {
                    bool match_found = false;
                    for (const unity_type_member &other_member : other.members) {
                        if (base_member.name == other_member.name) {
                            store.enter_member(base_member.name);
                            compare_types(base_member.type, other_member.type, store);
                            store.leave_member();
                            match_found = true;
                        }
                    }
                    if (!match_found)
                        store.add_type_member(base_member.name, base_member.type);
                }
                for (const unity_type_member &other_member : other.members) {
                    bool match_found = false;
                    for (const unity_type_member &base_member : base.members) {
                        if (base_member.name == other_member.name) {
                            match_found = true;
                            break;
                        }
                    }
                    if (!match_found)
                        store.remove_type_member(other_member.name, other_member.type);
                }
            }
        }
    }

}
