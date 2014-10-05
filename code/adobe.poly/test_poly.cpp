#include <adobe/poly.hpp>
#include <adobe/poly_regular.hpp>

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

struct ShapeInternal : adobe::poly_copyable_interface
{
    virtual double get_area_internal() const = 0;
    virtual double get_perimeter_internal() const = 0;
    virtual void scale_internal(double factor) = 0;
    virtual bool equals(ShapeInternal const& rhs) const = 0;
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

    virtual void scale_internal(double factor) override
    { this->get().scale(factor); }

    virtual bool equals(ShapeInternal const& rhs) const override
    { return this->type_info() == rhs.type_info() &&
             this->get() == *static_cast<const T*>(rhs.cast()); }
};

template<>
struct ShapeHolder<Circle> : adobe::optimized_storage_type<Circle, ShapeInternal>::type
{
    using base_t = typename adobe::optimized_storage_type<Circle, ShapeInternal>::type;

    ShapeHolder(Circle x) : base_t(std::move(x))
    { }

    ShapeHolder(adobe::move_from<ShapeHolder> x)
    : base_t{adobe::move_from<base_t>{x.source}}
    { }

    virtual double get_area_internal() const override
    { return this->get().compute_area(); }

    virtual double get_perimeter_internal() const override
    { return this->get().compute_perimeter(); }

    virtual void scale_internal(double factor) override
    { this->get().scale(factor); }

    virtual bool equals(ShapeInternal const& rhs) const override
    { return this->type_info() == rhs.type_info() &&
             this->get() == *static_cast<const Circle*>(rhs.cast()); }
};

template<typename T>
struct ShapeHolderRef;

template<typename T>
struct ShapeHolderRef<std::reference_wrapper<T>>
    : adobe::optimized_storage_type<std::reference_wrapper<T>, ShapeInternal>::type
{
    using base_t =
          typename adobe::optimized_storage_type<std::reference_wrapper<T>, ShapeInternal>::type;

    ShapeHolderRef(std::reference_wrapper<T> x) : base_t{std::move(x)} { }

    ShapeHolderRef(adobe::move_from<ShapeHolderRef> x)
        : base_t{adobe::move_from<base_t>{x.source}} { }

    virtual double get_area_internal() const override
    { return this->get().get().get_area(); }

    virtual double get_perimeter_internal() const override
    { return this->get().get().get_perimeter(); }

    virtual void scale_internal(double factor) override
    { this->get().get().scale(factor); }

    virtual bool equals(ShapeInternal const& rhs) const override
    { return this->type_info() == rhs.type_info() &&
             this->get().get() == *static_cast<const T*>(rhs.cast()); }
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

    virtual void scale_internal(double factor) override
    { this->get().get().scale(factor); }

    virtual bool equals(ShapeInternal const& rhs) const override
    { return this->type_info() == rhs.type_info() &&
             (this->get().get()) == *static_cast<const Circle*>(rhs.cast()); }
};

struct ErasedShape : adobe::poly_base<ShapeInternal, ShapeHolder>
{
    using base_t = adobe::poly_base<ShapeInternal, ShapeHolder>;

    using base_t::base_t; // inherit constructors

    ErasedShape(adobe::move_from<ErasedShape> x)
    : base_t(adobe::move_from<base_t>(x.source))
    { }

    double get_area() const
    { return this->interface_ref().get_area_internal(); }

    double get_perimeter() const
    { return this->interface_ref().get_perimeter_internal(); }

    virtual void scale(double factor)
    { this->interface_ref().scale_internal(factor); }
};

struct ErasedShapeRef : adobe::poly_base<ShapeInternal, ShapeHolderRef>
{
    using base_t = adobe::poly_base<ShapeInternal, ShapeHolderRef>;

    template<typename T>
    ErasedShapeRef(T& x) : base_t{std::ref(x)} { }

    ErasedShapeRef(const ErasedShapeRef& x) : base_t{x} { }

    ErasedShapeRef(adobe::move_from<ErasedShapeRef> x)
        : base_t(adobe::move_from<base_t>(x.source)) { }

    double get_area() const
    { return this->interface_ref().get_area_internal(); }

    double get_perimeter() const
    { return this->interface_ref().get_perimeter_internal(); }

    virtual void scale(double factor)
    { this->interface_ref().scale_internal(factor); }
};

using Shape = adobe::poly<ErasedShape>;

using ShapeRef = adobe::poly<ErasedShapeRef>;

#include <vector>
#include <iostream>

void print_areas(std::vector<ShapeRef> const& v)
{
    for (auto const& s : v)
    {
        std::cout << "Area: " << s.get_area() << std::endl;
    }
}

int main()
{
    Rectangle r{{1.0, 2.0}, 5.0, 6.0};

    ShapeRef s{r};

    auto c = Circle{{0.0, 0.0}, 42.0};

    auto s2 = ShapeRef{c};

    std::cout << s.get_area() << std::endl; // PRINTS 30

    r.set_size(4, 2);

    std::cout << s.get_area() << std::endl; // PRINTS 8

    std::vector<ShapeRef> v{r, c};

    c.set_radius(1);

    print_areas(v);

    Rectangle r2{{1.0, 2.0}, 5.0, 6.0};

    ShapeRef r2_erased{r2};

    std::cout << (v[0] == r2_erased) << std::endl;
    std::cout << (v[0] != r2_erased) << std::endl;
}
