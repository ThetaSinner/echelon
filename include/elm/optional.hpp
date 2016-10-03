#ifndef OPTIONAL_HPP_INCLUDED
#define OPTIONAL_HPP_INCLUDED

#include <memory>

template<typename T>
class Optional
{
public:
    static Optional<T> of(T value)
    {
        return Optional<T> (value);
    }

    static Optional<T> absent()
    {
        return Optional<T> ();
    }

    bool isPresent()
    {
        return has_value;
    }

    T get()
    {
        return value;
    }
private:
    Optional() {}

    Optional(T value)
    {
        has_value = true;
        this->value = value;
    }

    bool has_value = false;
    T value;
};

#endif // OPTIONAL_HPP_INCLUDED
