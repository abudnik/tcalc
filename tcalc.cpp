#include <iostream>

using namespace std;


namespace tcalc {

template< typename T, T v = T() >
struct Value
{
    enum { val = v };
    typedef T value_type;
};

template< typename T, T v >
struct Negate
{
    enum { val = -v };
};

// comparision
template< class V1, class V2>
struct Less
{
    enum { val = V1::val < V2::val };
};

template< class V1, class V2 >
struct Equal
{
    enum { val = V1::val == V2::val };
};

template< class V1, class V2 >
struct Greater
{
    enum { val = V1::val > V2::val };
};

// arithmetic
template< class V1, class V2 >
struct Add
{
    enum { val = V1::val + V2::val };
};

template< class V1, class V2 >
struct Sub
{
    enum { val = V1::val - V2::val };
};

template< class V1, class V2 >
struct Mul
{
    enum { val = V1::val * V2::val };
};

template< class V1, class V2 >
struct Div
{
    enum { val = V1::val / V2::val };
};

template< class V, unsigned p >
struct Power
{
    enum { val = Power< V, p - 1 >::val * V::val };
};

template< class V >
struct Power< V, 1 >
{
    enum { val = V::val };
};

template< class V >
struct Power< V, 0 >
{
    enum { val = 1 };
};

// special-purpose
template< unsigned v >
struct Factorial
{
    enum { val = Factorial< v - 1 >::val * v };
};

template<>
struct Factorial<0>
{
    enum { val = 1 };
};

////////////////////////////////
namespace container {

namespace list {

struct NullItem {};

template< typename V, typename L >
struct List
{
    enum { val = V::val };
    typedef V value_type;
};

template< class List > struct ListLength;

template<>
struct ListLength< NullItem >
{
    enum { val = 0 };
};

template< class V, class L >
struct ListLength< List< V, L > >
{
    enum { val = 1 + ListLength<L>::val };
};

template< class List > struct Next;

template< class V, class L >
struct Next< List< V, L > >
{
    typedef L NextType;
};

template< class L, int step >
class Advance
{
    typedef typename Next< L >::NextType Forward;
public:
    typedef typename Advance< Forward, step - 1 >::NextType NextType;
};

template< class L >
class Advance< L, 1 >
{
public:
    typedef typename Next< L >::NextType NextType;
};

template< class L >
class Advance< L, 0 >
{
public:
    typedef L NextType;
};

template< class L, int length = ListLength< L >::val >
class Reverse
{
    typedef typename Advance< L, length - 1 >::NextType::value_type Last;
public:
    typedef List< Last, typename Reverse< L, length - 1 >::NextType > NextType;
};

template< class L >
class Reverse< L, 1 >
{
    typedef typename L::value_type First;
public:
    typedef List< First, NullItem > NextType;
};

template< class L, int length = ListLength< L >::val >
class PopBack
{
    typedef typename Next< L >::NextType Remaining;
public:
    typedef List< typename L::value_type, typename PopBack< Remaining, length - 1 >::NextType > NextType;
};

template< class L>
class PopBack< L, 2 >
{
public:
    typedef List< typename L::value_type, NullItem > NextType;
};

template< class L >
struct PopFront
{
    typedef typename Next< L >::NextType NextType;
};

template< class V, class L >
class PushBack
{
    typedef typename Reverse< L >::NextType Reversed;
    typedef List< V, Reversed > ReversedA;
public:
    typedef typename Reverse< ReversedA >::NextType NextType;
};

template< class L1, class L2 >
class Merge
{
    typedef typename PushBack< typename L2::value_type, L1 >::NextType MergedWithFirst;
    typedef typename Next< L2 >::NextType L2_NextType;
public:
    typedef typename Merge< MergedWithFirst, L2_NextType >::NextType NextType;
};

template< class L1 >
class Merge< L1, NullItem >
{
public:
    typedef L1 NextType;
};

template< class L, unsigned n, unsigned i = 0 >
class First
{
    typedef List< typename L::value_type, NullItem > Trivial;

    template< class Li, unsigned ni, class Ri, unsigned ii >
    class FirstR
    {
        enum { index = ni - ii };
        typedef typename PushBack< typename Advance< Li, index >::NextType, Ri >::NextType Ls;
    public:
        typedef typename FirstR< Li, ni, Ls, ii - 1 >::NextType NextType;
    };

    template< class Li, unsigned ni, class Ri >
    class FirstR< Li, ni, Ri, 0 >
    {
    public:
        typedef Ri NextType;
    };

public:
    typedef typename FirstR< L, n, Trivial, n - 1 >::NextType NextType;
};

template< class L, int index >
class EraseByIndex
{
    typedef typename First<L, index>::NextType L1;
    typedef typename Advance<L, index + 1>::NextType L2;
public:
    typedef typename Merge< L1, L2 >::NextType NextType;
};

} // namespace list

} // namespace container

} // namespace tcalc


int main()
{
    using namespace tcalc;
    using namespace tcalc::container::list;
    const int v = Add< Value<int, 3>, Value<int, 42> >::val;
    const int ret = Power< Div< Value<int, 42>, Value<int, 21> >, Value<int, 8>::val >::val;

    typedef List< Value<int, 1>, List< Value<int, 2>, List< Value<int, 3>, NullItem > > > FunnyList;
    typedef List< Value<int, 4>, List< Value<int, 5>, List< Value<int, 6>, NullItem > > > FunnyList2;

    //cout << ListLength< FunnyList >::val << endl;
    //cout << Advance< FunnyList, 2 >::NextType::val << endl;

    //cout << Advance< typename PopBack< FunnyList >::NextType, 1 >::NextType::val << endl;
    //cout << Advance< typename PushBack< Value<int, 4>, FunnyList >::NextType, 2 >::NextType::val << endl;
    //cout << Advance< Merge< FunnyList, FunnyList2 >::Merged, 4 >::NextType::val << endl;

    //cout << Advance< typename First< FunnyList, 2 >::NextType, 0 >::NextType::val << endl;

    cout << Advance< typename EraseByIndex< FunnyList, 1 >::NextType, 1 >::NextType::val << endl;

    return 0;
}
