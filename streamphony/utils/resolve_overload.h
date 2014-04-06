#ifndef RESOLVE_OVERLOAD_H
#define RESOLVE_OVERLOAD_H

namespace utils {

/**
 *  Helps resolve the correct signal when using connect with the new signal
 */

template <typename... Args>
class resolve_overload
{
    resolve_overload() = delete;
    resolve_overload(const resolve_overload&) = delete;
public:
    template <typename Result, typename Class>
    static auto constexpr of( Result (Class::*pointer_to_method)(Args...) )
        -> decltype(pointer_to_method)
    {
        return pointer_to_method;
    }
    template <typename Result, typename Class>
    static auto constexpr of( Result (Class::*pointer_to_method)(Args...) const)
        -> decltype(pointer_to_method)
    {
        return pointer_to_method;
    }

    template <typename Result, typename Class>
    static auto constexpr of_non_const( Result (Class::*pointer_to_method)(Args...) )
        -> decltype(pointer_to_method)
    {
        return pointer_to_method;
    }

    template <typename Result, typename Class>
    static auto constexpr of_const( Result (Class::*pointer_to_method)(Args...) const)
        -> decltype(pointer_to_method)
    {
        return pointer_to_method;
    }
};

} // utils


#endif // RESOLVE_OVERLOAD_H
