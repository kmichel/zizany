#include "delta.hpp"
#include "../json_writer.hpp"

namespace zizany {

    delta::delta() {
    }

    delta::~delta() {
    }

    void
    delta::print(json_writer &writer) const {
        writer.start_object();
        writer.add_key("action");
        print_action(writer);
        writer.add_key("details");
        print_details(writer);
        writer.end_object();
    }
}
