#ifndef __YDSTL_SET_H__
#define __YDSTL_SET_H__

#include "ydstl_rbTree.h"
#include "ydstl_functional.h"
#include"ydstl_pair.h"

namespace ydstl
{
    template <typename Key, typename Compare = less<Key>, typename Alloc = alloc>
    class set
    {
    public:
        //typedefs
        typedef Key key_type;
        typedef Key value_type;
        typedef Compare key_compare;
        typedef Compare value_compare;

    private:
        typedef rb_tree<key_type, value_type, identity<key_type>, Compare, Alloc> rep_type;
        rep_type t; // set的存储实现
    public:
        typedef typename rep_type::pointer pointer;
        typedef typename rep_type::const_pointer const_pointer;
        typedef typename rep_type::reference reference;
        typedef typename rep_type::const_reference const_reference;
        typedef typename rep_type::const_iterator iterator;
        typedef typename rep_type::const_iterator const_iterator;
        typedef typename rep_type::size_type size_type;
        typedef typename rep_type::difference_type difference_type;


        public:
        // 
        set() :t(Comapre()){}
        explicit set(const Compare& comp): t(comp){}

        template<typename InputIterator>
        set(InputIterator first,InputIterator last):t(Compare()) {
            t.insert_unique(first,last);
        }

        template<typename InputIterator>
        set(InputIterator first,InputIterator last,const Compare& comp):t(comp) {
            t.insert_unique(first,last);
        }

        public:
        // accessors
        key_compare key_comp(){return t.key_comp();}
        value_copare value_comp() {return t.key_comp();}
        iterator begin() { return t.begin();}
        iterator end() { return t.end();}
        bool empty() const {return t.empty();}
        size_type size() const {return t.size();}
        size_type max_size() const {return t.max_size();}

        // insert/erase
        pair<iterator,bool> insert(const value_type& x){
            auto p=t.insert_unique(x);
            return pair<iterator,bool>(p.first,p.second);
        }

        template<typename InputIterator>
        void insert(InputIterator first,InputIterator last)
        {
            t.insert_unique(first,last);
        }
        
        void erase(const value_type& x)
        {
            t.erase(x);
        }
    };
} // namespace ydstl

#endif // __YDSTL_SET_H__