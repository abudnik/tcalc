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

template< class V1, class V2>
struct LessOrEqual
{
    enum { val = V1::val <= V2::val };
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

template< class V1, class V2 >
struct GreaterOrEqual
{
    enum { val = V1::val >= V2::val };
};

template< class V1, class V2 >
class Min
{
    template< class Vi1, class Vi2, int lesser >
    struct MinR;

    template< class Vi1, class Vi2 >
    struct MinR< Vi1, Vi2, 1 >
    {
        typedef Vi1 NextType;
    };

    template< class Vi1, class Vi2 >
    struct MinR< Vi1, Vi2, 0 >
    {
        typedef Vi2 NextType;
    };

public:
    typedef typename MinR< V1, V2, Less< V1, V2 >::val >::NextType NextType;
};

template< class V1, class V2 >
class Max
{
    template< class Vi1, class Vi2, int lesser >
    struct MaxR;

    template< class Vi1, class Vi2 >
    struct MaxR< Vi1, Vi2, 1 >
    {
        typedef Vi2 NextType;
    };

    template< class Vi1, class Vi2 >
    struct MaxR< Vi1, Vi2, 0 >
    {
        typedef Vi1 NextType;
    };

public:
    typedef typename MaxR< V1, V2, Less< V1, V2 >::val >::NextType NextType;
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

struct NullItem
{
    enum { val };
    static void Print() { cout << endl; }
};

template< typename V, typename L >
struct List
{
    enum { val = V::val };
    typedef V value_type;

    static void Print()
    {
        cout << val << ", ";
        L::Print();
    }
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

template< class L, class Item, unsigned index, unsigned length = ListLength< L >::val >
class InsertAt
{
    typedef typename First<L, index>::NextType L1;
    typedef typename Advance<L, index>::NextType L2;
    typedef typename PushBack< Item, L1 >::NextType L1PlusItem;
public:
    typedef typename Merge< L1PlusItem, L2 >::NextType NextType;
};

template< class L, class Item, unsigned index >
class InsertAt< L, Item, index, index >
{
public:
    typedef typename PushBack< Item, L >::NextType NextType;
};

template< class L, class Item, unsigned length >
class InsertAt< L, Item, 0, length >
{
public:
    typedef List< Item, L > NextType;
};

template< class L, unsigned index >
class EraseByIndex
{
    typedef typename First<L, index>::NextType L1;
    typedef typename Advance<L, index + 1>::NextType L2;
public:
    typedef typename Merge< L1, L2 >::NextType NextType;
};

template< class L >
class EraseByIndex< L, 0 >
{
public:
    typedef typename Next<L>::NextType NextType;
};

template< class Ls, class it >
class Search
{
    template< class L, class item, unsigned i, int eq >
    class SearchR;

    template< class L, class item, unsigned i >
    class SearchR< L, item, i, 1 >
    {
    public:
        enum { val = i };
    };

    template< class item, unsigned i >
    class SearchR< NullItem, item, i, 1 >
    {
    public:
        enum { val = i };
    };

    template< class item, unsigned i >
    class SearchR< NullItem, item, i, 0 >
    {
    public:
        enum { val = -1 };
    };

    template< class L, class item, unsigned i >
    class SearchR< L, item, i, 0 >
    {
        typedef typename Next<L>::NextType NextType;
    public:
        enum { val = SearchR< NextType, item, i + 1, Equal< NextType, item >::val >::val };
    };

public:
    enum { val = SearchR< Ls, it, 0, Equal< Ls, it >::val >::val };
};

template< class L, class Min = L >
class FindMinimum
{
    typedef typename Next<L>::NextType Forward;
    typedef typename tcalc::Min< L, Min >::NextType CurrentMin;
public:
    typedef typename FindMinimum< Forward, CurrentMin >::NextType NextType;
};

template< class Min >
class FindMinimum< NullItem, Min >
{
public:
    typedef Min NextType;
};

template< class Ls, class it >
class LowerBound
{
    template< class L, class item, unsigned i, int eq >
    class LowerR;

    template< class L, class item, unsigned i >
    class LowerR< L, item, i, 1 >
    {
    public:
        enum { val = i };
    };

