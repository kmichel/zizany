#pragma once

#include "unity_value_member.hpp"

#include <string>
#include <vector>

namespace zizany {
    class json_writer;

    class unity_value;

    class unity_value_members {
        std::vector<unity_value_member> members;
    public:
        unity_value_members();

        void reserve(const std::size_t size);

        void add(const std::string &member_name, std::unique_ptr<unity_value> &&value);

        const unity_value_member &at(const std::size_t index) const;

        const unity_value *get_by_name(const std::string &name) const;

        void print(json_writer &writer) const;

        class const_iterator {
            typename std::vector<unity_value_member>::const_iterator members_iterator;
        public:
            const_iterator(const typename std::vector<unity_value_member>::const_iterator members_iterator_)
                    : members_iterator(members_iterator_) {
            }

            const unity_value_member &operator*() const {
                return *members_iterator;
            }

            bool operator!=(const const_iterator &rhs) const {
                return members_iterator != rhs.members_iterator;
            }

            const const_iterator &operator++() {
                ++members_iterator;
                return *this;
            }
        };

        const_iterator begin() const {
            return const_iterator(members.begin());
        }

        const_iterator end() const {
            return const_iterator(members.end());
        }
    };
}
