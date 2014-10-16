#include "unity_preview.hpp"

#include "json_writer.hpp"

namespace zizany {
    unity_preview::unity_preview()
            : asset_id(),
              magic_int_1(),
              magic_int_2(),
              magic_int_3(),
              data(),
              file_layout() {
    }

    void
    unity_preview::print(json_writer &writer, const bool print_layout, const bool print_magic) const {
        writer.start_object();
        writer.add_key("asset_id");
        writer.add_number(asset_id);
        if (print_layout) {
            writer.add_key("offset");
            writer.add_number(file_layout.offset);
            writer.add_key("size");
            writer.add_number(file_layout.size);
        }
        if (print_magic) {
            writer.add_key("magic_int_1");
            writer.add_number(magic_int_1);
            writer.add_key("magic_int_2");
            writer.add_number(magic_int_2);
            writer.add_key("magic_int_3");
            writer.add_number(magic_int_3);
        }
        writer.add_key("data");
        writer.start_array(true);
        for (const unsigned char byte : data)
            writer.add_number(byte);
        writer.end_array();
        writer.end_object();
    }

}
