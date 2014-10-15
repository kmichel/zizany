#include "unity_value_members.hpp"

#include "json_writer.hpp"
#include "unity_value.hpp"

namespace zizany {
    unity_value_members::unity_value_members()
            : members() {
    }

    void
    unity_value_members::reserve(const std::size_t size) {
        members.reserve(size);
    }

    void
    unity_value_members::add(const std::string &member_name, std::unique_ptr<unity_value> &&value) {
        members.emplace_back(member_name, std::move(value));
    }

    const unity_value_member &
    unity_value_members::at(const std::size_t index) const {
        return members.at(index);
    }

    const unity_value *
    unity_value_members::get_by_name(const std::string &name) const {
        for (const unity_value_member &member : members)
            if (member.name == name)
                return &*member.value;
        return nullptr;
    }

    void
    unity_value_members::print(json_writer &writer) const {
        writer.start_object();
        for (const unity_value_member &member : members) {
            writer.add_key(member.name);
            member.value->print(writer);
        }
        writer.end_object();
    }
}
