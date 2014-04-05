#include <iostream>
#include "tcalc.h"

using namespace std;

// symbolic & variable type declaration
namespace tcalc
{
    DECLARE_SYM( child );
    DECLARE_SYM( boy );
    DECLARE_SYM( girl );
    DECLARE_SYM( bill );
    DECLARE_SYM( frank );
    DECLARE_SYM( alice );
    DECLARE_SYM( alex );
    DECLARE_VAR( X );
    DECLARE_VAR( G );
}

void TestProlog()
{
    using namespace tcalc;
    using namespace tcalc::container::list;
    using namespace tcalc::prolog;

    // test unification
    typedef Term< Value_boy, List< Value_bill, NullItem > > TermA;
    typedef Term< Value_boy, List< Value_frank, NullItem > > TermB;
    typedef Term< Value_boy, List< Value_X, NullItem > > TermX;
    typedef Term< Value_boy, List< Value_G, NullItem > > TermG;

    typedef Unify< TermA, NullItem, TermB, NullItem > Unified1;
    cout << Unified1::ret << endl;
    PrintMap< Unified1::ResultEnv >::Print();

    typedef Unify< TermA, NullItem, TermA, NullItem > Unified2;
    cout << Unified2::ret << endl;
    PrintMap< Unified2::ResultEnv >::Print();

    typedef Unify< TermA, NullItem, TermX, NullItem > Unified3;
    cout << Unified3::ret << endl;
    PrintMap< Unified3::ResultEnv >::Print();

    typedef Unify< TermB, NullItem, TermX, Unified3::ResultEnv > Unified4;
    cout << Unified4::ret << endl;
    PrintMap< Unified4::ResultEnv >::Print();

    typedef Unify< TermG, NullItem, TermX, NullItem > Unified5;
    cout << Unified5::ret << endl;
    PrintMap< Unified5::ResultEnv >::Print();

    typedef List< Pair< Value_G, Value_frank >, NullItem > Env;
    typedef Unify< TermG, Env, TermX, NullItem > Unified6;
    cout << Unified6::ret << endl;
    PrintMap< Unified6::ResultEnv >::Print();

    cout << "--------" << endl;

    // test search
    typedef Term< Value_child, List< Value_X, NullItem > > TermChildX;       // child(X)
    typedef Term< Value_boy, List< Value_X, NullItem > > TermBoyX;           // boy(X)
    typedef Term< Value_girl, List< Value_X, NullItem > > TermGirlX;         // girl(X)
    typedef Term< Value_boy, List< Value_alex, NullItem > > TermBoyAlex;     // boy(alex)
    typedef Term< Value_girl, List< Value_alice, NullItem > > TermGirlAlice; // girl(alice)
    typedef Term< Value_child, List< Value_G, NullItem > > TermChildG;       // child(G)

    typedef Rule< TermBoyAlex, NullItem > RuleBoyAlex;                       // boy(alex) :- True
    typedef Rule< TermGirlAlice, NullItem > RuleGirlAlice;                   // girl(alice) :- True
    typedef Rule< TermChildX, List< TermBoyX, NullItem > > RuleChildBoy;     // child(X) :- boy(x)
    typedef Rule< TermChildX, List< TermGirlX, NullItem > > RuleChildGirl;   // child(X) :- girl(x)

    typedef List< RuleChildBoy, List< RuleChildGirl,
                                      List< RuleBoyAlex, List< RuleGirlAlice, NullItem > > > > AllRules;

    typedef SearchGoal< TermChildG, AllRules > Result;
    //typedef SearchGoal< TermBoyX, AllRules > Result;
    cout << Result::ret << endl;
    PrintMap< Result::ResultEnv >::Print();
}

