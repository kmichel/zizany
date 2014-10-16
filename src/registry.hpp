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
        has_id(const id_type id) const {
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

        class iterator {
            typename owning_vector<entry_type>::iterator entries_iterator;
        public:
            iterator(const typename owning_vector<entry_type>::iterator entries_iterator_)
                    : entries_iterator(entries_iterator_) {
            }

            entry_type &operator*() const {
                return *entries_iterator;
            }

            bool operator!=(const iterator &rhs) const {
                return entries_iterator != rhs.entries_iterator;
            }

            const iterator &operator++() {
                ++entries_iterator;
                return *this;
            }
        };

        class const_iterator {
            typename owning_vector<entry_type>::const_iterator entries_iterator;
        public:
            const_iterator(const typename owning_vector<entry_type>::const_iterator entries_iterator_)
                    : entries_iterator(entries_iterator_) {
            }

            const entry_type &operator*() const {
                return *entries_iterator;
            }

            bool operator!=(const const_iterator &rhs) const {
                return entries_iterator != rhs.entries_iterator;
            }

            const const_iterator &operator++() {
                ++entries_iterator;
                return *this;
            }
        };

        iterator begin() {
            return iterator(entries.begin());
        }

        iterator end() {
            return iterator(entries.end());
        }

        const_iterator begin() const {
            return const_iterator(entries.begin());
        }

        const_iterator end() const {
            return const_iterator(entries.end());
        }
    };
}
