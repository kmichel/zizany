#pragma once

#include "delta.hpp"
#include "../unity_preview.hpp"

namespace zizany {
    class preview_changed_delta : public delta {
    public:
        unity_preview old_preview;
        unity_preview new_preview;

        preview_changed_delta(const unity_preview &old_preview_, const unity_preview &new_preview_);

        virtual void print_details(json_writer &writer) const;
    };
}
