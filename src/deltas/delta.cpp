#include "delta.hpp"
#include "../json_writer.hpp"

namespace zizany {

    delta::delta(const std::string &action_)
            : action(action_) {
    }

    delta::~delta() {
    }

    void delta::print(json_writer &writer) const {
        writer.start_object();
        writer.add_key("action");
        writer.add_string(action);
        writer.add_key("details");
        print_details(writer);
        writer.end_object();
    }
}
