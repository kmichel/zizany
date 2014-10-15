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
              script_asset() {
    }

    type_identity::type_identity(const asset_reference &script_asset_)
            : is_script(true),
              type_id(0),
              script_asset(script_asset_) {
    }

    void
    type_identity::print(json_writer &writer) const {
        writer.start_object();
        {
            writer.add_key("is_script");
            writer.add_bool(is_script);
            if (is_script) {
                writer.add_key("script_asset");
                script_asset.print(writer);
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
            return lhs.script_asset == rhs.script_asset;
        return false;
    }

    bool operator!=(const type_identity &lhs, const type_identity &rhs) {
        if (!lhs.is_script && !rhs.is_script)
            return lhs.type_id != rhs.type_id;
        if (lhs.is_script == rhs.is_script)
            return lhs.script_asset != rhs.script_asset;
        return true;
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
        const unity_composite_value *composite_value(dynamic_cast<const unity_composite_value *>(&value));
        if (composite_value != nullptr)
            return composite_value->members.get_by_name(name);
        return nullptr;
    }
}