void TestList()
{
    using namespace tcalc;
    using namespace tcalc::container::list;
    const int v = Add< Value<int, 3>, Value<int, 42> >::NextType::val;
    const int ret = Power< Div< Value<int, 42>, Value<int, 21> >::NextType, 8 >::NextType::val;

    typedef List< Value< int, 3 >, List< Value<int, 2>, List< Value<int, 1>, NullItem > > > FunnyList;
    typedef List< Value< int, 5 >, List< Value<int, 4>, List< Value<int, 6>, NullItem > > > FunnyList2;

    cout << ListLength< FunnyList >::val << endl;
    cout << Advance< FunnyList, 2 >::NextType::value_type::val << endl;

    //cout << Advance< typename PopBack< FunnyList >::NextType, 1 >::NextType::val << endl;
    //cout << Advance< typename PushBack< Value<int, 4>, FunnyList >::NextType, 2 >::NextType::val << endl;

    typedef InsertAt< FunnyList, Value<int, 5>, 2 >::NextType WithVal;
    WithVal::Print();

    typedef Merge< FunnyList, FunnyList2 >::NextType MergedList;
    MergedList::Print();

    static const int NUM_ITEMS = 7;
    typedef Generate< NUM_ITEMS >::NextType FirstList;
    typedef Generate< NUM_ITEMS, NUM_ITEMS / 2 >::NextType SecondList;

    typedef Merge< FirstList, SecondList >::NextType M;
    typedef MergeSort< M >::NextType Sorted;
    Sorted::Print();

    typedef Map< Sorted, Add, Value<int, 100> >::NextType Plus100;
    Plus100::Print();

    typedef Fold< Plus100, Add, Value<int> >::NextType Folded;
    cout << Folded::val << endl;

    typedef Filter< Sorted, Less, Value<int, 2> >::NextType Filtered;
    Filtered::Print();

    //typedef List< StringValue_1, NullItem > StringList;
    //StringList::Print();

    //typedef typename FindMinimum< FunnyList2 >::NextType E;
    //typedef Search< FunnyList2, E > Found;
    //typedef typename EraseAtIndex< FunnyList2, Found::val >::NextType L2MinusMin;

    //typedef LowerBound< FunnyList2, Value< int, 2 > > LB;
    //cout << LB::val << endl;
    //typedef typename InsertAt< FunnyList, E, LB::val >::NextType L1PlusMin;
    //L1PlusMin::Print();
    //typedef typename InsertAt< L1, E, LB::val >::NextType L1PlusMin;

    //L2MinusMin::Print();

    typedef List< Value<int, 1>, List< Value<int, 3>, NullItem > > FL;
    typedef List< Value<int, 3>, NullItem > FL2;

    typedef LowerBound< FL, Value< int, 2 > > LBx;
    cout << LBx::val << endl;

    typedef MergeSorted< FunnyList, FunnyList2 >::NextType MS;
    MS::Print();
    typedef MergeSort< MS >::NextType Sorted2;
    Sorted2::Print();

    //cout << FindMinimum< MergedList >::NextType::val << endl;

    //cout << Advance< typename First< FunnyList, 2 >::NextType, 0 >::NextType::val << endl;

    //Advance< typename EraseAtIndex< FunnyList, 1 >::NextType, 0 >::NextType::Print();

    cout << Search< FunnyList, Value< int, 1 > >::val << endl;

    ////////

    //typedef List< Value< int, 1 >, List< StringValue_1, NullItem > > ValueList;
    //ValueList::Print();
}

void TestMap()
{
    using namespace tcalc;
    using namespace tcalc::container::map;

    typedef NullItem Initial;
    typedef AddKey< Pair< tcalc::Value<int,0>, tcalc::Value<int,10> >, Initial >::NextType One;
    typedef AddKey< Pair< tcalc::Value<int,0>, tcalc::Value<int,5> >, One >::NextType ReplaceOne;
    typedef FindKey< tcalc::Value<int,0>, ReplaceOne >::NextType FoundOne;
    cout << FoundOne::Value::val << endl;
    cout << ListLength< ReplaceOne >::val << endl;

    typedef AddKey< Pair< tcalc::Value<int,1>, tcalc::Value<int,3> >, One >::NextType Two;
    cout << ListLength< Two >::val << endl;

    typedef AddKey< Pair< tcalc::Value<int,2>, tcalc::Value<int,6> >, Two >::NextType Three;
    typedef AddKey< Pair< tcalc::Value<int,2>, tcalc::Value<int,2> >, Three >::NextType ReplaceThree;
    cout << ListLength< ReplaceThree >::val << endl;

    typedef EraseKey< tcalc::Value<int,1>, Three >::NextType Erased;
    cout << ListLength< Erased >::val << endl;
}

int main()
{
    //TestList();
    //TestMap();
    TestProlog();
    return 0;
}
