#pragma once

#include <cstddef>
#include <cstring>
#include <memory>

namespace zizany {
    template<typename pod_type>
    class pod_vector {
        std::unique_ptr<pod_type[]> data_begin;
        pod_type *data_end;
    public:
        typedef pod_type *iterator;
        typedef const pod_type *const_iterator;

        pod_vector()
                : data_begin(),
                  data_end(nullptr) {
        }

        pod_vector(const pod_vector &that)
                : data_begin(new pod_type[that.size()]), data_end(data_begin.get() + that.size()) {
            memcpy(that.data_begin.get(), data_begin.get(), that.data_size());
        }

        pod_vector &operator=(const pod_vector &that) {
            allocate(that.size());
            memcpy(that.data_begin.get(), data_begin.get(), that.data_size());
        }

        std::size_t size() const {
            return static_cast<std::size_t>(data_end - data_begin.get());
        }

        std::size_t data_size() const {
            return size() * sizeof(pod_type);
        }

        void allocate(const std::size_t size) {
            data_begin.reset(new pod_type[size]);
            data_end = data_begin.get() + size;
        }

        pod_type &operator[](const std::size_t index) {
            return data_begin[index];
        }

        pod_type operator[](const std::size_t index) const {
            return data_begin[index];
        }


        char *data() {
            return reinterpret_cast<char *>(data_begin.get());
        }

        const char *data() const {
            return reinterpret_cast<const char *>(data_begin.get());
        }

        iterator begin() {
            return data_begin.get();
        }

        iterator end() {
            return data_end;
        }

        const_iterator begin() const {
            return data_begin.get();
        }

        const_iterator end() const {
            return data_end;
        }
    };
}
