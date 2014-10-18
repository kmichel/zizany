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

    stream_parser::stream_parser(file_stream &stream_, const endianness endianness_, range_checker &checker_)
            : stream(stream_),
              must_swap_bytes((endianness_ == endianness::little_endian) ^ is_little_endian()),
              last_seek(tell()),
              checker(checker_) {
    }

    stream_parser::~stream_parser() {
        checker.add_range(last_seek, tell());
    }

    std::string
    stream_parser::parse_string() {
        std::string string;
        while (true) {
            const int c(stream.getc());
            if (c == 0)
                return string;
            else
                string += static_cast<char>(c);
        }
    }

    void
    stream_parser::align(const int stride) {
        const int excess(static_cast<int>(tell() % stride));
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
        return stream.tell();
    }

    void
    stream_parser::seek(std::int64_t position) {
        checker.add_range(last_seek, tell());
        stream.seek(position, SEEK_SET);
        last_seek = position;
    }

    void
    stream_parser::seek_from_end(std::int64_t position) {
        checker.add_range(last_seek, tell());
        stream.seek(position, SEEK_END);
        last_seek = tell();
    }
}
