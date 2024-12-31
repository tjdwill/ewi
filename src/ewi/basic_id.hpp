// basic_id.hpp
/*
* Copyright (C) 2024 Terrance Williams
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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
            auto operator<=>(BasicID const& rhs) const = default;
        private:
            std::string d_formal;
    };
    
} // namespace ewi
#endif // INCLUDED_EWI_BASIC_ID
