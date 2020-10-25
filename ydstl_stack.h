#ifndef YDSTL_STACK_H
#define YDSTL_STACK_H

#include "ydstl_list.h"

namespace ydstl
{

    template <typename T, typename Sequence = list<T>>
    class stack
    {
friend bool operator== <>(const stack&x,const stack&y);
friend bool operator< <>(const stack&x,const stack&y);

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
        reference top() { return c.back(); }
        const_reference top() const { return c.back(); }
        void push(const value_type &x) { c.push_back(x); }
        void pop() { c.pop_back(); }
    };

    template<typename T,typename Sequence>
    bool operator==(const stack<T,Sequence>&x,const stack<T,Sequence>&y)
    {
        return x.c==y.c;
    }
    
    template<typename T,typename Sequence>
    bool operator<(const stack<T,Sequence>&x,const stack<T,Sequence>&y)
    {
        return x.c<y.c;
    }

} // namespace ydstl

#endif