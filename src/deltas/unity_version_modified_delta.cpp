#include "unity_version_modified_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    unity_version_modified_delta::unity_version_modified_delta(
            const std::string &old_version_, const std::string &new_version_)
            : delta("change unity version"), old_version(old_version_), new_version(new_version_) {
    }

    void
    unity_version_modified_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("old_version");
        writer.add_string(old_version);
        writer.add_key("new_version");
        writer.add_string(new_version);
        writer.end_object();
    }
}
