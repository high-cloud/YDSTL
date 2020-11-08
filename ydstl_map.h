#ifndef __YDSTL_MAP_H__
#define __YDSTL_MAP_H__

#include "ydstl_rbTree.h"
#include "ydstl_pair.h"
#include "ydstl_alloc.h"
#include "ydstl_functional.h"

namespace ydstl
{

    template <typename Key, typename T, typename Compare = less<Key>, typename Alloc = alloc>
    class map
    {
    public:
        //typedefs
        typedef Key key_type;
        typedef T data_type;
        typedef T mapped_type;
        typedef pair<key_type, data_type> value_type;
        typedef Compare key_compare;

        // 元素比较函数
        class value_compare : public binary_function<value_type, value_type, bool>
        {
        protected:
            Compare comp;
            value_compare(Comapre c) : comp(c) {}

        public:
            bool operator()(const value_type &x, const value_type &y) const
            {
                return comp(x.first, y.first);
            }
        };

        private:
        

    };
} // namespace ydstl

#endif // __YDSTL_MAP_H__