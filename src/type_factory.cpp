#include "type_factory.hpp"

#include <stdexcept>

namespace zizany {
    unity_type
    create_simple_type(const std::string &name, bool requires_padding) {
        unity_type type;
        type.name = name;
        type.set_requires_padding(requires_padding);
        if (name == "SInt8" || name == "UInt8" || name == "byte" || name == "unsigned byte" || name == "char" || name == "unsigned char" || name == "bool")
            type.size = 1;
        else if (name == "SInt16" || name == "UInt16" || name == "short" || name == "unsigned short")
            type.size = 2;
        else if (name == "SInt32" || name == "UInt32" || name == "int" || name == "unsigned int" || name == "float")
            type.size = 4;
        else if (name == "SInt64" || name == "UInt64" || name == "double")
            type.size = 4;
        else
            throw std::runtime_error("unexpected simple type name");
        return type;
    }

    unity_type
    create_array_type(const std::string &name, const std::string &size_name, const unity_type &size_type, const std::string &element_name, const unity_type &element_type) {
        unity_type type;
        type.name = name;
        type.is_array = true;
        type.set_requires_padding(true);
        type.size = -1;
        type.members.reserve(2);
        type.members.push_back({size_name, size_type});
        type.members.push_back({element_name, element_type});
        return type;
    }

    unity_type
    create_string_type() {
        unity_type type;
        type.name = "string";
        type.size = -1;
        type.members.reserve(1);
        type.members.push_back(
                {"Array", create_array_type("Array",
                        "size", create_simple_type("int"),
                        "data", create_simple_type("char"))});
        return type;
    }

    unity_type create_asset_reference_type(const std::string &target_type) {
        unity_type type;
        type.name = "PPtr<" + target_type + ">";
        type.size = 8;
        type.members.reserve(2);
        type.members.push_back({"m_FileID", create_simple_type("int")});
        type.members.push_back({"m_PathID", create_simple_type("int")});
        return type;
    }

    unity_type create_tail_blob_type() {
        unity_type type;
        type.name = "blob";
        type.size = -1;
        type.is_tail_blob = true;
        return type;
    }

    unity_type create_release_gameobject_type() {
        unity_type type;
        type.name = "GameObject";
        type.size = -1;
        unity_type pair_type;
        pair_type.name = "pair";
        pair_type.size = 12;
        pair_type.members.reserve(2);
        // This int is a type_id, but always a positive one, so there's no need to convert it to a durable type_identity
        pair_type.members.push_back({"first", create_simple_type("int")});
        pair_type.members.push_back({"second", create_asset_reference_type("Component")});
        unity_type vector_type;
        vector_type.name = "vector";
        vector_type.size = -1;
        vector_type.members.reserve(1);
        vector_type.members.push_back({
                "Array",
                create_array_type("Array",
                        "size", create_simple_type("int"),
                        "data", pair_type)});
        type.members.reserve(5);
        type.members.push_back({"m_Component", vector_type});
        type.members.push_back({"m_Layer", create_simple_type("int")});
        type.members.push_back({"m_Name", create_string_type()});
        type.members.push_back({"m_Tag", create_simple_type("short")});
        type.members.push_back({"m_IsActive", create_simple_type("bool")});
        return type;
    }

    unity_type create_release_monobehaviour_type() {
        unity_type type;
        type.name = "MonoBehaviour";
        type.size = 20;
        type.members.reserve(4);
        type.members.push_back({"m_GameObject", create_asset_reference_type("GameObject")});
        type.members.push_back({"m_Enabled", create_simple_type("UInt8", /*requires_padding:*/true)});
        type.members.push_back({"m_Script", create_asset_reference_type("MonoScript")});
        type.members.push_back({"magic_bytes", create_tail_blob_type()});
        return type;
    }

    unity_type create_release_monoscript_type() {
        unity_type type;
        type.name = "MonoScript";
        type.size = -1;
        type.members.reserve(6);
        type.members.push_back({"m_Name", create_string_type()});
        type.members.push_back({"m_ExecutionOrder", create_simple_type("int")});
        type.members.push_back({"magic_int_1", create_simple_type("unsigned int")});
        type.members.push_back({"m_ClassName", create_string_type()});
        type.members.push_back({"m_Namespace", create_string_type()});
        type.members.push_back({"m_AssemblyName", create_string_type()});
        type.members.push_back({"m_IsEditorScript", create_simple_type("bool")});
        return type;
    }
}
