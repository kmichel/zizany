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

sed -ie 's/extern char\*	g_ffmt    ANSI((char\*, float\*,  int, size_t));/extern char\*	g_ffmt    ANSI((char\*, const float\*,  int, size_t));/' $1
sed -ie 's/extern char\*	g_dfmt    ANSI((char\*, double\*, int, size_t));/extern char\*	g_dfmt    ANSI((char\*, const double\*, int, size_t));/' $1

sed -ie 's/g_ffmt(char \*buf, float \*f, int ndig, size_t bufsize)/g_ffmt(char \*buf, const float \*f, int ndig, size_t bufsize)/' $2
sed -ie 's/g_dfmt(char \*buf, double \*d, int ndig, size_t bufsize)/g_dfmt(char \*buf, const double \*d, int ndig, size_t bufsize)/' $3
