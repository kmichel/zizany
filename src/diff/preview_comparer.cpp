#include "preview_comparer.hpp"

#include "../unity_preview.hpp"

namespace zizany {
    bool are_previews_equal(const unity_preview &base, const unity_preview &other) {
        if (&base != &other) {
            if (base.magic_int_1 != other.magic_int_1)
                return false;
            if (base.magic_int_2 != other.magic_int_2)
                return false;
            if (base.magic_int_3 != other.magic_int_3)
                return false;
            if (base.data.size() != other.data.size())
                return false;
            for (std::size_t data_index = 0; data_index < base.data.size(); ++data_index)
                if (base.data[data_index] != other.data[data_index])
                    return false;
        }
        return true;
    }
}
