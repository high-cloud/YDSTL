#ifndef YDSTL_ALGORITHM_H
#define YDSTL_ALGORITHM_H

#include "ydstl_iterator.h"

namespace ydstl
{

    //-------------------------------------堆相关------------------------
    //------无仿函数版本，最大堆-----------

    //新元素已经插入，该函数让它重新成最大堆
    template <typename RandomAccessIterator>
    inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        __push_heap(first, last, difference_type(first), value_type(first));
    }

    template <typename RandomAccessIterator, typename Distance, typename T>
    inline void __push_heap(RandomAccessIterator first, RandomAccessIterator last, Distance *, T *)
    {
        __push_heap_aux(first, Distance(last - first), Distance(0), T(*(last - 1)));
    }

    template <typename RandomAccessIterator, typename Distance, typename T>
    void __push_heap_aux(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value)
    {
        Distance parent = (holeIndex - 1) / 2;
        while (holeIndex > topIndex && *(first + parent) < value)
        {
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    // 将最小值换到最后,并调整重新成堆,并不改变last
    template <typename RandomAccessIterator>
    inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        if(last-first<=1)
            return;
        __pop_heap(first, last, difference_type(first), value_type(first));
    }

    template <typename RandomAccessIterator, typename Distance, typename T>
    inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, Distance *, T *)
    {
        *(last - 1) = *first;
        __pop_heap_aux(first, Distance(0),Distance(last-first),T(*(first)));
    }

    template <typename RandomAccessIterator, typename Distance, typename T>
    void __pop_head_aux(RandomAccessIterator first, Distance topIndex, Distance len, T value)
    {
        Distance holeIndex = topIndex;
        Distance child = (holeIndex + 1) * 2; //右孩子
        while (child < len)
        {
            if (*(first + child) < *(first + child - 1))
            {
                child--;
            }
            if (*(first + child) > *(first + holeIndex))
            {
                *(first + holeIndex) = *(first + child);
                holeIndex = child;
                child = (holeIndex + 1) * 2;
            }
            else
            {
                break;
            }
        }
        if (child == len)
        {
            if (*(first + child) > *(first + holeIndex))
            {
                *(first + holeIndex) = *(first + child-1);
                holeIndex = child-1;
            }
        }

        *(first+holeIndex)=value;
    }

    template<typename RandomAccessIterator>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        while (last-first>1)
        {
            pop_heap(first,last--);
        }
        
    }

    template<typename RandomAccessIterator>
    inline void make_heap(RandomAccessIterator first,RandomAccessIterator last)
    {
        __make_heap(first,last,difference_type(first),value_type(first));
    }

    template<typename RandomAccessIterator,typename Distance,typename T>
    void __make_heap(RandomAccessIterator first,RandomAccessIterator last,Distance*,T*)
    {
        if(last-first<2) //如果元素只有一个或两个，不用排
            return;

        Distance len=last-first;
        Distance holeIndex=(len-1)/2; //第一个需要处理的序号
        while(true)
        {
            __pop_head_aux(first,holeIndex,len,T(*first+holeIndex));
            if(holeIndex=0) return;
            --holeIndex;
        }
    }

    //------有仿函数版本，最大堆-----------

    //新元素已经插入，该函数让它重新成最大堆
    template <typename RandomAccessIterator,typename Compare>
    inline void push_heap(RandomAccessIterator first, RandomAccessIterator last,Compare comp)
    {
        __push_heap(first, last, comp,difference_type(first), value_type(first));
    }

    template <typename RandomAccessIterator,typename Compare, typename Distance, typename T>
    inline void __push_heap(RandomAccessIterator first, Compare comp,RandomAccessIterator last, Distance *, T *)
    {
        __push_heap_aux(first, comp,Distance(last - first), Distance(0), T(*(last - 1)));
    }

    template <typename RandomAccessIterator,typename Compare, typename Distance, typename T>
    void __push_heap_aux(RandomAccessIterator first,Compare comp, Distance holeIndex, Distance topIndex, T value)
    {
        Distance parent = (holeIndex - 1) / 2;
        while (holeIndex > topIndex && comp(*(first + parent) , value))
        {
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    // 将最小值换到最后,并调整重新成堆,并不改变last
    template <typename RandomAccessIterator,typename Compare>
    inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last,Compare comp)
    {
        if(last-first<=1)
            return;
        __pop_heap(first, last, comp,difference_type(first), value_type(first));
    }

    template <typename RandomAccessIterator,typename Compare, typename Distance, typename T>
    inline void __pop_heap(RandomAccessIterator first, Compare comp, RandomAccessIterator last, Distance *, T *)
    {
        *(last - 1) = *first;
        __pop_heap_aux(first, Compare comp, Distance(0),Distance(last-first),T(*(first)));
    }

    template <typename RandomAccessIterator,typename Compare, typename Distance, typename T>
    void __pop_head_aux(RandomAccessIterator first,Compare comp,Distance topIndex, Distance len, T value)
    {
        Distance holeIndex = topIndex;
        Distance child = (holeIndex + 1) * 2; //右孩子
        while (child < len)
        {
            if (comp(*(first + child) , *(first + child - 1)))
            {
                --child;
            }
            if (comp(*(first + holeIndex),*(first+child) ) )
            {
                *(first + holeIndex) = *(first + child);
                holeIndex = child;
                child = (holeIndex + 1) * 2;
            }
            else
            {
                break;
            }
        }
        if (child == len)
        {
            if (comp(*(first + holeIndex),*(first+child) ))
            {
                *(first + holeIndex) = *(first + child-1);
                holeIndex = child-1;
            }
        }

        *(first+holeIndex)=value;
    }

    template<typename RandomAccessIterator,typename Compare>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last,Compare comp)
    {
        while (last-first>1)
        {
            pop_heap(first,last--,comp);
        }
        
    }

    template<typename RandomAccessIterator,typename Compare>
    inline void make_heap(RandomAccessIterator first,RandomAccessIterator last,Compare comp)
    {
        __make_heap(first,last,comp,difference_type(first),value_type(first));
    }

    template<typename RandomAccessIterator,typename Compare, typename Distance,typename T>
    void __make_heap(RandomAccessIterator first,Compare comp, RandomAccessIterator last,Distance*,T*)
    {
        if(last-first<2) //如果元素只有一个或两个，不用排
            return;

        Distance len=last-first;
        Distance holeIndex=(len-1)/2; //第一个需要处理的序号
        while(true)
        {
            __pop_head_aux(first,comp,holeIndex,len,T(*first+holeIndex));
            if(holeIndex=0) return;
            --holeIndex;
        }
    }

} // namespace ydstl

#endif