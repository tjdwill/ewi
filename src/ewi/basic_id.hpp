// basic_id.hpp
#ifndef INCLUDED_EWI_BASIC_ID
#define INCLUDED_EWI_BASIC_ID

#ifndef INCLUDED_STD_STRING
#include <string>
#define INCLUDED_STD_STRING
#endif

namespace ewi
{
    /// A type representing the basic form of identification.  Has an official, formal
    /// component to preclude ambiguity and an informal shortname for convenience.
    class BasicID
    {
        public:
            BasicID() = delete;
            BasicID(std::string const& formal) noexcept
                : d_formal{ std::move(formal) } {}
            // Accessors

            /// Get the unambiguous ID
            inline auto formal() const noexcept -> std::string const& { return d_formal; }
        private:
            std::string d_formal;
    };
} // namespace ewi
#endif // INCLUDED_EWI_BASIC_ID
