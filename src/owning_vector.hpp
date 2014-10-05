#pragma once

#include <memory>
#include <vector>

namespace zizany {
    template<typename element_type>
    class owning_vector {
        typedef std::size_t size_type;
        std::vector<std::unique_ptr<element_type>> elements;
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
    };
}
