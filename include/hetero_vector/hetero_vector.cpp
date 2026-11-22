#include "hetero_vector.h"

HeteroVector::HeteroVector() = default;

HeteroVector::HeteroVector(const HeteroVector& other)
{
    storage_.reserve(other.storage_.size());
    for (const auto& ptr : other.storage_)
    {
        storage_.push_back(ptr->clone());
    }
}

HeteroVector::HeteroVector(HeteroVector&&) noexcept = default;

HeteroVector& HeteroVector::operator=(const HeteroVector& other)
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

HeteroVector& HeteroVector::operator=(HeteroVector&&) noexcept = default;

HeteroVector::~HeteroVector() = default;

void HeteroVector::push(std::unique_ptr<ValueBoxBase> box)
{
    storage_.push_back(std::move(box));
}

size_t HeteroVector::size() const noexcept
{
    return storage_.size();
}

void HeteroVector::clear()
{
    storage_.clear();
}
