#pragma once

#include "../json_writer.hpp"
#include "../unity_value.hpp"

#include <climits>

namespace zizany {
    template<typename value_type>
    class unity_integer_value : public unity_value {
    public:
        value_type value;

        unity_integer_value(value_type value_)
                : unity_value(), value(value_) {
        }

        virtual void print(json_writer &writer) const {
            writer.add_number(value);
        }

        virtual bool equals(const unity_value &value_) const {
            if (value > std::numeric_limits<std::int64_t>::max())
                return value_.equals(static_cast<std::uint64_t>(value));
            else
                return value_.equals(static_cast<std::int64_t>(value));
        }

        virtual bool equals(std::int64_t value_) const {
            if (value > std::numeric_limits<std::int64_t>::max())
                return false;
            else
                return value_ == static_cast<std::int64_t>(value);
        }

        virtual bool equals(std::uint64_t value_) const {
            if (value < 0)
                return false;
            else
                return value_ == static_cast<std::uint64_t>(value);
        }
    };
}
