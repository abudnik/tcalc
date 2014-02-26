#ifndef __TCALK_H
#define __TCALK_H

namespace tcalc {

template< typename T, T v = T() >
struct Value
{
    enum { val = v };
    typedef T value_type;
};


#define DECLARE_NAME( s, isVar )          \
    struct Value_##s {                    \
        static const char *val;           \
        typedef char *value_type;         \
        static const bool var = isVar;    \
    };                                    \
    const char *Value_##s::val = #s;      \

#define DECLARE_SYM( s ) DECLARE_NAME( s, false )
#define DECLARE_VAR( s ) DECLARE_NAME( s, true )

template< typename T1, typename T2>
struct TypeEqual
{
    enum { val = 0 };
};

template< typename T >
struct TypeEqual< T, T >
{
    enum { val = 1 };
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
template< typename T, T v >
struct Negate
{
    enum { val = -v };
};

template< class V1, class V2 >
struct Add
{
    typedef Value< typename V1::value_type, V1::val + V2::val > NextType;
};

template< class V1, class V2 >
struct Sub
{
    typedef Value< typename V1::value_type, V1::val - V2::val > NextType;
};

template< class V1, class V2 >
struct Mul
{
    typedef Value< typename V1::value_type, V1::val * V2::val > NextType;
};

template< class V1, class V2 >
struct Div
{
    typedef Value< typename V1::value_type, V1::val / V2::val > NextType;
};

template< class V, unsigned p >
struct Power
{
    typedef Value< typename V::value_type, Power< V, p - 1 >::NextType::val * V::val > NextType;
};

template< class V >
struct Power< V, 1 >
{
    typedef V NextType;
};

template< class V >
struct Power< V, 0 >
{
    typedef Value< typename V::value_type, 1 > NextType;
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
    static void Print() { std::cout << std::endl; }
};

template< typename V, typename L >
struct List
{
    typedef V value_type;

    static void Print()
    {
        std::cout << value_type::val << ", ";
        L::Print();
    }
};

template< class List > struct ListLength;

template<>
struct ListLength< NullItem >
{
    static const unsigned val = 0;
};

template< class V, class L >
struct ListLength< List< V, L > >
{
    static const unsigned val = 1 + ListLength<L>::val;
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

template< class L >
class PopBack
{
    template< class Li, int length >
    class PopBackR
    {
        typedef typename Next< Li >::NextType Remaining;
    public:
        typedef List< typename Li::value_type, typename PopBackR< Remaining, length - 1 >::NextType > NextType;
    };

    template< class Li >
    class PopBackR< Li, 2 >
    {
    public:
        typedef List< typename Li::value_type, NullItem > NextType;
    };

    template< class Li >
    class PopBackR< Li, 1 >
    {
    public:
        typedef NullItem NextType;
    };

public:
    typedef typename PopBackR< L, ListLength< L >::val >::NextType NextType;
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

template< class V >
class PushBack< V, NullItem >
{    
public:
    typedef List< V, NullItem > NextType;
};

template< unsigned n, int i = 0, int until = n + i, class G = NullItem >
class Generate
{
    typedef List< Value< int, i >, G > NewList;
public:
    typedef typename Generate< n, i + 1, until, NewList >::NextType NextType;
};

template< unsigned n, int until, class G >
class Generate<n, until, until, G>
{
public:
    typedef G NextType;
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
    template< class Li, unsigned ni, class Ri, unsigned ii >
    class FirstR
    {
        typedef typename PushBack< typename Li::value_type, Ri >::NextType Ls;
        typedef typename Next< Li >::NextType Forward;
    public:
        typedef typename FirstR< Forward, ni, Ls, ii + 1 >::NextType NextType;
    };

    template< class Li, unsigned ni, class Ri >
    class FirstR< Li, ni, Ri, ni >
    {
    public:
        typedef Ri NextType;
    };

    typedef List< typename L::value_type, NullItem > Trivial;
    typedef typename Next< L >::NextType Forward;
public:
    typedef typename FirstR< Forward, n, Trivial, 1 >::NextType NextType;
};

template< class L, class Item, unsigned index, unsigned length = ListLength< L >::val >
class InsertAt
{
    typedef typename First< L, index >::NextType L1;
    typedef typename Advance< L, index >::NextType L2;
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
class EraseAtIndex
{
    typedef typename First<L, index>::NextType L1;
    typedef typename Advance<L, index + 1>::NextType L2;
public:
    typedef typename Merge< L1, L2 >::NextType NextType;
};

template< class L >
class EraseAtIndex< L, 0 >
{
public:
    typedef typename Next<L>::NextType NextType;
};

template< class Ls, class it >
class Search
{
public:
    static const int NOT_FOUND = -1;

private:
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
        enum { val = NOT_FOUND };
    };

