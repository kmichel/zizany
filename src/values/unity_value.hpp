#pragma once

namespace zizany {
    class json_writer;

    class unity_value {
    public:
        unity_value();

        unity_value(const unity_value &) = delete;

        unity_value &operator=(const unity_value &) = delete;

        virtual ~unity_value();

        virtual void print(json_writer &writer) const = 0;
    };
}
