#include "member_path.hpp"
#include "../json_writer.hpp"

namespace zizany {
    member_path::member_path()
            : components() {
    }

    void
    member_path::push(const std::string &component) {
        components.push_back(component);
    }

    void
    member_path::pop() {
        components.pop_back();
    }

    member_path
    member_path::join(const std::string &component) const {
        member_path child_path(*this);
        child_path.components.push_back(component);
        return child_path;
    }

    void
    member_path::print(json_writer &writer) const {
        writer.start_array(/*inline:*/true);
        for (const std::string &component : components)
            writer.add_string(component);
        writer.end_array();
    }
}
