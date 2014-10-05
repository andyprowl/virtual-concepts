/* =============================================================================== */
/* Dynamic Generic Programming                                                     */
/* =============================================================================== */
/* Test project using Pyry Jahkola's Poly library (https://github.com/pyrtsa/poly) */
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

/* =============================================================================== */
/* NOTICE: Circle has functions called compute_area() and compute_perimeter()      */
/*         rather than get_area() and get_perimeter()                              */
/* =============================================================================== */
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
private:
    Point _center;
    double _width;
    double _height;
};

#include <poly/interface.hpp>

POLY_CALLABLE(get_area);
POLY_CALLABLE(get_perimeter);

using Shape = poly::interface<
    double(get_area_, poly::self const&),
    double(get_perimeter_, poly::self const&)>;

template<typename T>
double call(get_area_, T const &x)
{
    return x.get_area();
}

double call(get_area_, Circle const &x)
{
    return x.compute_area();
}

template<typename T>
double call(get_perimeter_, T const &x)
{
    return x.get_perimeter();
}

double call(get_perimeter_, Circle const &x)
{
    return x.compute_perimeter();
}

#include <iostream>
#include <string>
#include <vector>

void print_areas(std::vector<Shape> const& v)
{
    for (auto const& s : v)
    {
        std::cout << "Area: " << get_area(s) << std::endl;
    }
}

void working_example()
{
    auto r = Rectangle{{1.0, 2.0}, 5.0, 6.0};

    auto c = Circle{{0.0, 0.0}, 42.0};

    std::vector<Shape> v{r, c};

    print_areas(v);
}

int main()
{
    working_example();
}
