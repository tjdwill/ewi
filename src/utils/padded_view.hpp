// padded_view.hpp
#ifndef INCLUDED_PADDED_VIEW
#define INCLUDED_PADDED_VIEW

#ifndef INCLUDED_STD_VECTOR
#include <vector>
#define INCLUDED_STD_VECTOR
#endif

#ifndef INCLUDED_CPPERRORS
#include <cpperrors>
#define INCLUDED_CPPERRORS
#endif

namespace utils
{
    /// Pads a view of a given container, allowing us to
    /// treat the boundaries of the container like they are
    /// non-boundary components. Since this holds a
    /// reference to an object, ensure the referrent lives
    /// at least as long as this instantiated object.
    template<typename T>
    class PaddedView 
    {
        public:
            PaddedView(std::vector<T> const& container)
                : d_vec{container} 
            {
                if (container.empty())
                    throw err::Exception("Cannot pad an empty vector.");
            }
            
           inline auto size() const -> int { return ((int)d_vec.size()) + 2; }
           auto operator[] (int pos) const  -> T const&;
        private:
            std::vector<T> const& d_vec;
    };

    /// Access the padded view. 
    /// Implementation Detail:
    /// accessing the left or right padding area returns
    /// the first or last element in the real container.
    template<typename T>
    auto PaddedView<T>::operator[](int pos) const -> T const&
    {
        // assuming valid index range from 0 to this->size() - 1.
        if (pos == 0)
            return d_vec.front();
        else if (pos == size() - 1)
            return d_vec.back();
        else
            return d_vec[pos-1];
    }
} // namespace utils
#endif // INCLUDED_PADDED_VIEW
