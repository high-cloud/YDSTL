#ifndef __YDSTL_HASHTABLE_H__
#define __YDSTL_HASHTABLE_H__

#include "ydstl_iterator.h"
#include "ydstl_alloc.h"
#include "ydstl_vector.h"
#include "ydstl_pair.h"

namespace ydstl
{

    template <typename Value>
    struct __hashtable_node
    {
        __hashtable_node *next;
        Value val;
    };

    template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc = alloc>
    class hashtable;

    template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
    struct __hashtable_iterator
    {
        typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
        typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
        typedef __hashtable_node<Value> node;

        typedef forward_iterator_tag iterator_category;
        typedef Value value_type;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef Value &reference;
        typedef Value *pointer;

        node *cur;    // the node iterator is pointing
        hashtable ht; // the hashtable iterator  belongs

        __hashtable_iterator(node *n, hashtable *tab) : cur(n), ht(tab) {}
        __hashtable_iterator() {}
        reference operator*() const { return cur->val; }
        pointer operator->() const { return &(cur->val); }
        iterator &operator++();   //todo
        iterator operator++(int); //todo
    };

    template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
    class hashtable
    {
    public:
        typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
        typedef Value value_type;
        typedef Key key_type;

        typedef HashFcn hasher;
        typedef EqualKey key_equal;
        typedef size_t size_type;

    private:
        hasher hash;
        key_equal equals;
        ExtractKey get_key;

        typedef __hashtable_node<value_type> node;
        typedef simple_alloc<node, Alloc> node_allocator;

        vector<node *, Alloc> buckets;
        size_type num_elements;

    private:
        // prime number relate to size of hash

        static const int __stl_num_primes = 28;
        static const unsigned long __stl_prime_list[__stl_num_primes] = {
            53, 97, 193, 389, 769,
            1543, 3079, 6151, 12289, 24593,
            49157, 98317, 196613, 393241, 786433,
            1572869, 3145739, 6291469, 12582917, 25165843,
            50331653, 100663319, 201326611, 402653189, 805306457,
            1610612741, 3221225473ul, 4294967291ul};

        // find the number closest to and bigger than n
        inline unsigned long __stl_next_prime(unsigned long n)
        {
            const unsigned long *first = __stl_prime_list;
            const unsigned long *last = __stl_prime_list + __stl_num_primes;
            const unsigned long *pos = lower_bound(first, last, n); // todo
            return pos == last ? *(last - 1) : pos;
        }

        inline size_type next_size(size_type n) const { return __stl_next_prime(n); }

        // return maxmum size of buckets
        size_t max_bucket_count() const
        {
            return __stl_prime_list[__stl_num_primes - 1];
        }

    public:
        // construct
        hashtable(size_type n, const HashFcn &hf, const EqualKey &eql) : hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0)
        {
            initialize_buckets(n);
        }

    private:
        // position relate

        size_type bkt_num(const value_type &obj, size_t n) const
        {
            return bkt_num_key(get_key(obj), n);
        }

        size_type bkt_num(const value_type &obj) const
        {
            return bkt_num_key(get_key(obj), buckets.size());
        }

        size_type bkt_num_key(const key_type &key, size_type n) const
        {
            return hash(key) % n;
        }

    private:
        // memory relate

        node *new_node(const value_type &obj)
        {
            node *n = node_allocator::allocate();
            n->next = 0;
            try
            {
                construct(&n->val, obj);
                return n;
            }
            catch (const std::exception &e)
            {
                //todo
                node_allocator::deallocate(n);
            }
        }

        void delete_node(node *n)
        {
            destroy(&n->val);
            node_allocator::deallocate(n);
        }

        void initialize_buckets(size_type n)
        {
            const size_type n_buckets = next_size(n);

            buckets.resize(n_buckets);
            buckets.insert(buckets.end(), n_buckets, (node *)0);
            num_elements = 0;
        }

    private:
        // judge if need resize
        void resize(size_type num_elements_hint);

        pair<iterator, bool> insert_unique_noresize(const value_type &obj);

    public:
        size_type bucket_count() const { return buckets.size(); }
        pair<iterator, bool> insert_unique(const value_type &obj){
            resize(num_elements+1);
            return insert_unique(obj);
        }
    };


    template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint)
    {

        const size_type old_n = buckets.size();
        // if hint is bigger than size, should resize
        if (num_elements_hint > old_n)
        {
            const size_type n = next_size(num_elements_hint);
            if (n > old_n)
            {
                vector<node *, Alloc> temp(n, (node *)0);
                for (size_type i = 0; i < old_n; ++i)
                {
                    node *first = buckets[i];
                    while (first)
                    {
                        size_type new_index = bkt_num(first->val, n);
                        buckets[i] = first->next;
                        first->next = temp[new_index];
                        temp[new_index] = first;
                        first = buckets[i];
                    }
                }
                buckets.swap(temp);
            }
        }
    }

    template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
    pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator,
         bool>
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique_noresize
    (const value_type &obj)
    {
        const size_type n=bkt_num(obj);
        node* first=buckets[n];
        node* cur=first;
        while (cur)
        {
            if(equals(cur->val,obj))
            {// already exists
                return pair<iterator,bool>(iterator(first,this),false);
            }
            cur=cur->next;
        }
        node* temp=new_node(obj);
        temp->next=first;
        buckets[n]=temp;
        ++num_elements;
        return pair<iterator,bool>(iterator(temp,this),true);
        
    }

} // namespace ydstl

#endif // __YDSTL_HASHTABLE_H__