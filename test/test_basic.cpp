#include <iostream>
#include <cassert>
#include <stdexcept>
#include "hetero_vector/hetero_vector.h"

struct IntBox : ValueBoxBase
{
    int data;
    explicit IntBox(int v) : data(v) {}
    std::type_index data_type() const override { return typeid(int); }
    std::unique_ptr<ValueBoxBase> clone() const override { return std::make_unique<IntBox>(data); }
    int value() const { return data; }
};

struct DoubleBox : ValueBoxBase
{
    double data;
    explicit DoubleBox(double v) : data(v) {}
    std::type_index data_type() const override { return typeid(double); }
    std::unique_ptr<ValueBoxBase> clone() const override { return std::make_unique<DoubleBox>(data); }
    double value() const { return data; }
};

int main()
{
    HeteroVector hv;
    push_box<IntBox>(hv, 99);
    push_box<DoubleBox>(hv, 3.14);

    assert(hv.size() == 2);

    auto& i = hv.get<int, IntBox>(0);
    assert(i.value() == 99);

    auto& d = hv.get<double, DoubleBox>(1);
    assert(d.value() == 3.14);

    // 测试拷贝
    HeteroVector hv2 = hv;
    auto& i2 = hv2.get<int, IntBox>(0);
    assert(i2.value() == 99);

    // 测试类型不匹配抛异常
    bool ok = false;
    try {
        hv.get<double, DoubleBox>(0);
    } catch (...) {
        ok = true;
    }
    assert(ok);

    std::cout << "all test passed\n";
    return 0;
}
