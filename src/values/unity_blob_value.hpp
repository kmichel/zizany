#pragma once

#include "../pod_vector.hpp"
#include "../unity_value.hpp"

namespace zizany {
    class unity_blob_value : public unity_value {
    public:
        pod_vector<char> data;

        unity_blob_value();

        virtual void print(json_writer &writer) const;

        virtual bool equals(const unity_value &value) const;

        virtual bool equals(const pod_vector<char> &value) const;
    };
}
