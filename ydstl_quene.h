#ifndef YDSTL_QUENE_H
#define YDSTL_QUENE_H


namespace ydstl{
    template <typename T, typename Sequence = list<T>>
    class quene
    {
friend bool operator== <>(const quene&x,const quene&y);
friend bool operator< <>(const quene&x,const quene&y);

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
        reference top() { return c.front(); }
        const_reference top() const { return c.front(); }
        void push(const value_type &x) { c.push_back(x); }
        void pop() { c.pop_front(); }
    };

    template<typename T,typename Sequence>
    bool operator==(const quene<T,Sequence>&x,const quene<T,Sequence>&y)
    {
        return x.c==y.c;
    }
    
    template<typename T,typename Sequence>
    bool operator<(const quene<T,Sequence>&x,const quene<T,Sequence>&y)
    {
        return x.c<y.c;
    }



}

#endif