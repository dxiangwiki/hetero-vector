#include <iostream>
#include "hetero_vector/hetero_vector.h"


struct IntBox : ValueBoxBase
{
    int data;
    explicit IntBox(int v) : data(v) {}
    std::type_index data_type() const override
    {
        return std::type_index(typeid(int));
    }
    std::unique_ptr<ValueBoxBase> clone() const override
    {
        return std::make_unique<IntBox>(data);
    }
    int value() const { return data; }
};

struct DoubleBox : ValueBoxBase
{
    double data;
    explicit DoubleBox(double v) : data(v) {}
    std::type_index data_type() const override
    {
        return std::type_index(typeid(double));
    }
    std::unique_ptr<ValueBoxBase> clone() const override
    {
        return std::make_unique<DoubleBox>(data);
    }
    double value() const { return data; }
};

struct Point
{
    int x, y;
};
struct PointBox : ValueBoxBase
{
    Point data;
    PointBox(int x, int y) : data{x, y} {}
    std::type_index data_type() const override
    {
        return std::type_index(typeid(Point));
    }
    std::unique_ptr<ValueBoxBase> clone() const override
    {
        return std::make_unique<PointBox>(data.x, data.y);
    }
    Point value() const { return data; }
};

int main()
{
    HeteroVector hv;
    push_box<IntBox>(hv, 100);
    push_box<DoubleBox>(hv, 3.14);
    push_box<PointBox>(hv, 10, 20);

    auto& ib = hv.get<int, IntBox>(0);
    auto& db = hv.get<double, DoubleBox>(1);
    auto& pb = hv.get<Point, PointBox>(2);

    std::cout << "int = " << ib.value() << '\n';
    std::cout << "double = " << db.value() << '\n';
    std::cout << "point = (" << pb.value().x << "," << pb.value().y << ")\n";

    HeteroVector hv2 = hv;
    auto& ib2 = hv2.get<int, IntBox>(0);
    std::cout << "copy hv2 int = " << ib2.value() << '\n';
    return 0;
}
