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
    void set_size(double width, double height) { _width = width; _height = height; }
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
struct ShapeHolderRef;

template<typename T>
struct ShapeHolderRef<std::reference_wrapper<T>>
    : adobe::optimized_storage_type<std::reference_wrapper<T>, ShapeInternal>::type
{
    using base_t = typename adobe::optimized_storage_type<
        std::reference_wrapper<T>,
        ShapeInternal>::type;

    ShapeHolderRef(std::reference_wrapper<T> x) : base_t{std::move(x)} { }

    ShapeHolderRef(adobe::move_from<ShapeHolderRef> x)
        : base_t{adobe::move_from<base_t>{x.source}} { }

    virtual double get_area_internal() const override
    { return this->get().get().get_area(); }

    virtual double get_perimeter_internal() const override
    { return this->get().get().get_perimeter(); }
};

template<>
struct ShapeHolderRef<std::reference_wrapper<Circle>>
    : adobe::optimized_storage_type<std::reference_wrapper<Circle>, ShapeInternal>::type
{
    using base_t =
          typename adobe::optimized_storage_type<std::reference_wrapper<Circle>, ShapeInternal>::type;

    ShapeHolderRef(std::reference_wrapper<Circle> x) : base_t{std::move(x)} { }

    ShapeHolderRef(adobe::move_from<ShapeHolderRef> x)
        : base_t{adobe::move_from<base_t>{x.source}} { }

    virtual double get_area_internal() const override
    { return this->get().get().compute_area(); }

    virtual double get_perimeter_internal() const override
    { return this->get().get().compute_perimeter(); }
};

struct ErasedShapeRef : adobe::poly_base<ShapeInternal, ShapeHolderRef>
{
    using base_t = adobe::poly_base<ShapeInternal, ShapeHolderRef>;

    using base_t::base_t;

    ErasedShapeRef(adobe::move_from<ErasedShapeRef> x)
        : base_t(adobe::move_from<base_t>(x.source)) { }

    double get_area() const
    { return this->interface_ref().get_area_internal(); }

    double get_perimeter() const
    { return this->interface_ref().get_perimeter_internal(); }
};

using ShapeRef = adobe::poly<ErasedShapeRef>;

#include <vector>
#include <iostream>

int main()
{
    Rectangle r{{1.0, 2.0}, 5.0, 6.0};

    ShapeRef s{std::ref(r)};

    std::cout << s.get_area() << std::endl;

    r.set_size(4, 2);

    std::cout << s.get_area() << std::endl;
}
