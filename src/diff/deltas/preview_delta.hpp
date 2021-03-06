#pragma once

#include "../delta.hpp"
#include "../../unity_preview.hpp"

namespace zizany {
    class preview_delta : public delta {
    public:
        const unity_preview old_preview;
        const unity_preview new_preview;

        preview_delta(const unity_preview &old_preview_, const unity_preview &new_preview_);

        virtual void print_action(json_writer &writer) const;

        virtual void print_details(json_writer &writer) const;
    };
}
