#include "entry.hpp"
#include <vector>
#include <iostream>
#include <format>

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
        os << "{\n";
        os << "\tDate: " << std::format("{:%F}", e.date()) << '\n'
           << "\tMetrics: [ ";
    
        for (double d: e.metrics())
            os << d << " "; 
        os << ']'<< '\n'
           << "\tNotes:\n\t\"" << e.notes() << "\"\n}";

        return os;
    }
}
