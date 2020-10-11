#ifndef YDSTL_UNINITIALIZED_H
#define YDSTL_UNINITIALIZED_H

#include"ydstl_iterator.h"
#include"ydstl_algobase.h"
#include<type_traits>

// 主要包括三个函数：uninitialized_fill_n,uninitialized_fill,uninitialized_copy
// TODO:: 完成底层函数
namespace ydstl{


// ----------------------------------------------------------uninitialized_fill_n

template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,Size n,const T&x, std::true_type)
{
    return fill_n(first,n,x);
    
}


template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,Size n,const T&x, std::false_type)
{
    auto cur=first;
    try{
    for(;n>0;--n,++cur){
        construct(&*first,x);
    }
    }catch(...){
        while (cur!=first)
        {
            --cur;
            destroy(&*cur);
        }

    }
    return cur;
}


template<typename ForwardIterator,typename Size,typename T,typename T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T&x,T1 *)
{
    typedef typename std::is_trivially_copy_assignable<T1> is_POD;
    return __uninitialized_fill_n_aux(first,Size,const T&x,is_POD{});
}


template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& x)
{
    return __uninitialized_fill_n(first,n,x,value_type(first));
}


// -------------------------------------------------------uninitialized_copy


template<typename InputIterator,typename ForwardIterator>
ForwardIterator __uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result,std::true_type){
    return copy(first,last,result);
}


template<typename InputIterator,typename ForwardIterator>
ForwardIterator __uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result,std::false_type){
    auto cur=result;
    try{
        for(;first!=last;++first,++cur){
            construct(&*cur,*first);
        }
    }catch(...){
        while (cur!=result)
        {
            --cur;
            destroy(&*cur);
        }
    }
    return cur;
}

template<typename InputIterator,typename ForwardIterator,typename T>
ForwardIterator __uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result,T*){
    typedef typename std::is_trivially_copy_assignable<T> is_POD;
    return __uninitialized_copy_aux(first,last,result,is_POD{});
}

template<typename InputIterator,typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result){
    return __uninitialized_copy(first,last,result,value_type(first));
}

// -------------------------------------------------------unintialized_fill


template<typename ForwardIterator,typename T,typename T1>
void __uninitialized_fill_aux(ForwardIterator first,ForwardIterator last,const T& x,std::false_type){
    auto cur=first;
    try{
        for(;cur!=last;++cur){
            construct(&*cur,x);
        }
    }catch(...){
        while(cur!=first){
            --cur;
            destroy(&*cur);
        }
    }
}

template<typename ForwardIterator,typename T,typename T1>
void __uninitialized_fill_aux(ForwardIterator first,ForwardIterator last,const T& x,std::true_type){
    fill(first,last,x);
}

template<typename ForwardIterator,typename T,typename T1>
void __uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& x,T1*){
    typedef typename std::is_trivially_copy_assignable<T1> is_POD;
    __uninitialized_fill(first,last,x,is_POD{});
}

template<typename ForwardIterator,typename T>
void uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& x){
    __uninitialized_fill(first,last,x,value_type(first));
}





}// end namespace ydstl

#endif