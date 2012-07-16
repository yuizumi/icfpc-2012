// -*- C++ -*-

#ifndef STORE_H_
#define STORE_H_

#include <boost/utility.hpp>
#include <vector>

template <typename T>
class Store : boost::noncopyable
{
public:
    explicit Store(size_t size) : elems_(size, NULL), cursor_() {}
    virtual ~Store();

    void Push(T* elem);
    void Pop();

    T* Top() const;

private:
    std::vector<T*> elems_;
    size_t cursor_;
};

#include "store-inl.h"

#endif // STORE_H_
