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
            : is_script(false),
              type_id(type_id_),
              script({{0, 0, 0, 0}, 0}) {
    }

    type_identity::type_identity(const guid &file_guid_, const int asset_id_)
            : is_script(true),
              type_id(0),
              script({file_guid_, asset_id_}) {
    }

    const unity_type *
    type_identity::resolve(const unity_file &file) const {
        if (is_script) {
            for (std::size_t type_index = 0; type_index < file.types.size(); ++type_index) {
                const int file_type_id(file.types.get_id_at(type_index));
                const unity_type &type(file.types.at(type_index));
                if (file_type_id < 0 && *this == get_type_identity(file, file_type_id))
                    return &type;
            }
        } else if (file.types.has_id(type_id)) {
            return &file.types.get_by_id(type_id);
        }
        return nullptr;
    }

    void
    type_identity::print(json_writer &writer) const {
        writer.start_object();
        {
            writer.add_key("is_script");
            writer.add_bool(is_script);
            if (is_script) {
                writer.add_key("file_guid");
                script.file_guid.print(writer);
                writer.add_key("asset_id");
                writer.add_number(script.asset_id);
            } else {
                writer.add_key("type_id");
                writer.add_number(type_id);
            }
        }
        writer.end_object();
    }

    bool operator==(const type_identity &lhs, const type_identity &rhs) {
        if (!lhs.is_script && !rhs.is_script)
            return lhs.type_id == rhs.type_id;
        if (lhs.is_script == rhs.is_script)
            return lhs.script.file_guid == rhs.script.file_guid && lhs.script.asset_id == rhs.script.asset_id;
        return false;
    }

    static const unity_asset *get_asset_of_type(const unity_file &file, const int type_id);

    static const unity_value *find_member_named(const unity_value &value, const std::string &name);

    type_identity
    get_type_identity(const unity_file &file, const int type_id) {
        if (type_id > 0) {
            return type_identity(type_id);
        } else if (type_id < 0) {
            const unity_asset *asset(get_asset_of_type(file, type_id));
            if (asset != nullptr) {
                const unity_value *asset_script(find_member_named(*asset->value, "m_Script"));
                if (asset_script != nullptr) {
                    const unity_asset_reference_value *asset_script_reference(dynamic_cast<const unity_asset_reference_value *>(asset_script));
                    return type_identity(asset_script_reference->value.file_guid, asset_script_reference->value.asset_id);
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
        const unity_composite_value *composite_value(dynamic_cast<const unity_composite_value *>(&value));
        if (composite_value != nullptr)
            return composite_value->members.get_by_name(name);
        return nullptr;
    }
}
