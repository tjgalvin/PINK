/**
 * @file   SelfOrganizingMapLib/Data.h
 * @date   Oct 12, 2018
 * @author Bernd Doser, HITS gGmbH
 */

#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <vector>

#ifdef __CUDACC__
    #include <thrust/device_vector.h>
#endif

namespace pink {

//! Primary template for generic Data
template <typename Layout, typename T>
class Data
{
public:

    typedef T ValueType;
    typedef Data<Layout, T> SelfType;
    typedef Layout LayoutType;
    typedef typename LayoutType::DimensionType DimensionType;

    /// Default construction
    Data()
     : layout{0}
    {}

    /// Construction without initialization
    Data(LayoutType const& layout)
     : layout(layout),
       data(layout.size())
    {}

    /// Construction and initialize all elements to value
    Data(LayoutType const& layout, T value)
     : layout(layout),
       data(layout.size(), value)
    {}

    /// Construction and copy data
    Data(LayoutType const& layout, std::vector<T> const& data)
     : layout(layout),
       data(data)
    {}

    /// Construction and move data
    Data(LayoutType const& layout, std::vector<T>&& data)
     : layout(layout),
       data(data)
    {}

    auto operator == (SelfType const& other) const
    {
        return layout == other.layout and
               data == other.data;
    }

    auto size() const { return data.size(); }

    /// Return the element
    auto operator [] (uint32_t position) -> T& { return data[position]; }
    auto operator [] (uint32_t position) const -> T const& { return data[position]; }

    auto operator [] (DimensionType const& position) -> T& { return data[layout.get_position(position)]; }
    auto operator [] (DimensionType const& position) const -> T const& { return data[layout.get_position(position)]; }

    auto get_data_pointer() { return &data[0]; }
    auto get_data_pointer() const { return &data[0]; }

    auto get_layout() -> LayoutType { return layout; }
    auto get_layout() const -> LayoutType const { return layout; }

    auto get_dimension() -> DimensionType { return layout.dimension; }
    auto get_dimension() const -> DimensionType const { return layout.dimension; }

#ifdef __CUDACC__
    /// Return device vector
    auto get_device_vector() { return d_data; }
    auto get_device_vector() const { return d_data; }

    /// Return device pointer
    auto get_device_pointer() { return &d_data[0]; }
    auto get_device_pointer() const { return &d_data[0]; }

    void update_host() { data = d_data; }
    void update_device() { d_data = data; }
#endif

private:

    template <typename A, typename B>
    friend void write(Data<A, B> const& data, std::string const& filename);

    LayoutType layout;

#ifdef __CUDACC__
    thrust::device_vector<T> d_data;
    thrust::host_vector<T> data;
#else
    std::vector<T> data;
#endif

};

} // namespace pink
