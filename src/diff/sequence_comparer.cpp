#include "sequence_comparer.hpp"

#include "delta_store.hpp"
#include "../values/unity_array_value.hpp"

#include <vector>

namespace zizany {
    enum class cell_state {
        terminal,
        diagonal,
        left,
        top,
        left_and_top
    };

    void
    compare_sequences(const unity_array_value &lhs, const unity_array_value &rhs, delta_store &store) {
        // TODO: optimize the easy case where long prefix and/or suffix are equal
        const std::size_t lhs_size = lhs.elements.size();
        const std::size_t rhs_size = rhs.elements.size();

        std::vector<std::vector<cell_state>> states;
        states.resize(lhs_size + 1);
        for (auto &row : states)
            row.resize(rhs_size + 1);

        for (auto &row : states)
            row[0] = cell_state::top;
        states[0].assign(rhs_size + 1, cell_state::left);
        states[0][0] = cell_state::terminal;

        std::vector<std::size_t> top_row_lengths(rhs_size + 1);

        for (std::size_t lhs_index = 1; lhs_index <= lhs_size; ++lhs_index) {
            std::size_t top_left_cell_length = 0;
            std::size_t left_cell_length = 0;
            for (std::size_t rhs_index = 1; rhs_index <= rhs_size; ++rhs_index) {
                std::size_t top_cell_length = top_row_lengths[rhs_index];
                if (lhs.elements.at(lhs_index - 1).equals(rhs.elements.at(rhs_index - 1))) {
                    states[lhs_index][rhs_index] = cell_state::diagonal;
                    top_row_lengths[rhs_index] = top_left_cell_length + 1;
                    left_cell_length = top_row_lengths[rhs_index];
                } else {
                    if (top_cell_length == left_cell_length) {
                        states[lhs_index][rhs_index] = cell_state::left_and_top;
                        top_row_lengths[rhs_index] = left_cell_length;
                    } else if (top_cell_length > left_cell_length) {
                        states[lhs_index][rhs_index] = cell_state::top;
                        left_cell_length = top_cell_length;
                    } else {
                        states[lhs_index][rhs_index] = cell_state::left;
                        top_row_lengths[rhs_index] = left_cell_length;
                    }
                }
                top_left_cell_length = top_cell_length;
            }
        }

        std::size_t lhs_index = lhs_size;
        std::size_t rhs_index = rhs_size;
        while (lhs_index != 0 || rhs_index != 0) {
            switch (states[lhs_index][rhs_index]) {
                case cell_state::terminal:
                    throw std::runtime_error("illegal state");
                case cell_state::diagonal:
                    lhs_index -= 1;
                    rhs_index -= 1;
                    break;
                case cell_state::left:
                    store.insert_sequence_item(static_cast<int>(lhs_index), rhs.elements.at(rhs_index - 1));
                    rhs_index -= 1;
                    break;
                case cell_state::left_and_top:
                case cell_state::top:
                    store.remove_sequence_item(static_cast<int>(lhs_index), lhs.elements.at(lhs_index - 1));
                    lhs_index -= 1;
                    break;
            }
        }
    }
}
