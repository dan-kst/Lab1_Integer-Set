#ifndef INTEGER_SET_HPP
#define INTEGER_SET_HPP

#include <memory>
#include <vector>
#include <initializer_list>

class IntegerSet {
private:
    // Using a smart pointer to a dynamic array as per lab requirements
    // However, to follow modern C++, we'll track size separately
    std::unique_ptr<int[]> elements_;
    size_t current_size_;
    size_t capacity_;

    void reserve(size_t new_capacity);

public:
    // Constructors & Destructor
    IntegerSet();
    IntegerSet(std::initializer_list<int> list);
    ~IntegerSet(); // unique_ptr handles deletion, but we declare it for the lab

    // Rule of Three/Five (SOLID: ensures correct copying behavior)
    IntegerSet(const IntegerSet& other);
    IntegerSet& operator=(const IntegerSet& other);

    // Core Functions
    void add(int element);
    bool contains(int element) const;
    size_t size() const { return current_size_; }

    // Set Operations (Returning unique_ptrs for cross-platform safety)
    std::unique_ptr<IntegerSet> unite(const IntegerSet& other) const;
    std::unique_ptr<IntegerSet> intersect(const IntegerSet& other) const;
    std::unique_ptr<IntegerSet> difference(const IntegerSet& other) const;

	// Helper functions
    std::vector<int> toVector() const;
    // For Console Wrap
    void print() const;
    void input(); 
};
#endif