    template< class L, class item, unsigned i >
    class SearchR< L, item, i, 0 >
    {
        typedef typename Next<L>::NextType NextType;
    public:
        enum { val = SearchR< NextType, item, i + 1, Equal< typename NextType::value_type, item >::val >::val };
    };

public:
    enum { val = SearchR< Ls, it, 0, Equal< typename Ls::value_type, it >::val >::val };
};

template< class L, typename Min = typename L::value_type >
class FindMinimum
{
    typedef typename Next<L>::NextType Forward;
    typedef typename tcalc::Min< typename L::value_type, Min >::NextType CurrentMin;
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
        enum { val = i - 1 };
    };

    template< class L, class item >
    class LowerR< L, item, 0, 1 >
    {
    public:
        enum { val = 0 };
    };

    template< class item, unsigned i >
    class LowerR< NullItem, item, i, 1 >
    {
    public:
        enum { val = i - 1 };
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
        enum { val = LowerR< NextType, item, i + 1, Greater< typename L::value_type, item >::val >::val };
    };

public:
    enum { val = LowerR< Ls, it, 0, Greater< typename Ls::value_type, it >::val >::val };
};


template< class L1, class L2 >
class MergeSorted
{
    typedef typename Next< L2 >::NextType L2MinusMin;

    typedef typename L2::value_type Min;
    typedef LowerBound< L1, Min > LB;
    typedef typename InsertAt< L1, Min, LB::val >::NextType L1PlusMin;
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
    typedef typename L::value_type E1;
    typedef typename Next<L>::NextType Forward;
    typedef typename Forward::value_type E2;

    typedef typename Min<E1, E2>::NextType R1;
    typedef typename Max<E1, E2>::NextType R2;
public:
    typedef List< R1, List< R2, NullItem > > NextType;
};

template< class L >
class MergeSort< L, 1 >
{
public:
    typedef L NextType;
};

// functional
template< class L, template<class, class> class F, class BindParam >
class Map
{
    template< template<class, class> class T, class Param, class Value >
    struct Mutator : T< Param, Value >
    {
    };

    typedef typename L::value_type E;
    typedef typename Mutator< F, BindParam, E >::NextType Mut;

    typedef typename Next<L>::NextType Forward;
public:
    typedef List< Mut, typename Map< Forward, F, BindParam >::NextType > NextType;
};

template< template<class, class> class F, class BindParam >
class Map< NullItem, F, BindParam >
{
public:
    typedef NullItem NextType;
};

template< class L, template<class, class> class F, class Value >
class Fold
{
    template< template<class, class> class T, class Param, class V >
    struct Mutator : T< Param, V >
    {
    };

    typedef typename L::value_type E;
    typedef typename Mutator< F, Value, E >::NextType Mut;

    typedef typename Next<L>::NextType Forward;
public:
    typedef typename Fold< Forward, F, Mut >::NextType NextType;
};

template< template<class, class> class F, class Value >
class Fold< NullItem, F, Value >
{
public:
    typedef Value NextType;
};

template< class L, template<class, class> class LeftPredicate, class BindParam, class G = NullItem >
class Filter
{
    template< class Param, template<class, class> class P, class Value >
    struct Predicate : P< Value, Param >
    {
    };

    template< class Li, class V, int eq >
    struct Match
    {
        typedef typename PushBack< V, Li >::NextType NextType;
    };

    template< class Li, class V >
    struct Match< Li, V, 0 >
    {
        typedef Li NextType;
    };

    typedef typename L::value_type E;
    typedef Predicate< BindParam, LeftPredicate, E > AppliedPredicate;
    typedef typename Match< G, E, AppliedPredicate::val >::NextType Matched;

    typedef typename Next<L>::NextType Forward;
public:
    typedef typename Filter< Forward, LeftPredicate, BindParam, Matched >::NextType NextType;
};

template< template<class, class> class LeftPredicate, class BindParam, class G >
class Filter< NullItem, LeftPredicate, BindParam, G >
{
public:
    typedef G NextType;
};

} // namespace list

namespace map {

using namespace list; // map implemented as list of Key-Value pairs

template< typename K, typename V >
struct Pair
{
    typedef K Key;
    typedef V Value;
};

template< typename Pair >
struct GetPairValue
{
    typedef typename Pair::Value Value;
};

template<>
struct GetPairValue< NullItem >
{
    typedef NullItem Value;
};

template< typename Key, typename Map >
class FindKey
{
    template< typename K, typename M, int eq >
    struct FindKeyR
    {
        typedef typename M::value_type NextType;
    };

