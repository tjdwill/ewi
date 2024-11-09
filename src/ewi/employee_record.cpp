// employee_record.cpp
#include "employee_record.hpp"

#include <ranges>

namespace ewi
{
    auto EmployeeRecord::jobs() const { return std::views::keys(d_data); }
}

