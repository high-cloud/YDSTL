#ifndef __YDSTL_ALLOC_H__
#define __YDSTL_ALLOC_H__

#include <stdlib.h>
#include <stdio.h>
#include<new>

namespace ydstl
{

    // 接口
    template <typename T, typename Alloc>
    class simple_alloc
    {
    public:
        static T *allocate(size_t n)
        {
            return n == 0 ? 0 : Alloc::allocate(n * sizeof(T));
        }
        static T *allocate(void)
        {
            return (T *)Alloc::allocate(sizeof(T));
        }

        void deallocate(T *)
        {
            Alloc::deallocate(p, sizeof(T));
        }
        void deallocate(T *, size_t n)
        {
            Alloc::deallocate(p, n * sizeof(T));
        }
    };
    
    // ----------------------------------------------------------------------------------------------
    // 第一级配置器,使用malloc

    class __maloc_alloc
    {
    public:
        static void *allocate(size_t n)
        {
            void *result = malloc(n);
            if (0 == result)
            {
                printf("BAD_ALLOC");
                exit(1);
            }

            return result;
        }

        static void deallocate(void *p, size_t)
        {
            free(p);
        }

        static void *reallocate(void *p, size_t old_sz, size_t new_sz)
        {
            void *result = realloc(p, new_sz);
            if (0 == result)
            {
                printf("/nBAD_REALLOCATE/n");
                exit(1);
            }

            return result;
        }
    };

    // 自由链表，管理内存池
    union FreeList
    {
        FreeList *next; //指向
        char data[1];   // dont konw what for now
    };

    enum
    {
        __ALIGN = 8,
        __MAX_BYTES = 128,
        __NFREELISTS = __MAX_BYTES / __ALIGN,
    };
    // 配置器
    class alloc
    {
    private:
        static char *start_free; // 内存池起始位置
        static char *end_free;   // 内存池结束位置
        static size_t heap_size; //申请附加值大小

        static FreeList *freelists[__NFREELISTS]; //自由链表，管理已经申请得空间

    private:
        // 向上对齐
        static size_t round_up(size_t n)
        {
            return ((n - 1 + __ALIGN) & ~(__ALIGN - 1));
        }
        // 返回合适index
        static size_t freelists_index(size_t bytes)
        {
            return (bytes - 1) / __ALIGN;
        }

        static void *refill(size_t size);
        static char *chunk_alloc(size_t size, int &n);

    public:
        static void *allocate(size_t n);
        static void deallocate(void *p, size_t size);
        static void *reallocate(void *p, size_t old_size, size_t new_size);
    };

    char *alloc::start_free = 0;
    char *alloc::end_free = 0;
    size_t alloc::heap_size = 0;

    FreeList *alloc::freelists[__NFREELISTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    // allocate 首先判断是否大于__MAX-BYTES，再决定采用何种方式申请空间
    void *alloc::allocate(size_t n)
    {
        if (n > 128)
        {
            return __maloc_alloc::allocate(n);
        }

        FreeList *my_free_list = freelists[freelists_index(n)];
        FreeList *result = my_free_list;
        if (result == 0)
        {
            void *r = refill(round_up(n));
            return r;
        }
        my_free_list = my_free_list->next;
        return result;
    }

    void alloc::deallocate(void *p, size_t size)
    {
        if (size > __MAX_BYTES)
        {
            __maloc_alloc::deallocate(p, size);
            return;
        }

        FreeList *q = (FreeList *)p, *my_free_list = freelists[freelists_index(size)];
        q->next = my_free_list;
        my_free_list = q;
    }

    void *alloc::reallocate(void *p, size_t old_size, size_t new_size)
    {
        deallocate(p, old_size);
        p = allocate(new_size);
        return p;
    }

    void* alloc::refill(size_t size)
    {
        int n = 20; // 默认申请 20 块
        char *result = chunk_alloc(size, n);

        //判断申请到几块
        if (n == 1)
        {
            return result;
        }

        // 将多余的交给自由链表管理
        FreeList *my_free_list = freelists[freelists_index(size)];
        FreeList *cur;
        FreeList *next = (FreeList *)(result + size);
        my_free_list = next;
        while (1 < n--)
        {
            cur = next;
            next = (FreeList *)((char *)next + size);
            cur->next = next;
        }
        cur->next = 0;

        return result;
    }

    char *alloc::chunk_alloc(size_t size, int &n)
    {
        size_t total_bytes = end_free - start_free; //内存池有多少
        size_t demand_bytes = size * n;             //需要多少
        char *result;

        //如果足够分配
        if (total_bytes >= demand_bytes)
        {
            result = start_free;
            start_free += demand_bytes;
            return result;
        }

        //如果不够n个，但足够至少一个块
        if (total_bytes >= size)
        {
            n = total_bytes / size;
            result = start_free;
            start_free += n * size;
            return result;
        }

        // 连一块也拿不出来
        //首先清空旧的内存池
        if (total_bytes > 0)
        {
            FreeList *my_free_list = freelists[freelists_index(total_bytes)];
            ((FreeList *)start_free)->next = my_free_list;
            my_free_list = (FreeList *)start_free;
        }
        // 申请heap空间
        size_t bytes_to_get = (demand_bytes << 1) + round_up(heap_size >> 4);
        start_free = (char *)malloc(bytes_to_get);
        // 如果申请不成功
        if (!start_free)  
        {
            //尝试freelist管理的更大的块
            FreeList *my_free_list,*p;
            for (int i = size + __ALIGN; i <= __MAX_BYTES; i += __ALIGN)
            {
                my_free_list = freelists[freelists_index(i)];
                if(!my_free_list)
                    continue;
                p=my_free_list;
                my_free_list=my_free_list->next;
                start_free=(char*)p;
                end_free=start_free+i;
                return chunk_alloc(size,n);
            }
            // 这也不成功
            printf("ERROR:: alloc::chunk_alloc out of memory/n");
            end_free=nullptr;
            throw std::bad_alloc();
        }

        heap_size+=bytes_to_get;
        end_free=start_free+bytes_to_get;
        return chunk_alloc(size,n);
    }

} //end namespace ydstl

#endif