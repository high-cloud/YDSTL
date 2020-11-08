#ifndef __YDSTL_RBTREE_H__
#define __YDSTL_RBTREE_H__

#include "ydstl_alloc.h"
#include "ydstl_pair.h"
#include "ydstl_iterator.h"
#include "ydstl_construct.h"

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

    // 红黑树 结点
    template <typename T>
    struct __rb_tree_node
    {
        typedef __rb_tree_node<T> *link_type;
        T value;
    };

    // 全局方法

    // 红黑树左旋转
    inline void __rb_tree_rotate_left(__rb_tree_node_base *x, __rb_tree_node_base *&root)
    {
        // 旋转x
        auto y = x->right;
        x->right = y->left;
        if (y->left)
            y->left->parent = x;
        y->parent = x->parent;

        if (x == root)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
        {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // 红黑树右旋转
    inline void __rb_tree_rotate_right(__rb_tree_node_base *x, __rb_tree_node_base *&root)
    {
        // 旋转x
        auto y = x->left;
        x->left = y->right;
        if (y->right)
            y->right->parent = x;
        y->parent = x->parent;

        if (x == root)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
        {
            x->parent->right = y;
        }
        y->right = x;
        x->parent = y;
    }
    // 调整红黑树重新平衡
    void __rb_tree_rebalance(__rb_tree_node_base *x, __rb_tree_node_base *&root)
    {
        x->color = __rb_tree_red; //新节点为红色
        auto y = x->parent;

        while (x->parent->color == __rb_tree_red && x != root)
        {
            if (x->parent == x->parent->parent->left) // x的父亲是祖父的左节点
            {
                auto y = x->parent->parent->right;
                if (y && y->color == __rb_tree_red)
                {
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    y->color = __rb_tree_black;
                    x = y->parent;
                }
                else
                {
                    if (x = x->parent->right)
                    {
                        x = x->parent;
                        __rb_tree_rotate_left(x, root);
                    }
                    x->parent->parent->color = __rb_tree_red;
                    x->parent->color = __rb_tree_black;
                    __rb_tree_rotate_right(x->parent->parent, root);
                }
            }
            else
            {

                auto y = x->parent->parent->left;
                if (y && y->color == __rb_tree_red)
                {
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    y->color = __rb_tree_black;
                    x = y->parent;
                }
                else
                {
                    if (x = x->parent->left)
                    {
                        x = x->parent;
                        __rb_tree_rotate_right(x, root);
                    }
                    x->parent->parent->color = __rb_tree_red;
                    x->parent->color = __rb_tree_black;
                    __rb_tree_rotate_left(x->parent->parent, root);
                }
            }

            root->color = __rb_tree_black;
        }
    }

    // 红黑树 删除节点后 重新平衡 todo
    void __rb_tree_delete_rebalance()

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
    template <typename T, typename Ref, typename Ptr>
    struct __rb_tree_iterator : __rb_tree_iterator_base
    {
        typedef T value_type;
        typedef Ref reference;
        typedef Ptr pointer;
        typedef __rb_tree_iterator<T, T &, T *> iterator;
        typedef __rb_tree_iterator<T, const T &, const T *> const_iterator;
        typedef __rb_tree_iterator<T, Ref, Ptr> self;
        typedef __rb_tree_node<T> *link_type;

        __rb_tree_iterator() {}
        __rb_tree_iterator(link_type x) { node = x; }
        __rb_tree_iterator(const iterator &it) { node = it.node; }

        reference operator*() const { return link_type(node)->value; }
        pointer operator->() const { return &(operator*()); }

        self &operator++()
        {
            increment();
            return *this;
        }
        self operator++(int)
        {
            self temp = *this;
            increment();
            return temp;
        }
        self &operator--()
        {
            decrement();
            return *this;
        }
        self operator--(int)
        {
            self temp = *this;
            decrement();
            return temp;
        }
    };

    // 红黑树
    template <typename key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = alloc>
    class rb_tree
    {
    protected:
        typedef void *void_pointer;
        typedef __rb_tree_node_base *base_ptr;
        typedef __rb_tree_node<Value> rb_tree_node;
        typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node__allocator;
        typedef __rb_tree_color_type color_type;

    public:
        typedef Value value_type;
        typedef Key key_type;
        typedef value_type *pointer;
        typedef const value_type *const_pointer;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef rb_tree_node *link_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
        typedef __rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;
        typedef rb_tree<key_type, value_type, KeyOfValue, Compare, Alloc> self;

    protected:
        link_type get_node() { return rb_tree_node__allocator::allocate(); };
        void put_node(link_type p) { rb_tree_node__allocator::deallocate(p); }

        link_type create_node(const value_type &x)
        {
            link_type node = get_node();
            try
            {
                construct(&node->value, x);
            }
            catch (const std::exception &e)
            {
                put_node(node);
            }

            return node;
        }
        link_type clone_node(link_type x)
        {
            link_type y = create_node(x->value);
            y->color = x->color;
            y->left = 0;
            y->right = 0;
            return y;
        }

        void destroy_node(link_type p)
        {
            destroy(&(x->value));
            put_node(x);
        }

        // 清除所有结点
        void clear(); //TODO

    protected:
        //  数据
        size_type node_count;
        link_type header;
        Compare key_compare;

        // 以下三个函数取得header的成员
        link_type root() const { return (link_type &)(header->parent); }
        link_type leftmost() const { return (link_type &)(header->left); }
        link_type rightmost() const { return (link_type &)(header->right); }

        // 下面6个静态函数方便取得结点的成员
        static link_type &left(link_type x) { return (link_type &)(x->left); }
        static link_type &right(link_type x) { return (link_type &)(x->right); }
        static link_type &parent(link_type x) { return (link_type &)(x->parent); }
        static reference value(link_type x) { return x->value; }
        static(const key_type &) key(link_type x) { return KeyOfValue(value(x)); }
        static color_type &color(link_type x) { return (color_type &)(x->color); }

        // 下面6个静态函数方便取得结点的成员
        static link_type &left(base_ptr x) { return (link_type &)(x->left); }
        static link_type &right(base_ptr x) { return (link_type &)(x->right); }
        static link_type &parent(base_ptr x) { return (link_type &)(x->parent); }
        static reference value(base_ptr x) { return (link_type)x->value; }
        static(const key_type &) key(base_ptr x) { return KeyOfValue(value((link_type)x)); }
        static color_type &color(base_ptr x) { return (color_type &)((link_type)x->color); }

        // 极大值和极小值
        static link_type minimum(link_type x) { return (link_type)__rb_tree_node_base::minimum(x); }
        static link_type maxmum(link_type x) { return (link_type)__rb_tree_node_base::maxmum(x); }

    private:
        iterator __insert(base_ptr x, base_ptr y, const value_type &v); // x插入点，y插入点父亲
        link_type __copy(link_type x, link_type p);                     //todo
        void __erase(link_type x);                                      //todo
        void init()
        {
            header = get_node();           // 申请新节点
            color(header) = __rb_tree_red; // 用来区别根节点

            root() = 0;
            leftmost() = header;
            rightmost() = header;
        }

    public:
        rb_tree(const Compare &comp = Compare()) : node_count(0), key_compare(comp) { init(); }
        ~rb_tree()
        {
            clear();
            put_node(header);
        }
        iterator find(const key_type &k);
        self &operator=(const self &x);

    public:
        Compare key_comp() const { return key_compare; }
        iterator begin() { return leftmost(); }
        iterator end() { return header; }
        bool empty() const { return node_count; }
        size_type size() const {return node_count;}
        size_type max_size() const { return size_type(-1); }

    public:
        pair<iterator, bool> insert_unique(const value_type &x);
        template <typename InputIterator>

        // 没考虑 插入错误 todo
        void insert_unique(InputIterator first, InputIterator last)
        {
            while (first != last)
            {
                insert_unque(*first);
            }
        }
        pair<iterator, bool> insert_unique(const value_type &x);

        iterator insert_equal(const value_type &x); //允许重复

        void erase(const value_type& x); // 删除元素 未考虑重复元素存在 todo
    };

    template <typename key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    auto rb_tree<key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr _x, base_ptr _y, const value_type &v)->iterator
    {
        link_type x = (link_type)_x;
        link_type y = (link_type)_y;
        link_type z = create_node(v);

        if (y == header || x != 0 || key_compare(KeyOfValue()(v), value(y)))
        {
            left(y) = z;
            if (y == header)
            {
                root() = z;
                rightmost() = z;
            }
            else if (y == leftmost())
            {
                leftmost() = z;
            }
        }
        else
        {
            right() = z;
            if (y == rightmost())
            {
                rightmost() = z;
            }
        }

        parent(z) = y;
        left(z) = 0;
        right(z) = 0;

        __rb_tree_rebalance(z, header->parent); // 调整平衡
        ++node_count;
        return iterator(z);
    }
    
    template<typename key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void rb_tree<key, Value, KeyOfValue, Compare, Alloc>::__erase(link_type x) 
    {
        if(x->left==0 && x->right==0) //没有子结点
        {
            if(x->color==__rb_tree_red)
            {
                //todo
            }
        }
    }

    template <typename key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    auto rb_tree<key, Value, KeyOfValue, Compare, Alloc>::find(const key_type &k) -> iterator
    {
        link_type y = header;
        link_type x = root();

        while (x != 0)
        {
            if (!key_compare(key(x), k))
            {
                y = x;
                x = left(y);
            }
            else
            {
                x = right(x);
            }

            iterator j = iterator(y);
            return (j == end() || key_compare(k, key(y))) ? end() : j;
        }
    }

    template <typename key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    auto rb_tree<key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type &v) -> pair<iterator, bool>
    {
        link_type y = header;
        link_type x = root();
        bool comp = true;
        while (x != 0)
        {
            y = x;
            comp = key_compare(KeyOfValue()(v), key(x));
            x = comp ? left(x) : right(x);
        }
        // 循环结束，y是叶节点

        iterator j = iterator(y);
        if (comp)
        {                     // 应该插入于左侧
            if (j == begin()) //y最小
                return pair<iterator, bool>(__insert(x, y, v), true);
            else
                --j;
        }
        if (key_compare(key(j.node), KeyOfValue()(v)))
        {
            return pair<iterator, bool>(__insert(x, y, v), true);
        }

        return pair<iterator, bool>(j, false);
    }

    template <typename key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    auto rb_tree<key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type &v) -> iterator
    {
        link_type y = header;
        link_type x = root();
        while (x)
        {
            y = x;
            x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
        }
        return __insert(x, y, v);
    }
    
    template<typename key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void rb_tree<key, Value, KeyOfValue, Compare, Alloc>::erase(const value_type& x) 
    {
        
        link_type y = header;
        link_type x = root();
        bool comp = true;
        while (x != 0)
        {
            y = x;
            comp = key_compare(KeyOfValue()(v), key(x));
            x = comp ? left(x) : right(x);
        }
        // 循环结束，y是叶节点

        iterator j = iterator(y);
        if (comp)
        {                     // 应该插入于左侧
            if (j == begin()) //y最小
                return;
            else
                --j;
        }
        if (!key_compare(key(j.node), KeyOfValue()(v))) // 确定y的值是v
        {
            __erase((link_type)j.node);
        }
    }



} // namespace ydstl

#endif