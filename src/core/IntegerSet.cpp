#include "./core/IntegerSet.hpp"

void IntegerSet::reserve(size_t new_capacity)
{
    capacity_ = new_capacity;
    std::unique_ptr<int[]> elements = std::make_unique<int[]>(capacity_);
    for(size_t i = 0; i < current_size_; i++)
    {
        elements[i] = elements_[i];
    }
    elements_ = std::move(elements);
}

// Constructors & Destructor
IntegerSet::IntegerSet() : current_size_(0), capacity_(10)
{
    elements_ = std::make_unique<int[]>(capacity_);
}
IntegerSet::IntegerSet(std::initializer_list<int> list) 
    : current_size_(0), capacity_(list.size() + 10) 
{
    elements_ = std::make_unique<int[]>(capacity_);
    for (int item : list) {
        add(item);
    }
}
IntegerSet::~IntegerSet(){} // unique_ptr handles deletion, but we declare it for the lab

// Rule of Three/Five (SOLID: ensures correct copying behavior)
IntegerSet::IntegerSet(const IntegerSet& other)
{
    current_size_ = other.current_size_;
    capacity_ = other.capacity_;
    elements_ = std::make_unique<int[]>(capacity_);
    for(size_t i = 0; i < current_size_; i++)
    {
        add(other.elements_[i]);
    }
}
IntegerSet& IntegerSet::operator=(const IntegerSet& other)
{
    if (this == &other) return *this; // Protects against mySet = mySet
    current_size_ = other.current_size_;
    capacity_ = other.capacity_;
    elements_ = std::make_unique<int[]>(capacity_);
    for(size_t i = 0; i < current_size_; i++)
    {
        elements_[i] = other.elements_[i];
    }
    return *this;
}

// Core Functions
void IntegerSet::add(int element)
{
    if(current_size_ >= capacity_)
        reserve(capacity_ + 10);
    if(!contains(element))
        elements_[current_size_++] = element;
}
bool IntegerSet::contains(int element) const
{
    for(size_t i = 0; i < current_size_; i++)
    {
        if(elements_[i] == element)
            return true;
    }
    return false;
}
// Set Operations (Returning unique_ptrs for cross-platform safety)
std::unique_ptr<IntegerSet> IntegerSet::unite(const IntegerSet& other) const
{
    std::unique_ptr<IntegerSet> uniteSet = std::make_unique<IntegerSet>();
    for(size_t i = 0; i < current_size_; i++)
    {
        uniteSet->add(this->elements_[i]);
    }
    for(size_t i = 0; i < other.current_size_; i++)
    {
        uniteSet->add(other.elements_[i]);
    }
    return uniteSet;
}
std::unique_ptr<IntegerSet> IntegerSet::intersect(const IntegerSet& other) const
{
    std::unique_ptr<IntegerSet> intersectSet = std::make_unique<IntegerSet>();
    for(size_t i = 0; i < current_size_; i++)
    {
        if(other.contains(elements_[i]))
            intersectSet->add(this->elements_[i]);
    }
    return intersectSet;
}
std::unique_ptr<IntegerSet> IntegerSet::difference(const IntegerSet& other) const
{
    std::unique_ptr<IntegerSet> differenceSet = std::make_unique<IntegerSet>();
    for(size_t i = 0; i < current_size_; i++)
    {
        if(!other.contains(elements_[i]))
            differenceSet->add(this->elements_[i]);
    }
    return differenceSet;
}

// Helper functions
std::vector<int> IntegerSet::toVector() const
{
    std::vector<int> elements(elements_.get(),elements_.get() + current_size_);
    return elements;
}

// For Console Wrap
void IntegerSet::print() const
{
    
}
void IntegerSet::input()
{
    
}
