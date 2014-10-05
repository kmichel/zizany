#pragma once

#include <functional>
#include <vector>

namespace zizany {
    class range_checker {
    public:
        typedef std::function<void(std::int64_t start, std::int64_t end)> unused_range_callback;
        typedef std::function<void(std::int64_t a_start, std::int64_t a_end, std::int64_t b_start, std::int64_t b_end)> overlapping_ranges_callback;
        struct range {
            std::int64_t start;
            std::int64_t end;
        };

        range_checker();

        range_checker(const range_checker &) = delete;

        range_checker &operator=(const range_checker &) = delete;

        void add_range(const std::int64_t start, const std::int64_t end);

        void check(unused_range_callback unused_range_callback, overlapping_ranges_callback overlapping_ranges_callback);

    private:
        std::vector<range> ranges;
    };
}
