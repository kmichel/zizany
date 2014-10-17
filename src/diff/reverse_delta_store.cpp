#include "reverse_delta_store.hpp"

#include "type_identity.hpp"
#include "../unity_type.hpp"

namespace zizany {

    reverse_delta_store::reverse_delta_store(delta_store &delegate_store_)
            : delta_store(), delegate_store(delegate_store_) {
    }

    void
    reverse_delta_store::modify_unity_version(const std::string &old_version, const std::string &new_version) {
        delegate_store.modify_unity_version(new_version, old_version);
    }

    void
    reverse_delta_store::set_current_asset(const int asset_id) {
        delegate_store.set_current_asset(asset_id);
    }

    void
    reverse_delta_store::set_current_type(const type_identity &type) {
        delegate_store.set_current_type(type);
    }

    void
    reverse_delta_store::enter_member(const std::string &member_name) {
        delegate_store.enter_member(member_name);
    }

    void
    reverse_delta_store::leave_member() {
        delegate_store.leave_member();
    }

    void
    reverse_delta_store::add_asset(const int asset_id, const type_identity &type, const unity_value &value) {
        delegate_store.remove_asset(asset_id, type, value);
    }

    void
    reverse_delta_store::remove_asset(const int asset_id, const type_identity &type, const unity_value &value) {
        delegate_store.add_asset(asset_id, type, value);
    }

    void
    reverse_delta_store::modify_asset_type(const int asset_id, const type_identity &old_type, const type_identity &new_type) {
        delegate_store.modify_asset_type(asset_id, new_type, old_type);
    }

    void
    reverse_delta_store::modify_asset_type_2(const int asset_id, const int old_type_2, const int new_type_2) {
        delegate_store.modify_asset_type_2(asset_id, new_type_2, old_type_2);
    }

    void
    reverse_delta_store::modify_value(const unity_value &old_value, const unity_value &new_value) {
        delegate_store.modify_value(new_value, old_value);
    }

    void
    reverse_delta_store::add_value_member(const std::string &member_name, const unity_value &value) {
        delegate_store.remove_value_member(member_name, value);
    }

    void
    reverse_delta_store::remove_value_member(const std::string &member_name, const unity_value &value) {
        delegate_store.add_value_member(member_name, value);
    }

    void
    reverse_delta_store::insert_sequence_item(const int position, const unity_value &value) {
        delegate_store.remove_sequence_item(position, value);
    }

    void
    reverse_delta_store::remove_sequence_item(const int position, const unity_value &value) {
        delegate_store.insert_sequence_item(position, value);
    }

    void
    reverse_delta_store::add_type(const type_identity &identity, const unity_type &type) {
        delegate_store.remove_type(identity, type);
    }

    void
    reverse_delta_store::remove_type(const type_identity &identity, const unity_type &type) {
        delegate_store.add_type(identity, type);
    }

    void
    reverse_delta_store::modify_type(const unity_type &old_type, const unity_type &new_type) {
        delegate_store.modify_type(new_type, old_type);
    }

    void
    reverse_delta_store::add_type_member(const std::string &member_name, const unity_type &type) {
        delegate_store.remove_type_member(member_name, type);
    }

    void
    reverse_delta_store::remove_type_member(const std::string &member_name, const unity_type &type) {
        delegate_store.add_type_member(member_name, type);
    }

    void
    reverse_delta_store::print(json_writer &writer) const {
        delegate_store.print(writer);
    }

    void
    reverse_delta_store::add_file_reference(const unity_file_reference &reference) {
        delegate_store.remove_file_reference(reference);
    }

    void
    reverse_delta_store::remove_file_reference(const unity_file_reference &reference) {
        delegate_store.add_file_reference(reference);
    }

    void
    reverse_delta_store::modify_file_reference(const guid &file_guid, const unity_file_reference_properties &old_properties, const unity_file_reference_properties &new_properties) {
        delegate_store.modify_file_reference(file_guid, new_properties, old_properties);
    }

    void
    reverse_delta_store::add_preview(const unity_preview &preview) {
        delegate_store.remove_preview(preview);
    }

    void
    reverse_delta_store::remove_preview(const unity_preview &preview) {
        delegate_store.add_preview(preview);
    }

    void
    reverse_delta_store::modify_preview(const unity_preview &old_preview, const unity_preview &new_preview) {
        delegate_store.modify_preview(new_preview, old_preview);
    }

    void
    reverse_delta_store::modify_file_magic(const int magic_id, const int old_value, const int new_value) {
        delegate_store.modify_file_magic(magic_id, new_value, old_value);
    }

    void reverse_delta_store::modifiy_endianness(const endianness old_endianness, const endianness new_endianness) {
        delegate_store.modifiy_endianness(new_endianness, old_endianness);
    }

    void reverse_delta_store::mark_modified_layout() {
        delegate_store.mark_modified_layout();
    }
}
