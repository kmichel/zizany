#pragma once

namespace zizany {
    class json_writer;

    class unity_type;

    class unity_value {
    public:
        const unity_type &type;
        unity_value *parent;

        unity_value(const unity_type &type_, unity_value *parent_);

        unity_value(const unity_value &) = delete;

        unity_value &operator=(const unity_value &) = delete;

        virtual ~unity_value();

        virtual void print(json_writer &writer) const = 0;
    };
}