    template< class item, unsigned i >
    class LowerR< NullItem, item, i, 1 >
    {
    public:
        enum { val = i };
    };

    template< class item, unsigned i >
    class LowerR< NullItem, item, i, 0 >
    {
    public:
        enum { val = i };
    };

    template< class L, class item, unsigned i >
    class LowerR< L, item, i, 0 >
    {
        typedef typename Next<L>::NextType NextType;
    public:
        enum { val = LowerR< NextType, item, i + 1, GreaterOrEqual< NextType, item >::val >::val };
    };

public:
    enum { val = LowerR< Ls, it, 0, GreaterOrEqual< Ls, it >::val >::val };
};


template< class L1, class L2 >
class MergeSorted
{
    typedef typename FindMinimum< L2 >::NextType E;
    typedef Search< L2, E > Found;
    typedef typename EraseByIndex< L2, Found::val >::NextType L2MinusMin;

    typedef LowerBound< L1, E > LB;
    typedef typename InsertAt< L1, E, LB::val >::NextType L1PlusMin;
public:
    typedef typename MergeSorted< L1PlusMin, L2MinusMin >::NextType NextType;
};

template< class L1 >
class MergeSorted< L1, NullItem >
{
public:
    typedef L1 NextType;
};

template< class L, unsigned length = ListLength< L >::val >
class MergeSort
{
    typedef ListLength< L > Length;
    typedef typename First< L, Length::val / 2 >::NextType L1;
    typedef typename Advance< L, Length::val / 2 >::NextType L2;

    typedef typename MergeSort< L1 >::NextType L1_sorted;
    typedef typename MergeSort< L2 >::NextType L2_sorted;
public:
    typedef typename MergeSorted< L1_sorted, L2_sorted >::NextType NextType;
};

template< class L >
class MergeSort< L, 2 >
{
    typedef L E1;
    typedef typename Next<L>::NextType E2;

    typedef typename Min<E1, E2>::NextType R1;
    typedef typename Max<E1, E2>::NextType R2;
public:
    typedef List< Value< int, R1::val >, List< Value< int, R2::val >, NullItem > > NextType; // TODO: int
};

template< class L >
class MergeSort< L, 1 >
{
public:
    typedef L NextType;
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

    typedef List< Value<int, 3>, List< Value<int, 2>, List< Value<int, 1>, NullItem > > > FunnyList;
    typedef List< Value<int, 5>, List< Value<int, 4>, List< Value<int, 6>, NullItem > > > FunnyList2;

    //cout << ListLength< FunnyList >::val << endl;
    //cout << Advance< FunnyList, 2 >::NextType::val << endl;

    //cout << Advance< typename PopBack< FunnyList >::NextType, 1 >::NextType::val << endl;
    //cout << Advance< typename PushBack< Value<int, 4>, FunnyList >::NextType, 2 >::NextType::val << endl;

    //typedef typename InsertAt< FunnyList, Value<int, 5>, 2 >::NextType WithVal;
    //WithVal::Print();

    typedef typename Merge< FunnyList, FunnyList2 >::NextType MergedList;
    //MergedList::Print();

    //typedef typename FindMinimum< FunnyList2 >::NextType E;
    //typedef Search< FunnyList2, E > Found;
    //typedef typename EraseByIndex< FunnyList2, Found::val >::NextType L2MinusMin;

    //typedef LowerBound< FunnyList, E > LB;
    //cout << LB::val << endl;
    //typedef typename InsertAt< FunnyList, E, LB::val >::NextType L1PlusMin;
    //L1PlusMin::Print();
    //typedef typename InsertAt< L1, E, LB::val >::NextType L1PlusMin;

    //L2MinusMin::Print();

    //typedef typename MergeSorted< FunnyList, FunnyList2 >::NextType MS;
    //MS::Print();
    typedef typename MergeSort< MergedList >::NextType Sorted;
    Sorted::Print();

    //cout << FindMinimum< MergedList >::NextType::val << endl;

    //cout << Advance< typename First< FunnyList, 2 >::NextType, 0 >::NextType::val << endl;

    //Advance< typename EraseByIndex< FunnyList, 1 >::NextType, 0 >::NextType::Print();

    //cout << Search< FunnyList, Value< int, 1 > >::val << endl;

    return 0;
}
