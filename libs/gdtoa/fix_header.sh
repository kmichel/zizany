set -e

sed -ie 's/extern float  strtof ANSI((CONST char \*, char \*\*));/extern float  strtof ANSI((CONST char \*, char \*\*)) \
#ifdef __cplusplus\
#if __cplusplus >= 201103L\
noexcept\
#else\
throw()\
#endif\
#endif\
;/' $1

sed -ie 's/extern double strtod ANSI((CONST char \*, char \*\*));/extern double strtod ANSI((CONST char \*, char \*\*)) \
#ifdef __cplusplus\
#if __cplusplus >= 201103L\
noexcept\
#else\
throw()\
#endif\
#endif\
;/' $1
