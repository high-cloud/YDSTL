#ifndef YDSTL_CONSTRUCT_H
#define YDSTL_CONSTRUCT_H

#include<new>
#include<type_traits>

namespace ydstl{

// construct
template<typename T1,typename T2>
inline void construct(T1 *p,const T2& value)
{
    ::new(p) T1(value);
}

// destruct
template<typename T>
inline void destroy(T* pointer)
{
    pointer->~T();
}

template<typename Iterator>
inline void destroy(Iterator first,Iterator last)
{
    __destroy(first,last,value_type(first));
}

template<typename Iterator, typename T>
inline void __destroy(Iterator first,Iterator last,T* p)
{
    __destroy_aux(first,last, std:is_trivally_destructible<T>());
}

template<typename Iterator>
inline void __destroy_aux(Iterator first,Iterator last,std::false_type)
{
    for(;first<last;++first)
    {
        destroy(&*first);
    }
}

}// end namespace ydstl

#endif