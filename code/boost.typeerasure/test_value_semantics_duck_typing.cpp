/* =============================================================================== */
/* Dynamic Generic Programming                                                     */
/* =============================================================================== */
/* Test project using the Boost.TypeErasure library                                */
/* (http://www.boost.org/doc/libs/1_56_0/doc/html/boost_typeerasure.html)          */
/*                                                                                 */
/* Tested with Boost 1.56                                                          */
/* =============================================================================== */
/* Author: Andy Prowl                                                              */
/* =============================================================================== */

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
    double get_area() const { return _radius * _radius * 3.14159; }
    double get_perimeter() const { return _radius * 2 * 3.14159; }
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
private:
    Point _center;
    double _width;
    double _height;
};

#include <boost/type_erasure/any.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/member.hpp>

namespace erasure = boost::type_erasure;

BOOST_TYPE_ERASURE_MEMBER((has_get_area), get_area)
BOOST_TYPE_ERASURE_MEMBER((has_get_perimeter), get_perimeter)

using ShapeRequirements = boost::mpl::vector<
    erasure::copy_constructible<>,
    has_get_area<double(), erasure::_self const>,
    has_get_perimeter<double(), erasure::_self const>,
    erasure::relaxed>;

using Shape = erasure::any<ShapeRequirements>;

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
    auto r = Rectangle{{1.0, 2.0}, 5.0, 6.0};

    auto c = Circle{{0.0, 0.0}, 42.0};

    std::vector<Shape> v{r, c};

    print_areas(v);
}
