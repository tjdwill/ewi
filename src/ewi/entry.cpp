// entry.cpp
#include "entry.hpp"
#include <vector>
#include <iostream>
#include <format>

#include <utils/string_flattener/string_flattener.hpp>

namespace ewi {
    
    auto operator== (Entry const& a, Entry const& b) noexcept -> bool
    {
        if (a.date() != b.date())
            return false;
        if (a.notes() != b.notes())
            return false;
        // vector comp
        auto a_vec = a.metrics();
        auto b_vec = b.metrics();
        if (a_vec.size() != b_vec.size())
            return false;
        return a_vec == b_vec;
    }

    auto operator<< (std::ostream& os, Entry const& e) noexcept -> std::ostream&
    {
        os << std::format("{:%F}", e.date()) << ' '
           << utils::StringFlattener::flatten(e.notes()) << ' ';
    
        for (double d: e.metrics())
            os << d << " "; 

        return os;
    }
}
