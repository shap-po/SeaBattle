#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include <memory>

using std::shared_ptr;
using std::unique_ptr;

template <class T, class... Args>
shared_ptr<T> MakeShared(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <class T, class... Args>
unique_ptr<T> MakeUnique(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

#endif // MEMORY_HPP_
