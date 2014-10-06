#include "unity_composite_value.hpp"

#include "../delta_store.hpp"
#include "../reverse_delta_store.hpp"

namespace zizany {
    unity_composite_value::unity_composite_value()
            : unity_value(), members() {
    }

    void
    unity_composite_value::print(json_writer &writer) const {
        members.print(writer);
    }

    bool
    unity_composite_value::equals(const unity_value &value) const {
        for (const unity_value_member &member : members)
            if (!value.member_equals(member.name, *member.value))
                return false;
        return true;
    }

    bool
    unity_composite_value::member_equals(const std::string &member_name, const unity_value &value) const {
        // Because we iterate on both side of the double dispatch, we have an O(n^2) hidden here.
        // We shouldn't expert large composite values though.
        const unity_value *member_value(members.get_by_name(member_name));
        if (member_value != nullptr)
            return member_value->equals(value);
        return false;
    }

    void
    unity_composite_value::compare(const unity_value &value, delta_store &store) const {
        reverse_delta_store reverse_store(store);
        value.compare(*this, reverse_store);
    }

    void
    unity_composite_value::compare(const unity_composite_value &value, delta_store &store) const {
        for (const unity_value_member &member : members) {
            const unity_value *other_member(value.members.get_by_name(member.name));
            if (other_member == nullptr)
                store.add_value_member(member.name, *member.value);
            else {
                store.enter_member(member.name);
                member.value->compare(*other_member, store);
                store.leave_member();
            }
        }
        for (const unity_value_member &member : value.members)
            if (members.get_by_name(member.name) == nullptr)
                store.remove_value_member(member.name, *member.value);
    }
}
