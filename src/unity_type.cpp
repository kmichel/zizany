#include "unity_type.hpp"

#include "json_writer.hpp"
#include "value_parsers/array_value_parser.hpp"
#include "value_parsers/asset_reference_value_parser.hpp"
#include "value_parsers/blob_value_parser.hpp"
#include "value_parsers/bool_value_parser.hpp"
#include "value_parsers/composite_value_parser.hpp"
#include "value_parsers/double_value_parser.hpp"
#include "value_parsers/fixed_size_blob_value_parser.hpp"
#include "value_parsers/float_value_parser.hpp"
#include "value_parsers/guid_value_parser.hpp"
#include "value_parsers/inline_array_value_parser.hpp"
#include "value_parsers/integer_value_parser.hpp"
#include "value_parsers/multiline_string_value_parser.hpp"
#include "value_parsers/padded_value_parser.hpp"
#include "value_parsers/string_value_parser.hpp"
#include "value_parsers/tail_blob_value_parser.hpp"

namespace zizany {
    unity_type::unity_type()
            : members(),
              name(),
              size(),
              magic_int_1(),
              magic_bitset_2(),
              is_array(false),
              is_tail_blob(false) {
    }

    bool
    unity_type::requires_padding() const {
        return (magic_bitset_2 & 0x4000u) != 0;
    }

    void
    unity_type::set_requires_padding(const bool requires_padding) {
        if (requires_padding)
            magic_bitset_2 |= 0x4000u;
        else
            magic_bitset_2 &= ~0x4000u;
    }

    bool
    unity_type::is_simple() const {
        return members.size() == 0 && size > 0;
    }

    void
    unity_type::print(json_writer &writer, const bool print_defaults, const bool print_magic) const {
        writer.start_object();
        writer.add_key("name");
        writer.add_string(name);
        if (is_array || print_defaults) {
            writer.add_key("is_array");
            writer.add_bool(is_array);
        }
        if (size != -1 || print_defaults) {
            writer.add_key("size");
            writer.add_number(size);
        }
        if (requires_padding() || print_defaults) {
            writer.add_key("requires_padding");
            writer.add_bool(requires_padding());
        }
        if (print_magic) {
            writer.add_key("magic_int_1");
            writer.add_number(magic_int_1);
            writer.add_key("magic_bitset_2");
            writer.add_number(magic_bitset_2);
        }
        if (members.size() > 0 || print_defaults) {
            writer.add_key("members");
            writer.start_array();
            for (const unity_type_member &member : members) {
                writer.start_object();
                writer.add_key("name");
                writer.add_string(member.name);
                writer.add_key("type");
                member.type.print(writer, print_defaults, print_magic);
                writer.end_object();
            }
            writer.end_array();
        }
        writer.end_object();
    }

    std::unique_ptr<value_parser>
    unity_type::get_value_parser(const std::string *const member_name) const {
        std::unique_ptr<value_parser> ret;
        if (is_tail_blob) {
            ret.reset(new tail_blob_value_parser);
        } else if (is_array) {
            const unity_type &element_type(members.at(1).type);
            if (element_type.is_simple()) {
                if (element_type.size == 1)
                    ret.reset(new blob_value_parser);
                else
                    ret.reset(new inline_array_value_parser(element_type.get_value_parser()));
            } else
                ret.reset(new array_value_parser(element_type.get_value_parser()));
        } else if (is_simple()) {
            if (name == "bool")
                ret.reset(new bool_value_parser);
            else if (name == "char")
                ret.reset(new integer_value_parser<char>);
            else if (name == "unsigned char")
                ret.reset(new integer_value_parser<unsigned char>);
            else if (name == "byte" || name == "SInt8")
                ret.reset(new integer_value_parser<std::int8_t>);
            else if (name == "unsigned byte" || name == "UInt8")
                ret.reset(new integer_value_parser<std::uint8_t>);
            else if (name == "short" || name == "SInt16")
                ret.reset(new integer_value_parser<std::int16_t>);
            else if (name == "unsigned short" || name == "UInt16")
                ret.reset(new integer_value_parser<std::uint16_t>);
            else if (name == "int" || name == "SInt32")
                ret.reset(new integer_value_parser<std::int32_t>);
            else if (name == "unsigned int" || name == "UInt32")
                ret.reset(new integer_value_parser<std::uint32_t>);
            else if (name == "SInt64")
                ret.reset(new integer_value_parser<std::int64_t>);
            else if (name == "UInt64")
                ret.reset(new integer_value_parser<std::uint64_t>);
            else if (name == "float")
                ret.reset(new float_value_parser());
            else if (name == "double")
                ret.reset(new double_value_parser());
            else
                ret.reset(new fixed_size_blob_value_parser(static_cast<std::size_t>(size)));
        } else if (name == "string") {
            // XXX: we should check that the type named "string" is actually what we expect
            if (member_name != nullptr && *member_name == "m_Script")
                ret.reset(new multiline_string_value_parser);
            else
                ret.reset(new string_value_parser);
        } else if (name == "GUID") {
            // XXX: we should check that the type named "GUID" is actually what we expect
            ret.reset(new guid_value_parser);
        } else if (name.find("PPtr<") == 0 && name.at(name.size() - 1) == '>') {
            // XXX: we should check that the type named "PPtr<*>" is actually what we expect
            ret.reset(new asset_reference_value_parser);
        } else {
            std::unique_ptr<composite_value_parser> composite_parser(new composite_value_parser);
            composite_parser->reserve(members.size());
            for (const unity_type_member &member : members)
                composite_parser->add_member(member.name, member.type.get_value_parser(&member.name));
            ret = std::move(composite_parser);
        }
        if (requires_padding())
            ret = std::unique_ptr<value_parser>(new padded_value_parser(std::move(ret)));
        return std::move(ret);
    }
}
