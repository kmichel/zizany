#pragma once

#include "../endianness.hpp"

#include <string>

namespace zizany {
    class delta;

    struct guid;

    class json_writer;

    struct type_identity;

    class unity_file_reference;

    class unity_file_reference_properties;

    class unity_preview;

    class unity_type;

    class unity_value;

    class delta_store {
    public:
        virtual ~delta_store();

        virtual void modify_unity_version(const std::string &old_version, const std::string &new_version) = 0;

        virtual void modify_file_magic(const int magic_id, const int old_value, const int new_value) = 0;

        virtual void modifiy_endianness(const endianness old_endianness, const endianness new_endianness) = 0;

        virtual void mark_modified_layout() = 0;

        virtual void set_current_asset(const int asset_id) = 0;

        virtual void set_current_type(const type_identity &type) = 0;

        virtual void enter_member(const std::string &member_name) = 0;

        virtual void leave_member() = 0;

        virtual void add_asset(const int asset_id, const type_identity &type, const unity_value &value) = 0;

        virtual void remove_asset(const int asset_id, const type_identity &type, const unity_value &value) = 0;

        virtual void modify_asset_type(const int asset_id, const type_identity &old_type, const type_identity &new_type) = 0;

        virtual void modify_asset_type_2(const int asset_id, const int old_type_2, const int new_type_2) = 0;

        virtual void modify_value(const unity_value &old_value, const unity_value &new_value) = 0;

        virtual void add_value_member(const std::string &member_name, const unity_value &value) = 0;

        virtual void remove_value_member(const std::string &member_name, const unity_value &value) = 0;

        virtual void insert_sequence_item(const int position, const unity_value &value) = 0;

        virtual void remove_sequence_item(const int position, const unity_value &value) = 0;

        virtual void add_type(const type_identity &identity, const unity_type &type) = 0;

        virtual void remove_type(const type_identity &identity, const unity_type &type) = 0;

        virtual void modify_type(const unity_type &old_type, const unity_type &new_type) = 0;

        virtual void add_type_member(const std::string &member_name, const unity_type &type) = 0;

        virtual void remove_type_member(const std::string &member_name, const unity_type &type) = 0;

        virtual void add_file_reference(const unity_file_reference &reference) = 0;

        virtual void remove_file_reference(const unity_file_reference &reference) = 0;

        virtual void modify_file_reference(const guid &file_guid, const unity_file_reference_properties &old_properties, const unity_file_reference_properties &new_properties) = 0;

        virtual void add_preview(const unity_preview &preview) = 0;

        virtual void remove_preview(const unity_preview &preview) = 0;

        virtual void modify_preview(const unity_preview &old_preview, const unity_preview &new_preview) = 0;

        virtual void print(json_writer &writer) const = 0;
    };
}
