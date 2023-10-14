#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <type_traits>
#include <algorithm>

using namespace std;

using Size_type = size_t;

template <typename T>
concept Size_convertible = std::is_convertible<T, Size_type>::value;

template <typename T>
struct vector_of_conbertible : std::false_type {};

template <Size_convertible T>
struct vector_of_conbertible<vector<T>> : std::true_type {};

template <typename T>
concept Size_vector = vector_of_conbertible<T>::value;

template <typename T>
concept Not_nullptr = !is_null_pointer<T>::value;

template <typename T>
concept Is_the_size = is_same<Size_type, T>::value;

template <typename T>
concept Not_exactly_size = std::is_convertible<T, Size_type>::value && !std::is_same<T, Size_type>::value;

class Size_conversion
{
    Size_conversion() = delete;

    template<Not_nullptr T, Size_type dim>
    friend class Grid;

    static void set_size(const vector<Size_type>& From, vector<Size_type>& To)
    {
        To = From;

        if (To.size() == 0)
            To.push_back(1);
    }
    
    template <Size_vector T>
    static vector<Size_type> convert_size(const T& original)
    {
        vector<Size_type> converted;
        converted.reserve(original.size());

        std::copy(original.begin(), original.end(), std::back_inserter(converted));

        return converted;
    }

    static vector<Size_type> convert_size(const vector<Size_type>& original) { return original; }
    
    template <Size_convertible... T>
    static vector<Size_type> convert_size(T... sizes) { return { static_cast<Size_type>(sizes)... }; }
};

template<Not_nullptr Value_type, Size_type dim=0>
class Grid final
{
private:
    vector<Size_type> m_size;
    std::unique_ptr<Value_type[]> m_data;

    template <typename... T>
    void instantiate(T... sizes)
    {
        Size_conversion::set_size(Size_conversion::convert_size(sizes...), this->m_size);

        if (size() > 0)
            this->m_data.reset(new Value_type[size()]);
    }

    static constexpr Size_type dim_subtract(const Size_type sub = 1)
    {
        if (dim > sub)
            return dim - sub;
        
        return 0;
    }
    
public:
    template<typename... Size_types>
    Grid(const Value_type* data, Size_types... sizes)
    {
        instantiate(sizes...);
        std::copy(data, data + size(), m_data.get());
    }
    
    template<typename... Size_types>
    Grid(const Value_type& data, Size_types... sizes)
    {
        instantiate(sizes...);
        std::fill(m_data.get(), m_data.get() + size(), data);
    }

    Grid()
    {
        instantiate(0);
    }
    
    template<Size_convertible... Size_types>
    Grid(Size_types... sizes) : Grid(Value_type(), sizes...) {}
    
    template<Size_vector T>
    Grid(const T& sizes) : Grid(Value_type(), sizes) {}
    
    Grid(const Grid& other) : Grid(other.m_data.get(), other.m_size) {}

    template<Not_nullptr Other_value_type, Size_type other_dim>
    friend class Grid;

    template<Not_nullptr Other_value_type, Size_type other_dim>
    Grid(const Grid<Other_value_type, other_dim>& other) : Grid(other.m_data.get(), other.m_size) {}
    
    Grid(Grid&& other) noexcept : m_size(other.m_size), m_data(std::move(other.m_data)) {}
    
    template<Not_nullptr Other_value_type, Size_type other_dim>
    Grid(Grid<Other_value_type, other_dim>&& other) noexcept : m_size(other.m_size), m_data(std::move(other.m_data)) {}

    Grid& operator=(const Grid& other)
    {
        Grid temp(other);
        std::swap(this->m_data, temp.m_data);
        std::swap(this->m_size, temp.m_size);

        return *this;
    }

    template<Not_nullptr Other_value_type, Size_type other_dim>
    Grid& operator=(const Grid<Other_value_type, other_dim>& other)
    {
        Grid temp(other);
        std::swap(this->m_data, temp.m_data);
        std::swap(this->m_size, temp.m_size);

        return *this;
    }

