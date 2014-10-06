#include "simple_delta_store.hpp"

#include "deltas/delta.hpp"
#include "deltas/asset_added_delta.hpp"
#include "deltas/asset_changed_delta.hpp"
#include "deltas/asset_member_added_delta.hpp"
#include "deltas/asset_member_removed_delta.hpp"
#include "deltas/asset_removed_delta.hpp"
#include "deltas/file_endianness_changed_delta.hpp"
#include "deltas/file_layout_changed_delta.hpp"
#include "deltas/file_magic_changed_delta.hpp"
#include "deltas/file_reference_added_delta.hpp"
#include "deltas/file_reference_changed_delta.hpp"
#include "deltas/file_reference_removed_delta.hpp"
#include "deltas/preview_added_delta.hpp"
#include "deltas/preview_changed_delta.hpp"
#include "deltas/preview_removed_delta.hpp"
#include "deltas/type_added_delta.hpp"
#include "deltas/type_changed_delta.hpp"
#include "deltas/type_member_added_delta.hpp"
#include "deltas/type_member_removed_delta.hpp"
#include "deltas/type_removed_delta.hpp"
#include "deltas/unity_version_modified_delta.hpp"
#include "json_writer.hpp"

namespace zizany {
    simple_delta_store::simple_delta_store()
            : delta_store(), deltas(), current_asset_id(0), current_type(0), path() {
    }

    void
    simple_delta_store::modify_unity_version(const std::string &old_version, const std::string &new_version) {
        deltas.add(new unity_version_modified_delta(old_version, new_version));
    }


    void
    simple_delta_store::modify_file_magic(const int magic_id, const int old_value, const int new_value) {
        deltas.add(new file_magic_changed_delta(magic_id, old_value, new_value));
    }

    void
    simple_delta_store::modifiy_endianness(const endianness old_endianness, const endianness new_endianness) {
        deltas.add(new file_endianness_changed_delta(old_endianness, new_endianness));
    }

    void
    simple_delta_store::mark_modified_layout() {
        deltas.add(new file_layout_changed_delta());
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
        deltas.add(new asset_added_delta(asset_id, type, value));
    }

    void
    simple_delta_store::remove_asset(const int asset_id, const type_identity &type, const unity_value &value) {
        deltas.add(new asset_removed_delta(asset_id, type, value));
    }

    void
    simple_delta_store::modify_value(const unity_value &old_value, const unity_value &new_value) {
        deltas.add(new asset_changed_delta(current_asset_id, path, old_value, new_value));
    }

    void
    simple_delta_store::add_value_member(const std::string &member_name, const unity_value &value) {
        deltas.add(new asset_member_added_delta(current_asset_id, path.join(member_name), value));
    }

    void
    simple_delta_store::remove_value_member(const std::string &member_name, const unity_value &value) {
        deltas.add(new asset_member_removed_delta(current_asset_id, path.join(member_name), value));
    }

    void
    simple_delta_store::add_type(const type_identity &identity, const unity_type &type) {
        deltas.add(new type_added_delta(identity, type));
    }

    void
    simple_delta_store::remove_type(const type_identity &identity, const unity_type &type) {
        deltas.add(new type_removed_delta(identity, type));
    }

    void
    simple_delta_store::modify_type(const unity_type &old_type, const unity_type &new_type) {
        deltas.add(new type_changed_delta(current_type, path, old_type, new_type));
    }

    void
    simple_delta_store::add_type_member(const std::string &member_name, const unity_type &type) {
        deltas.add(new type_member_added_delta(current_type, path.join(member_name), type));
    }

    void
    simple_delta_store::remove_type_member(const std::string &member_name, const unity_type &type) {
        deltas.add(new type_member_removed_delta(current_type, path.join(member_name), type));
    }

    void
    simple_delta_store::add_file_reference(const unity_file_reference &reference) {
        deltas.add(new file_reference_added_delta(reference));
    }

    void
    simple_delta_store::remove_file_reference(const unity_file_reference &reference) {
        deltas.add(new file_reference_removed_delta(reference));
    }

    void
    simple_delta_store::modify_file_reference(const unity_file_reference &old_reference, const unity_file_reference &new_reference) {
        deltas.add(new file_reference_changed_delta(old_reference, new_reference));
    }

    void
    simple_delta_store::add_preview(const unity_preview &preview) {
        deltas.add(new preview_added_delta(preview));
    }

    void
    simple_delta_store::remove_preview(const unity_preview &preview) {
        deltas.add(new preview_removed_delta(preview));
    }

    void
    simple_delta_store::modify_preview(const unity_preview &old_preview, const unity_preview &new_preview) {
        deltas.add(new preview_changed_delta(old_preview, new_preview));
    }

    void
    simple_delta_store::print(json_writer &writer) const {
        writer.start_array();
        for (const delta &delta_: deltas)
            delta_.print(writer);
        writer.end_array();
    }
}
