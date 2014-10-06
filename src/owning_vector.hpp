#pragma once

#include <memory>
#include <vector>

namespace zizany {
    template<typename element_type>
    class owning_vector {
        typedef std::size_t size_type;
        typedef typename std::vector<std::unique_ptr<element_type>> vector_type;
        vector_type elements;
    public:
        owning_vector()
                : elements() {
        }

        owning_vector(const owning_vector &) = delete;

        owning_vector &operator=(const owning_vector &) = delete;

        void reserve(size_type size) {
            elements.reserve(size);
        }

        size_type size() const {
            return elements.size();
        }

        void add(std::unique_ptr<element_type> &&member) {
            elements.push_back(std::move(member));
        }

        element_type &
        at(const size_type index) {
            return *elements.at(index);
        }

        const element_type &
        at(const size_type index) const {
            return *elements.at(index);
        }

        class iterator {
            typename vector_type::iterator vector_iterator;
        public:
            iterator(typename vector_type::iterator vector_iterator_)
                    : vector_iterator(vector_iterator_) {
            }

            element_type &operator*() const {
                return **vector_iterator;
            }

            bool operator!=(const iterator &rhs) const {
                return vector_iterator != rhs.vector_iterator;
            }

            const iterator &operator++() {
                ++vector_iterator;
                return *this;
            }
        };

        class const_iterator {
            typename vector_type::const_iterator vector_iterator;
        public:
            const_iterator(typename vector_type::const_iterator vector_iterator_)
                    : vector_iterator(vector_iterator_) {
            }

            const element_type &operator*() const {
                return **vector_iterator;
            }

            bool operator!=(const const_iterator &rhs) const {
                return vector_iterator != rhs.vector_iterator;
            }

            const const_iterator &operator++() {
                ++vector_iterator;
                return *this;
            }
        };

        iterator begin() {
            return iterator(elements.begin());
        }

        iterator end() {
            return iterator(elements.end());
        }

        const_iterator begin() const {
            return const_iterator(elements.begin());
        }

        const_iterator end() const {
            return const_iterator(elements.end());
        }
    };
}