    template< typename K, typename M >
    struct FindKeyR< K, M, 0 >
    {
        typedef typename FindKey< K, typename Next< M >::NextType >::NextType NextType;
    };

    typedef typename Map::value_type Item;
    typedef typename Item::Key K;
public:
    typedef typename FindKeyR< Key, Map, TypeEqual< Key, K >::val >::NextType NextType;
};

template< typename Key >
class FindKey< Key, NullItem >
{
public:
    typedef NullItem NextType;
};

template< class Pair, typename Map, unsigned index = 0, typename Forward = Map >
class AddKey
{
    template< typename P, typename M, unsigned i, typename F, int eq > // eq => replace
    class AddKeyR
    {
        typedef typename InsertAt< M, P, i >::NextType Left;
        typedef typename First< Left, i + 1 >::NextType L1;
        typedef typename Advance< M, i + 1 >::NextType Right;
    public:
        typedef typename Merge< L1, Right >::NextType NextType;
    };

    template< typename P, typename M, unsigned i, typename F >
    class AddKeyR< P, M, i, F, 0 >
    {
    public:
        typedef typename AddKey< P, M, i + 1, typename Next< F >::NextType >::NextType NextType;
    };

    template< typename P, unsigned i, typename F, int eq > // empty map
    class AddKeyR< P, NullItem, i, F, eq >
    {
    public:
        typedef List< P, NullItem > NextType;
    };

    typedef typename Forward::value_type Item;
    typedef typename Item::Key K;
    typedef typename Pair::Key Key;
public:
    typedef typename AddKeyR< Pair, Map, index, Forward, TypeEqual< K, Key >::val >::NextType NextType;
};

template< class Pair, typename Map, unsigned index >
class AddKey< Pair, Map, index, NullItem >
{
public:
    typedef List< Pair, Map > NextType;
};

template< class Key, typename Map, unsigned index = 0, typename Forward = Map >
class EraseKey
{
    template< typename K, typename M, unsigned i, typename F, int eq >
    class EraseKeyR
    {
    public:
        typedef typename EraseAtIndex< M, i >::NextType NextType;
    };

    template< typename K, typename M, unsigned i, typename F >
    class EraseKeyR< K, M, i, F, 0 >
    {
    public:
        typedef typename EraseKey< K, M, i + 1, typename Next< F >::NextType >::NextType NextType;
    };

    template< typename K, unsigned i, typename F, int eq > // empty map
    class EraseKeyR< K, NullItem, i, F, eq >
    {
    public:
        typedef NullItem NextType;
    };

    typedef typename Forward::value_type Item;
    typedef typename Item::Key K;
public:
    typedef typename EraseKeyR< Key, Map, index, Forward, TypeEqual< K, Key >::val >::NextType NextType;
};

template< class Key, typename Map, unsigned index >
class EraseKey< Key, Map, index, NullItem >
{
public:
    typedef Map NextType;
};

template< typename Map1, typename Map2 >
class MergeMap
{
    typedef typename Map2::value_type Pair2;
    typedef typename Next<Map2>::NextType Forward;

    typedef typename AddKey< Pair2, Map1 >::NextType MergedWithFirst;
public:
    typedef typename MergeMap< MergedWithFirst, Forward >::NextType NextType;
};

template< typename Map1 >
class MergeMap< Map1, NullItem >
{
public:
    typedef Map1 NextType;
};

template< typename Map2 >
class MergeMap< NullItem, Map2 >
{
public:
    typedef Map2 NextType;
};

template<>
class MergeMap< NullItem, NullItem >
{
public:
    typedef NullItem NextType;
};

template< typename Map >
class PrintMap
{
    typedef typename Map::value_type PairType;
    typedef typename PairType::Key K;
    typedef typename PairType::Value V;

    typedef typename Next< Map >::NextType Forward;
public:
    static void Print()
    {
        std::cout << "(" << K::val << ", " << V::val << "),";
        PrintMap< Forward >::Print();
    }
};

template<>
class PrintMap< NullItem >
{
public:
    static void Print()
    {
        std::cout << std::endl;
    }
};

} // namespace map

} // namespace container

