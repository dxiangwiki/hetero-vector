#ifndef HETERO_VECTOR_HPP
#define HETERO_VECTOR_HPP

#include <vector>
#include <memory>
#include <typeindex>
#include <stdexcept>

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
    HeteroVector() = default;
    HeteroVector(const HeteroVector& other)
    {
        storage_.reserve(other.storage_.size());
        for (const auto& ptr : other.storage_)
        {
            storage_.push_back(ptr->clone());
        }
    }
    HeteroVector(HeteroVector&&) noexcept = default;
    HeteroVector& operator=(const HeteroVector& other)
    {
        if (this != &other)
        {
            storage_.clear();
            storage_.reserve(other.storage_.size());
            for (const auto& ptr : other.storage_)
            {
                storage_.push_back(ptr->clone());
            }
        }
        return *this;
    }
    HeteroVector& operator=(HeteroVector&&) noexcept = default;
    ~HeteroVector() = default;

    void push(std::unique_ptr<ValueBoxBase> box)
    {
        storage_.push_back(std::move(box));
    }

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

    size_t size() const noexcept
    {
        return storage_.size();
    }

    void clear()
    {
        storage_.clear();
    }
};

template<typename BoxT, typename... Args>
void push_box(HeteroVector& hv, Args&&... args)
{
    hv.push(std::make_unique<BoxT>(std::forward<Args>(args)...));
}

#endif
