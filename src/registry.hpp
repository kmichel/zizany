#pragma once

#include <utility>
#include <vector>
#include <map>

#include "owning_vector.hpp"

namespace zizany {
    template<typename entry_type>
    class registry {
        typedef int id_type;
        typedef std::size_t size_type;
        owning_vector<entry_type> entries;
        std::vector<id_type> ids;
        std::map<id_type, size_type> id_index;
    public:
        registry()
                : entries(),
                  ids(),
                  id_index() {
        }

        registry(const registry &) = delete;

        registry &operator=(const registry &) = delete;

        void
        add(const id_type id, std::unique_ptr<entry_type> &&entry) {
            entries.add(std::move(entry));
            ids.push_back(id);
            id_index.insert(std::make_pair(id, entries.size() - 1));
        }

        bool
        has_id(const id_type id) {
            return id_index.find(id) != id_index.end();
        }

        entry_type &
        get_by_id(const id_type id) {
            return at(get_id_index(id));
        }

        entry_type &
        at(const size_type position) {
            return entries.at(position);
        }

        const entry_type &
        get_by_id(const id_type id) const {
            return at(get_id_index(id));
        }

        const entry_type &
        at(const size_type index) const {
            return entries.at(index);
        }

        size_type
        get_id_index(id_type id) const {
            return id_index.at(id);
        }

        id_type
        get_id_at(size_type index) const {
            return ids.at(index);
        }

        size_type
        size() const {
            return entries.size();
        }
    };
}
