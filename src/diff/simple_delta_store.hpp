#pragma once

#include "delta_store.hpp"
#include "member_path.hpp"
#include "type_identity.hpp"
#include "../owning_vector.hpp"

namespace zizany {
    class simple_delta_store : public delta_store {
        owning_vector<delta> deltas;
        int current_asset_id;
        type_identity current_type;
        member_path path;
    public:
        simple_delta_store();

        virtual void modify_unity_version(const std::string &old_version, const std::string &new_version);

        virtual void set_current_asset(const int asset_id);

        virtual void set_current_type(const type_identity &type);

        virtual void enter_member(const std::string &member_name);

        virtual void modify_file_magic(const int magic_id, const int old_value, const int new_value);

        virtual void modifiy_endianness(const endianness old_endianness, const endianness new_endianness);

        virtual void mark_modified_layout();

        virtual void leave_member();

        virtual void add_asset(const int asset_id, const type_identity &type, const unity_value &value);

        virtual void remove_asset(const int asset_id, const type_identity &type, const unity_value &value);

        virtual void modify_asset_type(const int asset_id, const type_identity &old_type, const type_identity &new_type);

        virtual void modify_asset_type_2(const int asset_id, const int old_type_2, const int new_type_2);

        virtual void modify_value(const unity_value &old_value, const unity_value &new_value);

        virtual void add_value_member(const std::string &member_name, const unity_value &value);

        virtual void remove_value_member(const std::string &member_name, const unity_value &value);

        virtual void insert_sequence_item(const int position, const unity_value &value);

        virtual void remove_sequence_item(const int position, const unity_value &value);

        virtual void add_type(const type_identity &identity, const unity_type &type);

        virtual void remove_type(const type_identity &identity, const unity_type &type);

        virtual void modify_type(const unity_type &old_type, const unity_type &new_type);

        virtual void add_type_member(const std::string &member_name, const unity_type &type);

        virtual void remove_type_member(const std::string &member_name, const unity_type &type);

        virtual void add_file_reference(const unity_file_reference &reference);

        virtual void remove_file_reference(const unity_file_reference &reference);

        virtual void modify_file_reference(const guid &file_guid, const unity_file_reference_properties &old_properties, const unity_file_reference_properties &new_properties);

        virtual void add_preview(const unity_preview &preview);

        virtual void remove_preview(const unity_preview &preview);

        virtual void modify_preview(const unity_preview &old_preview, const unity_preview &new_preview);

        virtual void print(json_writer &writer) const;
    };
}