namespace prolog {

using namespace container::list;
using namespace container::map;

template< typename Predicate, typename ArgList >
struct Term
{
    typedef Predicate Pred;
    typedef ArgList   Args;
};

template< typename Term, typename TermList >
struct Rule
{
    typedef Term     Head;
    typedef TermList Goals;
};

template< typename Rule, typename Parent = NullItem, typename Env = NullItem, unsigned i = 0 >
struct Goal
{
    typedef Rule   R;
    typedef Parent P;
    typedef Env    E;
    static const unsigned index = i;
};


template< typename SrcTerm, typename SrcEnv, typename DestTerm, typename DestEnv >
class Unify
{
    template< typename ST, typename SE, typename DT, typename DE, unsigned nArgs, int i >
    class UnifyR
    {
        typedef typename Advance< typename ST::Args, i >::NextType STT;
        typedef typename STT::value_type SrcArg;
        enum { sVar = SrcArg::var };

        typedef typename Advance< typename DT::Args, i >::NextType DTT;
        typedef typename DTT::value_type DestArg;
        enum { dVar = DestArg::var };

        template< typename sArg, typename sEnv, int isVar >
        struct DefineSrcVal
        {
            typedef typename FindKey< sArg, sEnv >::NextType FoundPair;
            typedef typename GetPairValue< FoundPair >::Value Val;
        };

        template< typename sArg, typename sEnv >
        struct DefineSrcVal< sArg, sEnv, 0 >
        {
            typedef sArg Val;
        };

        typedef typename DefineSrcVal< SrcArg, SE, sVar >::Val SrcVal;


        template< typename STi, typename SEi, typename DTi, typename DEi, unsigned nArgsi, int ii, int eq >
        class CompareWithSrcVal
        {
            typedef UnifyR< STi, SEi, DTi, DEi, nArgsi, ii + 1 > Result; // next iteration
        public:
            typedef typename Result::ResultEnv ResultEnv;
            enum { ret = Result::ret };
        };

        template< typename STi, typename SEi, typename DTi, typename DEi, unsigned nArgsi, int ii >
        class CompareWithSrcVal< STi, SEi, DTi, DEi, nArgsi, ii, 0 >
        {
        public:
            typedef DEi ResultEnv;
            enum { ret = 0 };
        };

        template< typename STi, typename SEi, typename DTi, typename DEi, unsigned nArgsi, int ii, typename DestVal, int eq >
        class CheckDestVal
        {
            typedef typename AddKey< Pair< DestArg, SrcVal >, DEi >::NextType Updated;
            typedef UnifyR< STi, SEi, DTi, Updated, nArgsi, ii + 1 > Result; // next iteration
        public:
            typedef typename Result::ResultEnv ResultEnv;
            enum { ret = Result::ret };
        };

        template< typename STi, typename SEi, typename DTi, typename DEi, unsigned nArgsi, int ii, typename DestVal >
        class CheckDestVal< STi, SEi, DTi, DEi, nArgsi, ii, DestVal, 0 >
        {
            typedef CompareWithSrcVal< STi, SEi, DTi, DEi, nArgsi, ii, TypeEqual< DestVal, SrcVal >::val > Result;
        public:
            typedef typename Result::ResultEnv ResultEnv;
            enum { ret = Result::ret };
        };

        template< typename STi, typename SEi, typename DTi, typename DEi, unsigned nArgsi, int ii, int eq >
        class CheckDestArg
        {
            typedef typename FindKey< DestArg, DE >::NextType DestVal;
            typedef CheckDestVal< STi, SEi, DTi, DEi, nArgsi, ii, DestVal, TypeEqual< DestVal, NullItem >::val > Result;
        public:
            typedef typename Result::ResultEnv ResultEnv;
            enum { ret = Result::ret };
        };

        template< typename STi, typename SEi, typename DTi, typename DEi, unsigned nArgsi, int ii >
        class CheckDestArg< STi, SEi, DTi, DEi, nArgsi, ii, 0 >
        {
            typedef CompareWithSrcVal< STi, SEi, DTi, DEi, nArgsi, ii, TypeEqual< DestArg, SrcVal >::val > Result;
        public:
            typedef typename Result::ResultEnv ResultEnv;
            enum { ret = Result::ret };
        };

        template< typename STi, typename SEi, typename DTi, typename DEi, unsigned nArgsi, int ii, int eq >
        class CheckSrcVal
        {
            typedef UnifyR< STi, SEi, DTi, DEi, nArgsi, ii + 1 > Result; // next iteration
        public:
            typedef typename Result::ResultEnv ResultEnv;
            enum { ret = Result::ret };
        };

