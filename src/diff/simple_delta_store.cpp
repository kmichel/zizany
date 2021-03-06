#include "simple_delta_store.hpp"

#include "delta.hpp"
#include "deltas/assets_delta.hpp"
#include "deltas/asset_delta.hpp"
#include "deltas/asset_type_delta.hpp"
#include "deltas/asset_type_2_delta.hpp"
#include "deltas/asset_members_delta.hpp"
#include "deltas/file_endianness_delta.hpp"
#include "deltas/file_layout_delta.hpp"
#include "deltas/file_magic_delta.hpp"
#include "deltas/file_references_delta.hpp"
#include "deltas/file_reference_delta.hpp"
#include "deltas/previews_delta.hpp"
#include "deltas/preview_delta.hpp"
#include "deltas/sequence_delta.hpp"
#include "deltas/types_delta.hpp"
#include "deltas/type_delta.hpp"
#include "deltas/type_members_delta.hpp"
#include "deltas/unity_version_delta.hpp"
#include "../json_writer.hpp"

namespace zizany {
    simple_delta_store::simple_delta_store()
            : delta_store(), deltas(), current_asset_id(0), current_type(0), path() {
    }

    void
    simple_delta_store::modify_unity_version(const std::string &old_version, const std::string &new_version) {
        deltas.add(new unity_version_delta(old_version, new_version));
    }


    void
    simple_delta_store::modify_file_magic(const int magic_id, const int old_value, const int new_value) {
        deltas.add(new file_magic_delta(magic_id, old_value, new_value));
    }

    void
    simple_delta_store::modifiy_endianness(const endianness old_endianness, const endianness new_endianness) {
        deltas.add(new file_endianness_delta(old_endianness, new_endianness));
    }

    void
    simple_delta_store::mark_modified_layout() {
        deltas.add(new file_layout_delta());
    }

    void
    simple_delta_store::set_current_asset(const int asset_id) {
        current_asset_id = asset_id;
    }

    void
    simple_delta_store::set_current_type(const type_identity &type) {
        current_type = type;
    }

    void
    simple_delta_store::enter_member(const std::string &member_name) {
        path.push(member_name);
    }

    void
    simple_delta_store::leave_member() {
        path.pop();
    }

    void
    simple_delta_store::add_asset(const int asset_id, const type_identity &type, const unity_value &value) {
        deltas.add(new assets_delta(delta_set_operation::add, asset_id, type, value));
    }

    void
    simple_delta_store::remove_asset(const int asset_id, const type_identity &type, const unity_value &value) {
        deltas.add(new assets_delta(delta_set_operation::remove, asset_id, type, value));
    }

    void
    simple_delta_store::modify_asset_type(const int asset_id, const type_identity &old_type, const type_identity &new_type) {
        deltas.add(new asset_type_delta(asset_id, old_type, new_type));
    }

    void
    simple_delta_store::modify_asset_type_2(const int asset_id, const int old_type_2, const int new_type_2) {
        deltas.add(new asset_type_2_delta(asset_id, old_type_2, new_type_2));
    }

    void
    simple_delta_store::modify_value(const unity_value &old_value, const unity_value &new_value) {
        deltas.add(new asset_delta(current_asset_id, path, old_value, new_value));
    }

    void
    simple_delta_store::add_value_member(const std::string &member_name, const unity_value &value) {
        deltas.add(new asset_members_delta(delta_set_operation::add, current_asset_id, path.join(member_name), value));
    }

    void
    simple_delta_store::remove_value_member(const std::string &member_name, const unity_value &value) {
        deltas.add(new asset_members_delta(delta_set_operation::remove, current_asset_id, path.join(member_name), value));
    }

    void
    simple_delta_store::insert_sequence_item(const int position, const unity_value &value) {
        deltas.add(new sequence_delta(delta_sequence_operation::insert, current_asset_id, path, position, value));
    }

    void
    simple_delta_store::remove_sequence_item(const int position, const unity_value &value) {
        deltas.add(new sequence_delta(delta_sequence_operation::remove, current_asset_id, path, position, value));
    }

    void
    simple_delta_store::add_type(const type_identity &identity, const unity_type &type) {
        deltas.add(new types_delta(delta_set_operation::add, identity, type));
    }

    void
    simple_delta_store::remove_type(const type_identity &identity, const unity_type &type) {
        deltas.add(new types_delta(delta_set_operation::remove, identity, type));
    }

    void
    simple_delta_store::modify_type(const unity_type &old_type, const unity_type &new_type) {
        deltas.add(new type_delta(current_type, path, old_type, new_type));
    }

    void
    simple_delta_store::add_type_member(const std::string &member_name, const unity_type &type) {
        deltas.add(new type_members_delta(delta_set_operation::add, current_type, path.join(member_name), type));
    }

    void
    simple_delta_store::remove_type_member(const std::string &member_name, const unity_type &type) {
        deltas.add(new type_members_delta(delta_set_operation::remove, current_type, path.join(member_name), type));
    }

    void
    simple_delta_store::add_file_reference(const unity_file_reference &reference) {
        deltas.add(new file_references_delta(delta_set_operation::add, reference));
    }

    void
    simple_delta_store::remove_file_reference(const unity_file_reference &reference) {
        deltas.add(new file_references_delta(delta_set_operation::remove, reference));
    }

    void
    simple_delta_store::modify_file_reference(const guid &file_guid, const unity_file_reference_properties &old_properties, const unity_file_reference_properties &new_properties) {
        deltas.add(new file_reference_delta(file_guid, old_properties, new_properties));
    }

    void
    simple_delta_store::add_preview(const unity_preview &preview) {
        deltas.add(new previews_delta(delta_set_operation::add, preview));
    }

    void
    simple_delta_store::remove_preview(const unity_preview &preview) {
        deltas.add(new previews_delta(delta_set_operation::remove, preview));
    }

    void
    simple_delta_store::modify_preview(const unity_preview &old_preview, const unity_preview &new_preview) {
        deltas.add(new preview_delta(old_preview, new_preview));
    }

    void
    simple_delta_store::print(json_writer &writer) const {
        writer.start_array();
        for (const delta &delta_: deltas)
            delta_.print(writer);
        writer.end_array();
    }
}
