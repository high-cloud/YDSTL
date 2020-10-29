#ifndef YDSTL_RBTREE_H
#define YDSTL_RBTREE_H

#include "ydstl_iterator.h"

// 红黑树
// 有一个特殊的header结点，父亲指向根，右指向最大，左指向最小

namespace ydstl
{

    //-----------------结点定义

    typedef bool __rb_tree_color_type;
    const __rb_tree_color_type __rb_tree_red = false;  // 红色为0
    const __rb_tree_color_type __rb_tree_black = true; // 黑色为1

    //基类
    struct __rb_tree_node_base
    {
        typedef __rb_tree_color_type color_type;
        typedef __rb_tree_node_base *base_ptr;

        color_type color;
        base_ptr parent; //父结点
        base_ptr left;
        base_ptr right;

        // x不能为空
        static base_ptr minimum(base_ptr x)
        {
            while (x->left)
            {
                x = x->left;
            }

            return x;
        }

        // x不能为空
        static base_ptr maxmum(base_ptr x)
        {
            while (x->right)
            {
                x = x->right;
            }

            return x;
        }
    };

    // 结点
    template <typename T>
    struct __rb_tree_node
    {
        typedef __rb_tree_node<T> *link_type;
        T value;
    }

    // -------------迭代器---------------

    // base
    struct __rb_tree_iterator_base
    {
        typedef __rb_tree_node_base::base_ptr base_ptr;
        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;

        base_ptr node; // 指向的结点

        // 最大值再增为空
        void increment()
        {
            //如果是header
            if (node->color == __rb_tree_red && node->parent->parent == node)
                node = node->left;
            else if (node->right)
            {
                node = node->right;
                while (node->left)
                {
                    node = node->left;
                }
            }
            else
            {
                base_ptr y = node->parent;
                while (node == y->right)
                {
                    node = y;
                    y = node->parent;
                }
                if (node->right != y) // 防止根节点是最大的
                    node = y;
            }
        }

        //
        void decrement()
        {
            //如果是header
            if (node->color == __rb_tree_red && node->parent->parent == node)
                node = node->right;
            else if (node->left)
            {
                node = node->left;
                while (node->right)
                {
                    node = node->right;
                }
            }
            else
            {
                base_ptr y = node->parent;
                while (node == y->left)
                {
                    node = y;
                    y = node->parent;
                }
                if (node->left != y)
                    node = y;
            }
        }

    };

    // 迭代器
    template<typename T,typename Ref,typename Ptr>
    struct __rb_tree_iterator: __rb_tree_iterator_base
    {
        typedef T value_type;
        typedef Ref reference;
        typedef Ptr pointer;
        typedef __rb_tree_iterator<T,T&,T*> iterator;
        typedef __rb_tree_iterator<T,const T&,const T*> const_iterator;
        typedef __rb_tree_iterator<T,Ref,Ptr> self;
        typedef __rb_tree_node<T>* link_type;

        __rb_tree_iterator(){}
        __rb_tree_iterator(link_type* x) {node=x;}
        __rb_tree_iterator(const iterator& it){node=it.node;}

        reference operator*() const{return link_type(node)->value;}
        pointer operator->()const {return &(operator*());}

        self& operator++() { increment(); return *this;}
        self operator++(int) { self temp=*this; increment(); return temp;}
        self& operator--() { decrement(); return *this;}
        self operator--(int) { self temp=*this; decrement(); return temp;}
    };

} // namespace ydstl

#endif