        template< typename STi, typename SEi, typename DTi, typename DEi, unsigned nArgsi, int ii >
        class CheckSrcVal< STi, SEi, DTi, DEi, nArgsi, ii, 0 >
        {
            typedef CheckDestArg< STi, SEi, DTi, DEi, nArgsi, ii, dVar > Result;
        public:
            typedef typename Result::ResultEnv ResultEnv;
            enum { ret = Result::ret };
        };

        typedef CheckSrcVal< ST, SE, DT, DE, nArgs, i, TypeEqual< SrcVal, NullItem >::val > Result;
    public:
        typedef typename Result::ResultEnv ResultEnv;
        enum { ret = Result::ret };
    };

    template< typename ST, typename SE, typename DT, typename DE, unsigned nArgs >
    class UnifyR< ST, SE, DT, DE, nArgs, nArgs >
    {
    public:
        typedef DE ResultEnv;
        enum { ret = 1 };
    };

    template< typename ST, typename SE, typename DT, typename DE, unsigned nArgs, int eq >
    class CmpPred
    {
        typedef UnifyR< ST, SE, DT, DE, nArgs, 0 > Result;
    public:
        typedef typename Result::ResultEnv ResultEnv;
        enum { ret = Result::ret };
    };

    template< typename ST, typename SE, typename DT, typename DE, unsigned nArgs >
    class CmpPred< ST, SE, DT, DE, nArgs, 0 >
    {
    public:
        typedef DE ResultEnv;
        enum { ret = 0 };
    };

    template< typename ST, typename SE, typename DT, typename DE, unsigned nSrcArgs, int eq >
    class CmpArgs
    {
        typedef TypeEqual< typename ST::Pred, typename DT::Pred > ComparePredicates;
        typedef CmpPred< ST, SE, DT, DE, nSrcArgs, ComparePredicates::val > Result;
    public:
        typedef typename Result::ResultEnv ResultEnv;
        enum { ret = Result::ret };
    };

    template< typename ST, typename SE, typename DT, typename DE, unsigned nSrcArgs >
    class CmpArgs< ST, SE, DT, DE, nSrcArgs, 0 >
    {
    public:
        typedef DE ResultEnv;
        enum { ret = 0 };
    };

    typedef typename SrcTerm::Args SrcArgs;
    typedef typename DestTerm::Args DestArgs;
    typedef CmpArgs< SrcTerm, SrcEnv, DestTerm, DestEnv, ListLength<SrcArgs>::val,
                     ListLength<SrcArgs>::val == ListLength<DestArgs>::val > Result;
public:
    typedef typename Result::ResultEnv ResultEnv;
    enum { ret = Result::ret };
};


DECLARE_SYM( got_ );
DECLARE_SYM( goal_ );

template< typename SearchTerm, typename GlobalRules >
class SearchGoal
{
    typedef Term< Value_got_, List< Value_goal_, NullItem > > GoalTerm;
    typedef Rule< GoalTerm, List< SearchTerm, NullItem > > MainRule;
    typedef Goal< MainRule > MainGoal;

    template< typename GoalStack, typename Rules >
    class SearchR
    {
        template< typename GS, typename G, int eq >
        class CheckOriginalGoal
        {
            typedef SearchR< GS, Rules > Result;
        public:
            typedef typename Merge< typename G::E, typename Result::ResultEnv >::NextType ResultEnv;
            enum { ret = 1 };
        };

        template< typename GS, typename G >
        class CheckOriginalGoal< GS, G, 0 >
        {
            typedef typename G::P P;
            typedef typename G::R Rule;

            typedef typename P::R PRule;
            typedef typename PRule::Goals PRuleGoals;
            typedef typename Advance< PRuleGoals, P::index >::NextType PRuleGoalsOffset;
            typedef typename PRuleGoalsOffset::value_type PGoal;

            typedef Unify< typename Rule::Head, typename G::E, PGoal, typename P::E > Unified;

            typedef Goal< PRule, typename P::P, typename Unified::ResultEnv, P::index + 1 > NewParent;

            typedef List< NewParent, GS > PushNewParent;

            typedef SearchR< PushNewParent, Rules > Result;
        public:
            typedef typename Result::ResultEnv ResultEnv;
            enum { ret = Result::ret };
        };

        template< typename GS, bool ge >
        class CheckGoalIndex
        {
            typedef typename GS::value_type G;
            typedef typename G::P P;
            typedef typename Next< GS >::NextType PopG;

