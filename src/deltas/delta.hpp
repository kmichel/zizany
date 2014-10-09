#pragma once

namespace zizany {
    class json_writer;

    class delta {
    public:
        delta();

        virtual ~delta();

        void print(json_writer &writer) const;

        virtual void print_action(json_writer &action) const = 0;

        virtual void print_details(json_writer &writer) const = 0;
    };
}
