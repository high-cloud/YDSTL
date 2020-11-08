#ifndef __YDSTL_VECTOR_H__
#define __YDSTL_VECTOR_H__

#include "ydstl_alloc.h"
#include "ydstl_construct.h"
#include "ydstl_uninitiallzed.h"

#include <assert.h>

// vector 容器的定义

//TODO: much
namespace ydstl
{

    template <typename T, typename Alloc = alloc>
    class vector
    {
    public:
        // 嵌套型别定义
        typedef T value_type;
        typedef value_type *pointer_type;
        typedef value_type *iterator;
        typedef value_type &reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

    protected:
        typedef simple_alloc<value_type, Alloc> data_allocator;

        iterator start;          //已用空间的开头
        iterator end;            //已用空间的尾
        iterator end_of_storage; //可用空间的尾

        void fill_initialized(size_type n, const T &value)
        {
            start = allocate_and_fill(n, value);
            end = start + n;
            end_of_storage = end;
        }
        void deallocate()
        {
            if (start)
            {
                data_allocator::deallocate(start, end_of_storage - start);
            }
        }
        iterator allocate_and_fill(size_t, const T &x)
        {
            iterator result = data_allocator::allocate(n);
            uninitialized_fill_n(result, n, x);
            return result;
        }

    public:
        iterator begin() { return start; }
        iterator end() { return end; }
        size_type size() const { return (size_type)(end - start); }
        size_type capacity() const { return (size_type)(end_of_storage - start); }
        bool empty() const { return start == end; }
        reference operator[](size_type n) { return *(begin() + n); }

        // constructor
        vector() : start(0), end(0), end_of_storage(0) {}
        vector(size_type n, const T &value) { fill_initialized(n, value); }
        explicit vector(size_type n) { fill_initialized(n, T()); }

        ~vector()
        {
            destroy(start, end);
            deallocate();
        }

        // 头尾元素
        reference front() { return *begin(); }
        reference back() { return *end(); }
        void push_back(const T &x)
        {
            if (end != end_of_storage)
            {
                construct(end, x);
                ++end;
            }
            else
            {
                allocate_insert(end, x);
            }
        }
        void pop_back()
        {
            assert(!empty());
            --end;
            destroy(end);
        }

        void insert(iterator position, size_type n, const T &x);

        // 清除某位置的元素
        iterator erase(iterator position)
        {
            if (position < end && position >= start)
            {
                copy(position + 1, end, position);
            }
            --end;
            destroy(end);
            return position;
        }

        void resize(size_type new_size, const T &x)
        {
            if (new_size < size())
            {
                erase(start + new_size, end);
            }
            else
            {
                insert(end(), new_size - size(), x);
            }
        }
        void resize(size_type new_size)
        {
            resize(new_size, T());
        }
        void clear()
        {
            destroy(start, end);
        }

    private:
        // helper function
        //
        void insert_aux(iterator position, const T &x)
      
    };

    template <typename T, typename Alloc>
    void vector<T, Alloc>::insert_aux(iterator position, const T &x)
    {
        const size_type old_size = size();
        const size_type len = old_size == 0 ? 1 : 2 * old_size;

        iterator new_start = data_allocator::allocate(len);
        iterator new_end = new_start;

        try
        {
            // 拷贝前面的内容
            new_end = uninitialized_copy(start, position, new_start);
            // 添加新元素
            construct(new_end, x);
            ++new_end;
            new_end = uninitialized_copy(position, end, new_end);
        }
        catch (...)
        {
            // commit or rollback
            destroy(new_start, new_end);
            data_allocator::deallocate(new_start, len);
            throw;
        }

        destroy(start, end);
        deallocate();

        start = new_start;
        end = new_end;
        end_of_storage = new_start + len;
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::insert(iterator position, size_type n, const T &x)
    {
        if (n != 0)
        {
            if (end_of_storage - end >= n)
            {
                //如果备用空间大于新增元素个数
                T x_copy = x;
                size_type elems_after = end - position;
                iterator old_end = end;
                if (elems_after > n)
                {
                    uninitialized_copy(end - n, end, end);
                    end += n;
                    copy_backward(position, old_end - n, old_end);
                    fill(position, position + n, x_copy);
                }
                else
                {
                    uninitialized_fill_n(end, n - elems_after, x_copy);
                    end += n - elems_after;
                    uninitialized_copy(position, old_end, end);
                    end += elems_after;
                    fill(position, old_end, x_copy);
                }
            }
            else
            {
                //备用空间小于添加元素数量

                const size_type old_size = size();
                const size_type len = old_size + max(old_size, n);

                iterator new_start = data_allocator::allocate(len);
                iterator new_end = new_start;

                try
                {
                    new_end = uninitialized_copy(start, position, new_start);
                    new_end = uninitialized_fill_n(new_end, n, x);
                    new_end = uninitialized_copy(position, end, new_end);
                }
                catch (...)
                {
                    destroy(new_start, new_end);
                    data_allocator::deallocate(new_start, len);
                    throw;
                }

                //清除原有空间
                destroy(start，end);
                deallocate();

                start = new_start;
                end = new_end;
                end_of_storage = start + len;
            }
        }
    }

} // namespace ydstl

#endif