    Grid& operator=(Grid&& other) noexcept
    {
        Grid temp(other);
        std::swap(this->m_data, temp.m_data);
        std::swap(this->m_size, temp.m_size);

        return *this;
    }
    
    template<Not_nullptr Other_value_type, Size_type other_dim>
    Grid& operator=(Grid<Other_value_type, other_dim>&& other) noexcept
    {
        Grid temp(other);
        std::swap(this->m_data, temp.m_data);
        std::swap(this->m_size, temp.m_size);

        return *this;
    }

    ~Grid() {}

    Size_type get_size(Size_type n) const
    {
        if (n < m_size.size())
            return m_size[n];
        else if (n == 0)
            return 0;
        return 1;
    }

    Size_type size() const
    {
        size_t mult = 1;

        for (const auto& s : m_size)
            mult *= s;

        return mult;
    }

    template <Size_convertible... Idx_types>
    Grid<Value_type, dim_subtract(sizeof...(Idx_types))> operator()(Idx_types... idx) const
    {
        Grid<Value_type, dim> output = *this;
        for (const auto& i : { idx... })
            output = output[i];

        return output;
    }

    Grid<Value_type, dim_subtract()> operator[](Size_type idx) const
    {
        if (m_size.size() == 0 || idx >= size())
            return {};
        if (m_size.size() == 1)
            return m_data[idx];
        
        vector<Size_type> ouptut_size;
        ouptut_size.reserve(m_size.size() - 1);
        std::copy(m_size.begin() + 1, m_size.end(), back_inserter(ouptut_size));

        return { m_data.get() + size() / m_size[0] * idx, ouptut_size };
    }

    operator Value_type() const
    {
        if (size() == 0)
            return 0;

        return m_data[0];
    }

    void print()
    {
        for (auto it = m_data; it < m_data + size(); it++)
            cout << *it << ' ';
        cout << endl;
    }

    friend ostream& operator<<(ostream& stream, const Grid& grid) 
    {
        cout << "Grid of size ";
        auto g_size = grid.size();
        auto g_dim = grid.m_size.size();

        if (g_size == 0 || g_dim == 0)
            return stream << 0 << endl;

        string size_string = "";
        for (const auto& size : grid.m_size)
            size_string += to_string(size) + "x";
        size_string[size_string.length() - 1] = ' ';

        stream << size_string << endl;

        auto last_size = grid.m_size[g_dim-1];

        for (auto it = grid.m_data.get(); it != grid.m_data.get() + last_size; it++)
            stream << *it << ' ';
        stream << endl;

        if (g_size > last_size)
        {
            if (g_size > 2*last_size)
                if (g_size > 3*last_size)
                    stream << "..." << endl;
                else
                {
                    for (auto it = grid.m_data.get() + last_size; it != grid.m_data.get() + 2*last_size; it++)
                        stream << *it << ' ';
                    stream << endl;
                }

            for (auto it = grid.m_data.get() + g_size - last_size; it != grid.m_data.get() + g_size; it++)
                stream << *it << ' ';

            stream << endl;
        }

        return stream;
    }
};

int main()
{
    Grid<float, 3> const g3(1.0f, 2, 3, 4);
    cout << "Creating Grid<float, 3> g3(1.0f, 2, 3, 4) = " << endl << g3 << endl;

    assert(1.0f == g3(1, 1, 1));
    cout << "1.0f == g3(1, 1, 1) =" << endl << g3(1, 1, 1) << endl;

    Grid<float, 2> g2(2.0f, 2, 5);
    cout << "Creating Grid<float, 2> g2(2.0f, 2, 5) = " << endl << g2 << endl;

    assert(2.0f == g2(1, 1));
    cout << "2.0f == g2(1, 1) =" << endl << g2(1, 1) << endl;

    g2 = g3[1];
    cout << "g2 = g3[1] = " << endl << g3[1] << endl;

    assert(1.0f == g2(1, 1));
    cout << "1.0f == g2(1, 1) =" << endl << g2(1, 1) << endl;
    
    return 0;
}
