#include "unity_array_value.hpp"

#include "../json_writer.hpp"

namespace zizany {
    unity_array_value::unity_array_value()
            : unity_value(), elements() {
    }

    void
    unity_array_value::print(json_writer &writer) const {
        writer.start_array();
        for (const unity_value &element : elements)
            element.print(writer);
        writer.end_array();
    }

    bool
    unity_array_value::equals(const unity_value &value) const {
        if (!value.size_equals(elements.size()))
            return false;
        for (std::size_t index = 0; index < elements.size(); ++index)
            if (!value.item_equals(index, elements.at(index)))
                return false;
        return true;
    }

    bool
    unity_array_value::size_equals(std::size_t size) const {
        return elements.size() == size;
    }

    bool
    unity_array_value::item_equals(std::size_t index, const unity_value &value) const {
        return elements.at(index).equals(value);
    }
}
