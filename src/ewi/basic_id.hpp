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
            BasicID(std::string const& formal, std::string const& informal) noexcept
                : d_formal{ std::move(formal) }, d_informal{ std::move(informal) } {}
            BasicID(std::string const& formal) noexcept 
            {
                BasicID(formal, "");
            }

            // Accessors

            /// Get the unambiguous ID
            inline auto formal() const noexcept -> std::string const& { return d_formal; }
            /// Get the informal entity shortname
            inline auto informal() const noexcept -> std::string const& { return d_informal; }
            /// Update the entity shortname
            inline void informal(std::string const& shortname) noexcept { d_informal = shortname; }
        private:
            std::string d_formal;
            std::string d_informal {};
    };
} // namespace ewi
#endif // INCLUDED_EWI_BASIC_ID
