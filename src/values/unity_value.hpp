#pragma once

#include "../owning_vector.hpp"

#include <cstdint>
#include <string>

namespace zizany {
    struct asset_reference;

    class delta_store;

    struct guid;

    class json_writer;

    class unity_array_value;

    class unity_composite_value;

    class unity_value {
    public:
        unity_value();

        unity_value(const unity_value &) = delete;

        unity_value &operator=(const unity_value &) = delete;

        virtual ~unity_value();

        virtual void print(json_writer &writer) const = 0;

        /* equality double dispatch */

        virtual bool equals(const unity_value &value) const = 0;

        virtual bool equals(bool value) const;

        virtual bool equals(char value) const;

        virtual bool equals(std::int64_t value) const;

        virtual bool equals(std::uint64_t value) const;

        virtual bool equals(float value) const;

        virtual bool equals(double value) const;

        virtual bool equals(const std::string &value) const;

        virtual bool equals(const guid &value) const;

        virtual bool equals(const asset_reference &value) const;

        virtual bool equals(const std::vector<char> &value) const;

        virtual bool equals(const unity_array_value &value) const;

        virtual bool equals(const unity_composite_value &value) const;

        /* delta generation double dispatch */

        virtual void compare(const unity_value &value, delta_store &store) const;

        virtual void compare(const unity_array_value &value, delta_store &store) const;

        virtual void compare(const unity_composite_value &value, delta_store &store) const;
    };
}
