#ifndef HETERO_VECTOR_H
#define HETERO_VECTOR_H

#include <vector>
#include <memory>
#include <typeindex>
#include <stdexcept>

// 基类，供用户继承
struct ValueBoxBase
{
    virtual ~ValueBoxBase() = default;
    virtual std::type_index data_type() const = 0;
    virtual std::unique_ptr<ValueBoxBase> clone() const = 0;
};

class HeteroVector
{
private:
    std::vector<std::unique_ptr<ValueBoxBase>> storage_;

public:
    HeteroVector();
    HeteroVector(const HeteroVector& other);
    HeteroVector(HeteroVector&&) noexcept;
    HeteroVector& operator=(const HeteroVector& other);
    HeteroVector& operator=(HeteroVector&&) noexcept;
    ~HeteroVector();

    void push(std::unique_ptr<ValueBoxBase> box);

    // 模板必须留在头（.h），编译器实例化需要看到代码
    template<typename T, typename BoxT>
    BoxT& get(size_t idx)
    {
        if (idx >= storage_.size())
        {
            throw std::out_of_range("HeteroVector::get index out of range");
        }
        ValueBoxBase* raw = storage_[idx].get();
        if (raw->data_type() != std::type_index(typeid(T)))
        {
            throw std::runtime_error("HeteroVector::get data type mismatch");
        }
        return dynamic_cast<BoxT&>(*raw);
    }

    template<typename T, typename BoxT>
    const BoxT& get(size_t idx) const
    {
        if (idx >= storage_.size())
        {
            throw std::out_of_range("HeteroVector::get index out of range");
        }
        const ValueBoxBase* raw = storage_[idx].get();
        if (raw->data_type() != std::type_index(typeid(T)))
        {
            throw std::runtime_error("HeteroVector::get data type mismatch");
        }
        return dynamic_cast<const BoxT&>(*raw);
    }

    size_t size() const noexcept;
    void clear();
};

template<typename BoxT, typename... Args>
void push_box(HeteroVector& hv, Args&&... args)
{
    hv.push(std::make_unique<BoxT>(std::forward<Args>(args)...));
}

#endif
