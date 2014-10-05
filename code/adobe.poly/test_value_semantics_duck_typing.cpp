/* =============================================================================== */
/* Dynamic Generic Programming                                                     */
/* =============================================================================== */
/* Test project using the Adobe.Poly library                                       */
/* (http://sourceforge.net/projects/adobe-source/)                                 */
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

#include <adobe/poly.hpp>

struct ShapeInternal : adobe::poly_copyable_interface
{
    virtual double get_area_internal() const = 0;
    virtual double get_perimeter_internal() const = 0;
};

template<typename T>
struct ShapeHolder : adobe::optimized_storage_type<T, ShapeInternal>::type
{
    using base_t = typename adobe::optimized_storage_type<T, ShapeInternal>::type;

    ShapeHolder(T x) : base_t(std::move(x)) { }

    ShapeHolder(adobe::move_from<ShapeHolder> x)
        : base_t{adobe::move_from<base_t>{x.source}} { }

    virtual double get_area_internal() const override
    { return this->get().get_area(); }

    virtual double get_perimeter_internal() const override
    { return this->get().get_perimeter(); }
};

struct ErasedShape : adobe::poly_base<ShapeInternal, ShapeHolder>
{
    using base_t = adobe::poly_base<ShapeInternal, ShapeHolder>;

    using base_t::base_t; // INHERIT BASE CLASS'S CONSTRUCTORS

    ErasedShape(adobe::move_from<ErasedShape> x)
        : base_t(adobe::move_from<base_t>(x.source)) { }

    double get_area() const
    { return this->interface_ref().get_area_internal(); }

    double get_perimeter() const
    { return this->interface_ref().get_perimeter_internal(); }
};

using Shape = adobe::poly<ErasedShape>;

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
