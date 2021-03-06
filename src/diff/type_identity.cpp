#include "type_identity.hpp"

#include "../json_writer.hpp"
#include "../unity_asset.hpp"
#include "../unity_file.hpp"
#include "../unity_type.hpp"
#include "../unity_value.hpp"
#include "../values/unity_asset_reference_value.hpp"
#include "../values/unity_composite_value.hpp"

#include <stdexcept>

namespace zizany {
    type_identity::type_identity(const int type_id_)
            : kind(type_kind::builtin_type),
              type_id(type_id_),
              script_asset() {
    }

    type_identity::type_identity(const asset_reference &script_asset_)
            : kind(type_kind::script_type),
              type_id(),
              script_asset(script_asset_) {
    }

    void
    type_identity::print(json_writer &writer) const {
        writer.start_object();
        {
            writer.add_key("kind");
            switch (kind) {
                case type_kind::builtin_type:
                    writer.add_string("builtin");
                    writer.add_key("type_id");
                    writer.add_number(type_id);
                    break;
                case type_kind::script_type:
                    writer.add_string("script");
                    writer.add_key("script_asset");
                    script_asset.print(writer);
                    break;
            }
        }
        writer.end_object();
    }

    bool operator==(const type_identity &lhs, const type_identity &rhs) {
        if (lhs.kind == rhs.kind)
            switch (lhs.kind) {
                case type_kind::builtin_type:
                    return lhs.type_id == rhs.type_id;
                case type_kind::script_type:
                    return lhs.script_asset == rhs.script_asset;
            }
        return false;
    }

    bool operator!=(const type_identity &lhs, const type_identity &rhs) {
        if (lhs.kind == rhs.kind)
            switch (lhs.kind) {
                case type_kind::builtin_type:
                    return lhs.type_id != rhs.type_id;
                case type_kind::script_type:
                    return lhs.script_asset != rhs.script_asset;
            }
        return true;
    }

    static const unity_asset *get_asset_of_type(const unity_file &file, const int type_id);

    static const unity_value *find_member_named(const unity_value &value, const std::string &name);

    type_identity
    get_type_identity(const unity_file &file, const int type_id) {
        if (type_id > 0) {
            return type_identity(type_id);
        } else if (type_id < 0) {
            const unity_asset *const asset(get_asset_of_type(file, type_id));
            if (asset != nullptr) {
                const unity_value *const asset_script(find_member_named(*asset->value, "m_Script"));
                if (asset_script != nullptr) {
                    const unity_asset_reference_value *const asset_script_reference(dynamic_cast<const unity_asset_reference_value *>(asset_script));
                    // XXX: on release files, the reference is not durable enough
                    // All scripts are bundled together in a large bundle where asset_id change
                    // between releases for a given script.
                    // However, we can open the referenced file, parse the MonoScript asset
                    // And from this we can get the class name and namespace.
                    if (asset_script_reference != nullptr)
                        return type_identity(asset_script_reference->value);
                }
            }
        }
        throw std::runtime_error("unexpected type structure");
    }

    static
    const unity_asset *get_asset_of_type(const unity_file &file, const int type_id) {
        for (std::size_t asset_index = 0; asset_index < file.assets.size(); ++asset_index) {
            const unity_asset &asset(file.assets.at(asset_index));
            if (asset.type_id == type_id)
                return &asset;
        }
        return nullptr;
    }

    static
    const unity_value *find_member_named(const unity_value &value, const std::string &name) {
        const unity_composite_value *const composite_value(dynamic_cast<const unity_composite_value *>(&value));
        if (composite_value != nullptr)
            return composite_value->members.get_by_name(name);
        return nullptr;
    }
}
