#ifndef YDSTL_ALGOBASE_H
#define YDSTL_ALGOBASE_H


// 基本算法

//-----------------equal-----------------------------------------
// 比较[first,last)是否一样
template<typename InputIterator1,typename InputIterator2>
bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2)
{
    for(;first1!=last1;++first1,++first12)
    {
        if(*first1!=*first2)
            return false;
    }
    return true;
} 

template<typename InputIterator1,typename InputIterator2,typename Compare>
bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,Compare compare)
{
    for(;first1!=last1;++first1,++first12)
    {
        if(!compare(*first1,*first2))
            return false;
    }
    return true;
}


//--------------------------------fill-----------------------------
// 将[first,last)改填新值
template<typename ForwardIterator,typename T>
void fill(ForwardIterator first,ForwardIterator last,const T&value){
    for(;first!=last;++first){
        *first=value;
    }
}

//---------------fill_n----------------
template<typename ForwardIterator,typename Size,typename T>
ForwardIterator fill_n(ForwardIterator first,Size n,const T& value){
    for(;n>0;--n,++first){
        *first=value;
    }
    return first;
}







#endif