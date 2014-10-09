#include "file_endianness_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    file_endianness_delta::file_endianness_delta(
            const endianness old_endianness_, const endianness new_endianness_)
            : delta(), old_endianness(old_endianness_), new_endianness(new_endianness_) {
    }

    void
    file_endianness_delta::print_action(json_writer &writer) const {
        writer.add_string("change file endianness");
    }

    void
    file_endianness_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("old_endianness");
        writer.add_string(old_endianness == endianness::big_endian ? "big endian" : "little endian");
        writer.add_key("new_endianness");
        writer.add_string(new_endianness == endianness::big_endian ? "big endian" : "little endian");
        writer.end_object();
    }
}
