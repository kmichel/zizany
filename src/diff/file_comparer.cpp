#include "../delta_store.hpp"
#include "../guid.hpp"
#include "../unity_asset.hpp"
#include "../unity_file.hpp"
#include "../unity_file_reference.hpp"
#include "../unity_preview.hpp"
#include "../unity_type.hpp"
#include "../values/unity_value.hpp"
#include "file_comparer.hpp"
#include "file_reference_comparer.hpp"
#include "preview_comparer.hpp"
#include "type_comparer.hpp"
#include "identified_type.hpp"

namespace zizany {

    void
    compare_files(const unity_file &base, const unity_file &other, delta_store &store) {
        // Those modification objects can be applied to another unity_file if they have the right related elements
        // Two modifications with disjoint list of related elements will be applicable together,
        // this will be the basis of conflict resolution in three-way merges.

        bool layout_modified = false;

        if (base.unity_version != other.unity_version)
            store.modify_unity_version(base.unity_version, other.unity_version);

        if (base.magic_int_1 != other.magic_int_1)
            store.modify_file_magic(1, base.magic_int_1, other.magic_int_1);
        if (base.magic_int_2 != other.magic_int_2)
            store.modify_file_magic(2, base.magic_int_2, other.magic_int_2);
        if (base.magic_int_3 != other.magic_int_3)
            store.modify_file_magic(3, base.magic_int_3, other.magic_int_3);

        if (base.file_layout.metadata_size != other.file_layout.metadata_size)
            layout_modified = true;
        if (base.file_layout.assets_start != other.file_layout.assets_start)
            layout_modified = true;
        if (base.file_layout.previews_start != other.file_layout.previews_start)
            layout_modified = true;
        if (base.file_layout.file_endianness != other.file_layout.file_endianness)
            layout_modified = true;

        { // types
            std::vector<identified_type> base_identified_types(identify_types(base));
            std::vector<identified_type> other_identified_types(identify_types(other));

            for (const identified_type &base_identified_type : base_identified_types) {
                bool match_found = false;
                for (const identified_type &other_identified_type : other_identified_types)
                    if (base_identified_type.identity == other_identified_type.identity) {
                        match_found = true;
                        store.set_current_type(base_identified_type.identity);
                        compare_types(base_identified_type.type, other_identified_type.type, store);
                        break;
                    }
                if (!match_found)
                    store.remove_type(base_identified_type.identity, base_identified_type.type);
            }
            for (const identified_type &other_identified_type : other_identified_types) {
                bool match_found = false;
                for (const identified_type &base_identified_type : base_identified_types)
                    if (base_identified_type.identity == other_identified_type.identity) {
                        match_found = true;
                        break;
                    }
                if (!match_found)
                    store.add_type(other_identified_type.identity, other_identified_type.type);
            }
        }

        { // assets
            // XXX: for release files, we don't have type structure, which means we can't
            // read the m_script value, and can't generate a durable type_identity.
            for (std::size_t base_asset_index = 0; base_asset_index < base.assets.size(); ++base_asset_index) {
                const int base_asset_id(base.assets.get_id_at(base_asset_index));
                const unity_asset &base_asset(base.assets.at(base_asset_index));
                if (other.assets.has_id(base_asset_id)) {
                    const unity_asset &other_asset(other.assets.get_by_id(base_asset_id));
                    const type_identity &base_asset_identity(get_type_identity(base, base_asset.type_id));
                    const type_identity &other_asset_identity(get_type_identity(other, other_asset.type_id));
                    store.set_current_asset(base_asset_id);
                    if (!(base_asset_identity == other_asset_identity))
                        store.modify_asset_type(base_asset_id, base_asset_identity, other_asset_identity);
                    if (base_asset.type_id_2 != other_asset.type_id_2)
                        store.modify_asset_type_2(base_asset_id, base_asset.type_id_2, other_asset.type_id_2);
                    base_asset.value->compare(*other_asset.value, store);
                    if (
                            base_asset.file_layout.offset != other_asset.file_layout.offset
                                    || base_asset.file_layout.size != other_asset.file_layout.size) {
                        layout_modified = true;
                    }
                } else {
                    store.remove_asset(base_asset_id, get_type_identity(base, base_asset.type_id), *base_asset.value);
                }
            }

            for (std::size_t other_asset_index = 0; other_asset_index < other.assets.size(); ++other_asset_index) {
                const int other_asset_id(other.assets.get_id_at(other_asset_index));
                const unity_asset &other_asset(other.assets.at(other_asset_index));
                if (!base.assets.has_id(other_asset_id))
                    store.add_asset(other_asset_id, get_type_identity(other, other_asset.type_id), *other_asset.value);
            }
        }

        { // file-references
            for (const unity_file_reference &base_file_reference : base.file_references) {
                bool match_found = false;
                for (const unity_file_reference &other_file_reference : other.file_references)
                    if (base_file_reference.file_guid == other_file_reference.file_guid) {
                        match_found = true;
                        if (!are_file_references_equal(base_file_reference, other_file_reference))
                            store.modify_file_reference(base_file_reference, other_file_reference);
                        break;
                    }
                if (!match_found)
                    store.remove_file_reference(base_file_reference);
            }

            for (const unity_file_reference &other_file_reference : other.file_references) {
                bool match_found = false;
                for (const unity_file_reference &base_file_reference : base.file_references)
                    if (base_file_reference.file_guid == other_file_reference.file_guid) {
                        match_found = true;
                        break;
                    }
                if (!match_found)
                    store.add_file_reference(other_file_reference);;
            }
        }

        { // previews
            for (const unity_preview &base_preview : base.previews) {
                bool match_found = false;
                for (const unity_preview &other_preview : other.previews)
                    if (base_preview.asset_id == other_preview.asset_id) {
                        match_found = true;
                        if (!are_previews_equal(base_preview, other_preview))
                            store.modify_preview(base_preview, other_preview);
                        if (
                                base_preview.file_layout.offset != other_preview.file_layout.offset
                                        || base_preview.file_layout.size != other_preview.file_layout.size) {
                            layout_modified = true;
                        }
                        break;
                    }
                if (!match_found)
                    store.remove_preview(base_preview);
            }

            for (const unity_preview &other_preview : other.previews) {
                bool match_found = false;
                for (const unity_preview &base_preview : base.previews)
                    if (base_preview.asset_id == other_preview.asset_id) {
                        match_found = true;
                        break;
                    }
                if (!match_found)
                    store.add_preview(other_preview);
            }
        }

        if (layout_modified)
            store.mark_modified_layout();
    }
}
