#include "unity_file.hpp"

#include "unity_asset.hpp"
#include "unity_file_reference.hpp"
#include "unity_preview.hpp"
#include "unity_type.hpp"
#include "values/unity_value.hpp"

namespace zizany {
    unity_file::unity_file()
            : types(),
              assets(),
              file_references(),
              previews(),
              magic_int_1(),
              magic_int_2(),
              magic_int_3(),
              unity_version(),
              artifact_data() {
    }
}
