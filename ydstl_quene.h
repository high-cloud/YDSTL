#ifndef YDSTL_QUENE_H
#define YDSTL_QUENE_H

#include "ydstl_list.h"
#include "ydstl_vector.h"
#include "ydstl_functional.h"
#include "ydstl_algorithm.h"

namespace ydstl
{
    // 定义队列
    template <typename T, typename Sequence = list<T>>
    class quene
    {
        friend bool operator==<>(const quene &x, const quene &y);
        friend bool operator<<>(const quene &x, const quene &y);

    public:
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
        typedef typename Sequence::const_reference const_reference;

    protected:
        Sequence c;

    public:
        bool empty() { return c.empty(); }
        size_type size() { return c.size(); }
        reference top() { return c.front(); }
        const_reference top() const { return c.front(); }
        void push(const value_type &x) { c.push_back(x); }
        void pop() { c.pop_front(); }
    };

    template <typename T, typename Sequence>
    bool operator==(const quene<T, Sequence> &x, const quene<T, Sequence> &y)
    {
        return x.c == y.c;
    }

    template <typename T, typename Sequence>
    bool operator<(const quene<T, Sequence> &x, const quene<T, Sequence> &y)
    {
        return x.c < y.c;
    }

    // 优先队列定义
    template <typename T, typename Sequence = vector<T>, typename Compare = less<typename Sequence::value_type>>
    class priority_quene
    {
    public:
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
        typedef typename Sequence::const_reference const_reference;

    protected:
        Sequence c; //底层容器
        Compare comp;

    public:
        // 构造函数
        priority_quene() : c() {}
        explicit priority_quene(const Compare &x) : c(), comp(x) {}

        template <typename InputIterator>
        priority_quene(InputIterator first, InputIterator last, const Compare &x) : c(first, last), comp(x)
        {
            make_heap(first, last, comp);
        }

        template <typename InputIterator>
        priority_quene(InputIterator first, InputIterator last) : c(first, last)
        {
            make_heap(first, last, comp);
        }

        bool empty() const { return c.empty(); }
        size_type size() const { return c.size(); }
        const_reference top() const { return c.front(); }
        void push(const value_type &x)
        {
            try
            {
                c.push_back(x);
                push_heap(c.begin(), c.end(), comp);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }

        void pop(){
            pop_heap(c.begin(),c.end(),comp);
            c.pop_back();
        }
    };

} // namespace ydstl

#endif