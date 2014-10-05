struct Point
{
    Point(double x, double y) : x{x}, y{y} { }
    friend bool operator == (Point const& lhs, Point const& rhs)
    { return (lhs.x == rhs.x) && (lhs.y == rhs.y); }
    double x = 0;
    double y = 0;
};

struct Circle
{
    Circle(Point const& center, double radius)
        : _center{center}, _radius{radius} { }
    double compute_area() const { return _radius * _radius * 3.14159; }
    double compute_perimeter() const { return _radius * 2 * 3.14159; }
    void set_radius(double r) { _radius = r; }
    void scale(double factor) { _radius *= factor; }
    friend bool operator == (Circle const& lhs, Circle const& rhs)
    { return (lhs._center == rhs._center) && (lhs._radius == rhs._radius); }
    friend bool operator != (Circle const& lhs, Circle const& rhs)
    { return !(lhs == rhs); }
private:
    Point _center;
    double _radius;
};

struct Rectangle
{
    Rectangle(Point const& center, double width, double height)
        : _center{center}, _width{width}, _height{height} { }
    double get_area() const { return _width * _height; }
    double get_perimeter() const { return (_width + _height) * 2; }
    void set_size(double width, double height) { _width = width; _height = height; }
    void scale(double factor) { _width *= factor; _height *= factor; }
    friend bool operator == (Rectangle const& lhs, Rectangle const& rhs)
    { return (lhs._center == rhs._center) && (lhs._width == rhs._width) && (lhs._height == rhs._height); }
    friend bool operator != (Rectangle const& lhs, Rectangle const& rhs)
    { return !(lhs == rhs); }
private:
    Point _center;
    double _width;
    double _height;
};

#include <boost/type_erasure/any.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/member.hpp>

template<class T>
struct has_get_area
{
    static double apply(T const& x) { return x.get_area(); }
};

template<>
struct has_get_area<Circle>
{
    static double apply(Circle const& x) { return x.compute_area(); }
};

template<class T>
struct has_get_perimeter
{
    static double apply(T const& x) { return x.get_perimeter(); }
};

template<>
struct has_get_perimeter<Circle>
{
    static double apply(Circle const& x) { return x.compute_perimeter(); }
};

template<class T>
struct has_scale
{
    static void apply(T& x, double factor) { x.scale(factor); }
};

namespace boost { namespace type_erasure
{
    template<class C, class Base>
    struct concept_interface<has_get_area<C>, Base, C> : Base
    {
        double get_area() const
        { return call(has_get_area<C>(), *this); }
    };

    template<class C, class Base>
    struct concept_interface<has_get_perimeter<C>, Base, C> : Base
    {
        double get_perimeter() const
        { return call(has_get_perimeter<C>(), *this); }
    };

    template<class C, class Base>
    struct concept_interface<has_scale<C>, Base, C> : Base
    {
        void scale(double arg)
        { call(has_scale<C>(), *this, arg); }
    };
} }

namespace erasure = boost::type_erasure;

/*
BOOST_TYPE_ERASURE_MEMBER((has_get_area), get_area)
BOOST_TYPE_ERASURE_MEMBER((has_get_perimeter), get_perimeter)

using ShapeRequirements = boost::mpl::vector<
    erasure::copy_constructible<>,
    erasure::equality_comparable<>,
    has_get_area<double(), erasure::_self const>, // GOOD LUCK UNDERSTANDING THE ERROR WHEN OMITTING THE SECOND ARGUMENT
    has_get_perimeter<double(), erasure::_self const>, // GOOD LUCK UNDERSTANDING THE ERROR WHEN OMITTING THE SECOND ARGUMENT
    erasure::relaxed>;
*/

using ShapeRequirements = boost::mpl::vector<
    boost::type_erasure::copy_constructible<>,
    boost::type_erasure::equality_comparable<>,
    has_get_area<boost::type_erasure::_self>,
    has_get_perimeter<boost::type_erasure::_self>,
    has_scale<boost::type_erasure::_self>,
    boost::type_erasure::relaxed>;

using Shape = erasure::any<ShapeRequirements>;
using ShapeRef = erasure::any<ShapeRequirements, erasure::_self&>;
using ShapeRefRef = erasure::any<ShapeRequirements, erasure::_self&&>;

#include <vector>
#include <iostream>

void print_areas(std::vector<Shape> const& v)
{
    for (auto const& s : v)
    {
        std::cout << "Area: " << s.get_area() << std::endl;
    }
}

int main()
{
    namespace te = boost::type_erasure;

    auto r = Rectangle{{1.0, 2.0}, 5.0, 6.0};

    auto s = ShapeRef{r};

    r.set_size(1., 2.);

    r.scale(5.);

    std::cout << s.get_area() << std::endl;

    std::cout << s.get_perimeter() << std::endl;

    auto c = Circle{{0.0, 0.0}, 42.0};

    std::vector<Shape> v{r, c};

    c.set_radius(1);

    c.scale(5);

    print_areas(v);

    Rectangle r2{{1.0, 2.0}, 5.0, 6.0};

    Shape r2_erased{r};

    std::cout << (v[0] == r2_erased) << std::endl;
    std::cout << (v[0] != r2_erased) << std::endl;

    Shape s2{r};
    ShapeRef ls = s; // WORKS
    ShapeRefRef rs = std::move(r); // WORKS
    ShapeRefRef rs2 = std::move(s2); // DOES NOT WORK!
}
