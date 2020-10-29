#ifndef YDSTL_FUNCTIONAL_H
#define YDSTL_FUNCTIONAL_H

// 该文件定义 仿函数

namespace ydstl
{

    //--------------------------------------- 仿函数基类 -------------------

    // 一元仿函数
    template <typename Arg, typename Result>
    struct unary_function
    {
        typedef Arg argument_type;
        typedef Result result_type;
    };

    //二元仿函数
    template <typename Arg1, typename Arg2, typename Result>
    struct binary_function
    {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };

    //---------------------------------算术类仿函数-------------------

    // 加法
    template <typename T>
    struct plus : public binary_function<T, T, T>
    {
        T operator()(const T &x, const T &y) const { return x + y; }
    };

    // 减法
    template <typename T>
    struct minus : public binary_function<T, T, T>
    {
        T operator()(const T &x, const T &y) const { return x - y; }
    };

    // 乘法

    template <typename T>
    struct multiplies : public binary_function<T, T, T>
    {
        T operator()(const T &x, const T &y) const { return x * y; }
    };

    // 除法
    template <typename T>
    struct divide : public binary_function<T, T, T>
    {
        T operator()(const T &x, const T &y) const { return x / y; }
    };

    // 模
    template <typename T>
    struct modulus : public binary_function<T, T, T>
    {
        T operator()(const T &x, const T &y) const { return x % y; }
    };

    // 否定
    template <typename T>
    struct negate : public unary_function<T, T>
    {
        T operator()(const T &x) const { return -x; }
    };

    //------------------------关系运算-------------------------

    // 等于
    template <typename T>
    struct equal_to : public binary_function<T, T, T>
    {
        bool operator()(const T &x, const T &y) const { return x == y; }
    };

    template <typename T>
    struct not_equal_to : public binary_function<T, T, T>
    {
        bool operator()(const T &x, const T &y) const { return x != y; }
    };

    template <typename T>
    struct greater : public binary_function<T, T, T>
    {
        bool operator()(const T &x, const T &y) const { return x > y; }
    };

    template <typename T>
    struct greater_equal : public binary_function<T, T, T>
    {
        bool operator()(const T &x, const T &y) const { return x >= y; }
    };

    template <typename T>
    struct less : public binary_function<T, T, T>
    {
        bool operator()(const T &x, const T &y) const { return x < y; }
    };

    template <typename T>
    struct less_equal : public binary_function<T, T, T>
    {
        bool operator()(const T &x, const T &y) const { return x < y; }
    };

    //---------------------------------- 逻辑运算-----------------------

    template <typename T>
    struct logical_and : public binary_function<T, T, T>
    {
        bool operator()(const T &x, const T &y) const { return x && y; }
    };

    template <typename T>
    struct logical_or : public binary_function<T, T, T>
    {
        bool operator()(const T &x, const T &y) const { return x || y; }
    };

    template <typename T>
    struct logical_not : public unary_function<T, bool>
    {
        bool operator()(const T &x) const { return !x; }
    };



//------------------------------ 其他------------------------------

// 全同函数
template<typename T>
struct identity: public unary_function<T,T>
{
    const T& operator()(const T& x) const {return x;}
};

// 选择函数
template<typename Pair>
struct select1st: public unary_function<Pair,typename Pair::first_type> 
{
    const typename Pair::first_type operator()(const Pair& x) const { return x.first;}
};

template<typename Pair>
struct select2nd: public unary_function<Pair,typename Pair::second_type> 
{
    const typename Pair::second_type operator()(const Pair& x) const{ return x.second;}
};

// 投射函数
template<typename Arg1,typename Arg2>
struct project1st: public binary_function<Arg1,Arg2,Arg1>
{
    Arg1 operator()(const Arg1& x,const Arg2& y) const{return x;}
};

template<typename Arg1,typename Arg2>
struct project2nd: public binary_function<Arg1,Arg2,Arg2>
{
    Arg2 operator()(const Arg1& x,const Arg2& y) const{return y;}
}; 

} // namespace ydstl

#endif