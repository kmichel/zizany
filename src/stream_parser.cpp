#include "stream_parser.hpp"

#include "parser_exception.hpp"

namespace zizany {

    static
    bool is_little_endian() {
        union {
            std::int32_t value;
            char bytes[sizeof(std::int32_t)];
        } endianness_detector{1};
        return endianness_detector.bytes[0] == 1;
    }

    stream_parser::stream_parser(std::istream &stream_, const endianness endianness, range_checker &checker_)
            : stream(stream_),
              must_swap_bytes((endianness == endianness::little_endian) ^ is_little_endian()),
              last_seek(tell()),
              checker(checker_) {
        stream.exceptions(std::istream::badbit | std::istream::eofbit | std::istream::failbit);
    }

    stream_parser::~stream_parser() {
        checker.add_range(last_seek, tell());
    }

    std::string
    stream_parser::parse_string() {
        std::string string;
        std::getline(stream, string, '\0');
        return string;
    }

    void
    stream_parser::align(const int stride) {
        const int excess(static_cast<int>(stream.tellg() % stride));
        if (excess != 0) {
            for (int i = excess; i < stride; ++i) {
                const char value(parse<char>());
                if (value != 0)
                    throw parser_exception("padding should be filled with zeroes");
            }
        }
    }

    std::int64_t
    stream_parser::tell() const {
        return stream.tellg();
    }

    void
    stream_parser::seek(std::int64_t position) {
        checker.add_range(last_seek, tell());
        stream.seekg(position, std::ios_base::seekdir::beg);
        last_seek = position;
    }

    void
    stream_parser::seek_from_end(std::int64_t position) {
        checker.add_range(last_seek, tell());
        stream.seekg(position, std::ios_base::seekdir::end);
        last_seek = tell();
    }
}
