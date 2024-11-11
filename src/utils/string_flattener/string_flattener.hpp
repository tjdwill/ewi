// string_flattener.hpp

// A utility that compresses a given stream of textual data to one line.
#ifndef INCLUDED_STRING_FLATTENER
#define INCLUDED_STRING_FLATTENER

#ifndef INCLUDED_STD_STRING
#include <string>
#define INCLUDED_STD_STRING
#endif 

namespace utils {
    struct StringFlattener
    {
        /// [From what I've read](https://stackoverflow.com/questions/72005354/demystifying-the-newline-character-again),
        /// C++ compilers handle conversion between newline character types for text streams
        /// (so, streams not in binary mode). I am taking this to mean that I don't need to do
        /// anything special in order to have cross-platform uniformity?

        /// The character used for newline replacement. 
        static constexpr auto SUBTITUTION_STR {'\u0001'};
        static constexpr auto NEWLINE {'\n'};
        /// Given a potentially multi-line string `str`, this function replaces each
        /// newline symbol `\n` with a substitution character. The substitution character
        /// is intended to be a byte sequence that is rarely used in textual data, if ever.
        /// If a string occupies a single line, this function should return an unaltered
        /// copy of said string. 
        static auto flatten(std::string str) -> std::string;

        /// Given a previously-flattened string `str`, expand it, replacing the
        /// substitution character with a newline sequence. If the string was not
        /// previously compressed, this function returns a copy of the input string.
        static auto expand(std::string str) -> std::string;
    };
} // namespace utils
#endif // INCLUDED_STRING_FLATTENER
