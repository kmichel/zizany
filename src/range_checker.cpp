#include "range_checker.hpp"

#include <algorithm>
#include <set>
#include <stdexcept>

struct event {
    std::size_t range_id;
    std::int64_t position;
    bool is_start;
};

namespace zizany {
    range_checker::range_checker()
            : ranges() {
    }

    void
    range_checker::add_range(const std::int64_t start, const std::int64_t end) {
        if (start > end)
            throw std::runtime_error("invalid range");
        ranges.push_back({start, end});
    }

    void
    range_checker::check(
            unused_range_callback unused_range_callback,
            overlapping_ranges_callback overlapping_ranges_callback) {
        std::vector<event> events;
        for (std::size_t index = 0; index < ranges.size(); ++index) {
            const range &range(ranges[index]);
            if (range.start != range.end) {
                events.push_back({index, range.start, true});
                events.push_back({index, range.end, false});
            }
        }
        std::sort(events.begin(), events.end(), [](const event &a, const event &b) {
            if (a.position == b.position) {
                if (a.is_start == b.is_start)
                    return a.range_id < b.range_id;
                return !a.is_start;
            }
            return a.position < b.position;
        });
        std::set<std::size_t> active_ranges;
        std::int64_t unused_area_start(0);
        for (std::size_t index = 0; index < events.size(); ++index) {
            const event &event(events[index]);
            if (event.is_start) {
                if (active_ranges.empty() && unused_area_start != event.position)
                    unused_range_callback(unused_area_start, event.position);
                const range &added_range(ranges.at(event.range_id));
                for (auto it = active_ranges.begin(); it != active_ranges.end(); ++it) {
                    const range &active_range(ranges.at(*it));
                    overlapping_ranges_callback(active_range.start, active_range.end, added_range.start, added_range.end);
                }
                active_ranges.insert(event.range_id);
            } else {
                active_ranges.erase(event.range_id);
                if (active_ranges.empty())
                    unused_area_start = event.position;
            }
        }
    }
}
