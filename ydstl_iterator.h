#ifndef YDSTL_ITERATOR.H
#define YDSTL_ITERATOR.H

#include<stddef.h>

struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag{};
struct bidirectional_iterator_tag{};
struct randon_access_iterator_tag{};

template<typename Category,typename T,typename Distance=ptrdiff_t,typename Pointer=T*,typename Reference=T&>
struct iterator
{
    typedef Category iterator_category;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef T value_type;
    typedef Reference reference;
};



template<typename Iterator>
struct iterator_traits
{
    typedef typename Iterator::value_type                value_type;
    typedef typename Iterator::difference_type           difference_type;
    typedef typename Iterator::iterator_category         iterator_category;
    typedef typename Iterator::pointer                   pointer;
    typedef typename Iterator::reference                 reference;
};

// 特化
template<typename T>
struct iterator_traits<T*>
{
    typedef typename T                value_type;
    typedef typename ptrdiff_t           difference_type;
    typedef typename randon_access_iterator_tag         iterator_category;
    typedef typename T*               pointer;
    typedef typename T&                 reference;
};


template<typename T>
struct iterator_traits<const T*>
{
    typedef typename T                value_type;
    typedef typename ptrdiff_t           difference_type;
    typedef typename randon_access_iterator_tag         iterator_category;
    typedef typename const T*               pointer;
    typedef typename const T&                 reference;
};









#endif