#ifndef YDSTL_ITERATOR.H
#define YDSTL_ITERATOR.H

#include<stddef.h>

namespace ydstl{

struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag:public input_iterator_tag{};
struct bidirectional_iterator_tag:forward_iterator_tag{};
struct random_access_iterator_tag:bidirectional_iterator_tag{};

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

// value type function
template<typename Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
    typedef typename iterator_traits<Iterator>::value_type value_type;
    return static_cast<value_type *>(0);
}

// return iterator category
template<typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&){
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return static_cast<category *>(0);
}


// return distance category
template<typename Iterator>
inline typename iterator_traits<Iterator>::difference_type
iterator_category(const Iterator&){
    typedef typename iterator_traits<Iterator>::difference_type distance;
    return static_cast<distance *>(0);
}



}// end namespace ydstl



#endif