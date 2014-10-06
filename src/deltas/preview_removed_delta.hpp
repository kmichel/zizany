#pragma once

#include "delta.hpp"
#include "../unity_preview.hpp"

namespace zizany {
    class preview_removed_delta : public delta {
    public:
        unity_preview preview;

        preview_removed_delta(const unity_preview &preview_);

        virtual void print_details(json_writer &writer) const;
    };
}
