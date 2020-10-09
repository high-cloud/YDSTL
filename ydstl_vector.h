#ifndef YDSTL_VECTOR_H
#define YDSTL_VECTOR_H

#include"ydstl_alloc.h"

// vector 容器的定义

//TODO: much
namespace ydstl{

    template<typename T,typename Alloc=alloc>
    class vector
    {
    public:
        // 嵌套型别定义
        typedef T value_type;
        typedef value_type* pointer_type;
        typedef value_type* iterator;
        typedef value_type& reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
    protected:
        typedef simple_alloc<value_type,Alloc> data_allocator;

        iterator start; //已用空间的开头
        iterator end; //已用空间的尾
        iterator end_of_storage;  //可用空间的尾

//TODO

    };
    
}

#endif