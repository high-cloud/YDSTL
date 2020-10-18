#ifndef YDSTL_LIST_H
#define YDSTL_LIST_H

// list
#include "ydstl_alloc.h"
#include "ydstl_iterator.h"
#include "ydstl_construct.h"

namespace ydstl
{
    //------------list node---------------
    template <typename T>
    struct __list_node
    {
        __list_node<T> *prev;
        __list_node<T> *next;
        T data;
    };

    //------------list iterator------------------
    template <typename T, typename Ref, typename Ptr>
    struct __list_iterator
    {
        typedef __list_iterator<T, T &, T *> iterator;
        typedef __list_iterator<T, Ref, Ptr> self;

        typedef bidirection_iterator_tag iterator_category;
        typedef T value_type;
        typedef Ptr pointer;
        typedef Ref reference;
        typedef __list_node<T> *link_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        link_type node;

        // construct
        __list_iterator(link_type x) : node(x) {}
        __list_iterator() {}
        __list_iterator(const iterator &x) : node(x.node) {}

        bool operator==(const self &x) const { return node == x.node; }
        bool operator!=(const self &x) const { return node != x.node; }
        reference operator*() const { return node->data; }
        pointer operator->() const { return &(operator*()); }

        self &operator++()
        {
            node = (link_type)(node->next);
            return *this;
        }

        self operato++(int)
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }

        self &operator--()
        {
            node = (link_type)(node->prev);
            return *this;
        }

        self operator--(int)
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }
    };

    //-------------------class list------------------------
    template <class T, class Alloc = alloc>
    class list
    {
    protected:
        typedef __list_node<T> list_node;
        // 空间配置器
        typedef simple_alloc<__list_node, Alloc> list_node_allocator;

    public:
        typedef __list_iterator<T, T &, T *> iterator;

        typedef list_node *link_type;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef iterator::size_type size_type;
        typedef iterator::difference_type difference_type;

    protected:
        // list使用一个指针来完整记录整个链表
        // node指向一个空节点，它的下一个是链表的头
        // 链表是双向环形的
        link_type node;

    protected:
        //内存相关
        link_type get_node() { return list_node_allocator::allocate(); }
        void put_node(link_type p) { list_node_allocator::deallocate(p); }
        link_type create_node(const T &x)
        {
            link_type p = get_node();
            construct(&p->data, x);
            return p;
        }

        void destroy_node(link_type p)
        {
            destroy(&p->data);
            put_node(p);
        }

    public:
        //construct
        list() { empty_initialize(); }

        // 元素相关
        reference front() { return *begin(); }
        reference back() { return *(--end()); }
        void push_back(const T &x) { insert(end(), x); }
        void push_front(const T &x) { insert(begin(), x); }
        void pop_front() { erase(begin()); }
        void pop_back() { erase(--end()); }
        void clear();                //清空所有元素
        void remove(const T &value); //删除所有data=value的结点
        void unique();               //删除data相同的结点

        iterator insert(iterator position, const T &x);

        //迭代器相关
        iterator begin() { return (link_type)((*node)->next); }
        iterator end() { return node; }
        iterator erase(iterator position);
        void swap(list& x){
            link_type tmp=node;
            node=x.node;
            x.node=tmp;
        }

        // 链表 序列相关
        void splice(iterator position, list &x)
        { // 把x插到position前面
            if (!x.empty())
                transfer(position, x.begin(), x.end());
        }
        void splice(iterator position, list &, iterator i)
        {
            iterator j = i;
            ++j;
            if (position == i || position == j)
            {
                return;
            }
            transfer(position, i, j);
        }
        void splice(iterator position, iterator first, iterator last);

        void merge(list &x); //合并，递增顺序
        void reverse();
        void sort();

        //大小相关
        bool empty() const { return node == node->next; }
        size_type size() const
        {
            size_type result = 0;
            distance(begin(), end(), result); //TODO : 全局函数，库还没写
            return result;
        }

    protected:
        // help funtion

        //初始化空结点
        void empty_initialize()
        {
            node = get_node();
            node->next = node;
            node->prev = node;
        }

        // 将first，last之间的元素迁到position之前
        void transfer(iterator position, iterator first, iterator last)
        {
            if (position != last)
            {
                last.node->prev->next = position;
                position.node->prev->next = first;
                position.node->prev->next = last;
                auto tmp = first.node->prev;
                first.node->prev = position.node->prev;
                position.node->prev = last.node->prev;
                last.node->prev = tmp;
            }
        }

        //
    };

    template <class T, class Alloc>
    void list<T, Alloc>::clear()
    {
        link_type cur = node->next;
        while (cur != node)
        {
            link_type tmp = cur;
            cur = cur->next;
            destroy_node(tmp);
        }
        node->next = node;
        node->prev = node;
    }

    template <class T, class Alloc>
    void list<T, Alloc>::remove(const T &value)
    {
        iterator first = begin();
        iterator last = end();
        while (first != last)
        {
            iterator tmp = first;
            ++first;
            if (*tmp == value)
                erase(tmp);
        }
    }

    template <class T, class Alloc>
    void list<T, Alloc>::unique()
    {
        iterator first = begin();
        iterator last = end();
        if (first == last)
            return;
        iterator next = first;
        while (++next != last)
        {
            if (*first == *next)
                erase(next);
            else
            {
                first = next;
            }
            next = first;
        }
    }

    template <class T, class Alloc>
    list<T, Alloc>::iterator list<T, Alloc>::erase(iterator position)
    {
        link_type prev_node = link_type(position.node->prev);
        link_type next_node = link_type(position.node->next);
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destroy_node(position.node);
        return iterator(next_node);
    }

    template <class T, class Alloc>
    void list<T, Alloc>::merge(list<T, Alloc> &x)
    {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();

        while (first1 != last1 && first2 != last2)
        {
            if (*first1 > *first2)
            {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            }
            else
            {
                ++first1;
            }
        }
        if (first2 != last2)
            transfer(first1, first2, last2);
    }

    template <class T, class Alloc>
    void list<T, Alloc>::reverse(){
        // 空或者只有一个元素，不用做什么
        if( node->next==node || node->next->next==node)
            return
        iterator first=begin();
        ++first;
        while(first!=node){
            iterator old=first;
            ++first;
            transfer(begin(),old,first);
        }
    }

    template <class T, class Alloc>
    void list<T, Alloc>::sort()
    {
        if( node->next==node || node->next->next==node)
            return;
        
        list<T,Alloc> carry;
        list<T,Alloc> counter[64];
        int fill=0
        while(!empty())
        {
            carry.splice(carry.begin(),begin());
            int i=0;
            while(i<fill && !counter[i].empty())
            {
                counter[i].merge(carry);
                carry.swap(counter[i++])
            }
            carry.swap(counter[i]);
            if(i==fill) ++fill;
        }

        for(int i=1;i<fill;++i)
        {
            counter[i].merge(counter[i-1]);
        }
        swap(counter[fill+1]);
    }

} // namespace ydstl
#endif