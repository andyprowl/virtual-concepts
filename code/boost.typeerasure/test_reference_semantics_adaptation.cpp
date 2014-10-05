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
    double compute_area() const { return _radius * _radius * 3.14159; }
    double compute_perimeter() const { return _radius * 2 * 3.14159; }
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
private:
    Point _center;
    double _width;
    double _height;
};

#include <boost/type_erasure/any.hpp>
#include <boost/mpl/vector.hpp>
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
} }

namespace erasure = boost::type_erasure;

using ShapeRequirements = boost::mpl::vector<
    erasure::copy_constructible<>,
    has_get_area<erasure::_self>,
    has_get_perimeter<erasure::_self>,
    erasure::relaxed>;

using ShapeRef = erasure::any<ShapeRequirements, erasure::_self&>;

#include <vector>
#include <iostream>

int main()
{
    Rectangle r{{1.0, 2.0}, 5.0, 6.0};

    ShapeRef s{r};

    std::cout << s.get_area() << std::endl;

    r.set_size(4, 2);

    std::cout << s.get_area() << std::endl;
}