            typedef CheckOriginalGoal< PopG, G, TypeEqual< P, NullItem >::val > Result;
        public:
            typedef typename Result::ResultEnv ResultEnv;
            enum { ret = Result::ret };
        };

        template< typename G, typename GS, typename T, typename CurrentR >
        class IterateRules
        {
            template< typename GSi, typename Child, int eq >
            class CheckAnswer
            {
                typedef List< Child, GSi > PushChild;
                typedef IterateRules< G, PushChild, T, typename Next<CurrentR>::NextType > Result;
            public:
                typedef typename Result::ResultEnv ResultEnv;
                enum { ret = Result::ret };
            };

            template< typename GSi, typename Child >
            class CheckAnswer< GSi, Child, 0 >
            {
                typedef IterateRules< G, GSi, T, typename Next<CurrentR>::NextType > Result;
            public:
                typedef typename Result::ResultEnv ResultEnv;
                enum { ret = Result::ret };
            };

            template< typename GSi, int eq >
            class CheckArgs
            {
                typedef typename CurrentR::value_type CurrentRule;

                typedef Unify< T, typename G::E, typename CurrentRule::Head, NullItem > Unified;
                typedef Goal< CurrentRule, G, typename Unified::ResultEnv > Child;

                typedef CheckAnswer< GSi, Child, Unified::ret > Result;
            public:
                typedef typename Result::ResultEnv ResultEnv;
                enum { ret = Result::ret };
            };

            template< typename GSi >
            class CheckArgs< GSi, 0 >
            {
                typedef IterateRules< G, GSi, T, typename Next<CurrentR>::NextType > Result;
            public:
                typedef typename Result::ResultEnv ResultEnv;
                enum { ret = Result::ret };
            };

            template< typename GSi, int eq >
            class CheckPredicates
            {
                typedef typename CurrentR::value_type CurrentRule;
                typedef typename CurrentRule::Head CurrentRuleHead;

                typedef CheckArgs< GSi, ListLength< typename T::Args >::val == ListLength< typename CurrentRuleHead::Args >::val > Result;
            public:
                typedef typename Result::ResultEnv ResultEnv;
                enum { ret = Result::ret };
            };

            template< typename GSi >
            class CheckPredicates< GSi, 0 >
            {
                typedef IterateRules< G, GSi, T, typename Next<CurrentR>::NextType > Result;
            public:
                typedef typename Result::ResultEnv ResultEnv;
                enum { ret = Result::ret };
            };

            typedef typename CurrentR::value_type CurrentRule;
            typedef typename CurrentRule::Head CurrentRuleHead;

            typedef CheckPredicates< GS, TypeEqual< typename CurrentRuleHead::Pred, typename T::Pred >::val > Result;
        public:
            typedef typename Result::ResultEnv ResultEnv;
            enum { ret = Result::ret };
        };

        template< typename G, typename GS, typename T >
        class IterateRules< G, GS, T, NullItem >
        {
            typedef SearchR< GS, Rules > Result;
        public:
            typedef typename Result::ResultEnv ResultEnv;
            enum { ret = Result::ret };
        };

        template< typename GS >
        class CheckGoalIndex< GS, false >
        {
            typedef typename GS::value_type G;
            typedef typename Next< GS >::NextType PopG;

            typedef typename G::R Rule;
            typedef typename Rule::Goals RGoals;
            typedef typename Advance< RGoals, G::index >::NextType RGoalsOffset;
            typedef typename RGoalsOffset::value_type T;

            typedef IterateRules< G, PopG, T, Rules > Result;
        public:
            typedef typename Result::ResultEnv ResultEnv;
            enum { ret = Result::ret };
        };

        typedef typename GoalStack::value_type G;
        typedef typename G::R R;
        typedef typename R::Goals SubGoals;

        typedef CheckGoalIndex< GoalStack, ( G::index >= ListLength<SubGoals>::val ) > Result;
    public:
        typedef typename Result::ResultEnv ResultEnv;
        enum { ret = Result::ret };
    };

    template< typename Rules >
    class SearchR< NullItem, Rules >
    {
    public:
        typedef NullItem ResultEnv;
        enum { ret = 0 };
    };

    typedef SearchR< List< MainGoal, NullItem >, GlobalRules > Result;
public:
    typedef typename Result::ResultEnv ResultEnv;
    enum { ret = Result::ret };
};

} // namespace prolog

} // namespace tcalc

#endif
