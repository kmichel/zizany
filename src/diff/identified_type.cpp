#include "identified_type.hpp"

#include "../unity_file.hpp"
#include "../unity_type.hpp"

namespace zizany {
    std::vector<identified_type>
    identify_types(const unity_file &file) {
        std::vector<identified_type> identified_types;
        for (std::size_t type_index = 0; type_index < file.types.size(); ++type_index) {
            const int type_id(file.types.get_id_at(type_index));
            const unity_type &type(file.types.at(type_index));
            identified_types.push_back({get_type_identity(file, type_id), type});
        }
        return identified_types;
    }
}
