#pragma once

#include "../delta.hpp"
#include "../delta_set_operation.hpp"
#include "../../unity_preview.hpp"

namespace zizany {
    class previews_delta : public delta {
    public:
        const delta_set_operation operation;
        const unity_preview preview;

        previews_delta(const delta_set_operation operation_, const unity_preview &preview_);

        virtual void print_action(json_writer &writer) const;

        virtual void print_details(json_writer &writer) const;
    };
}
