#include "unity_array_value.hpp"

#include "../json_writer.hpp"
#include "../diff/reverse_delta_store.hpp"
#include "../diff/sequence_comparer.hpp"

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
        return value.equals(*this);
    }

    bool
    unity_array_value::equals(const unity_array_value &value) const {
        if (elements.size() != value.elements.size())
            return false;
        for (std::size_t index = 0; index < elements.size(); ++index)
            if (!elements.at(index).equals(value.elements.at(index)))
                return false;
        return true;
    }

    void
    unity_array_value::compare(const unity_value &value, delta_store &store) const {
        reverse_delta_store reverse_store(store);
        value.compare(*this, reverse_store);
    }

    void
    unity_array_value::compare(const unity_array_value &value, delta_store &store) const {
        compare_sequences(*this, value, store);
    }
}
