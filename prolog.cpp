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
    DECLARE_VAR( Y );
    DECLARE_VAR( Z );
    DECLARE_VAR( G );

    DECLARE_SYM( trude );
    DECLARE_SYM( sally );
    DECLARE_SYM( erica );
    DECLARE_SYM( tom );
    DECLARE_SYM( mike );
    DECLARE_SYM( father_child );
    DECLARE_SYM( mother_child );
    DECLARE_SYM( parent_child );
    DECLARE_SYM( sibling );
}

void TestProlog()
{
    using namespace tcalc;
    using namespace tcalc::container::list;
    using namespace tcalc::prolog;

    typedef Term< Value_boy, List< Value_bill, NullItem > > TermA;
    typedef Term< Value_boy, List< Value_frank, NullItem > > TermB;
    typedef Term< Value_boy, List< Value_X, NullItem > > TermX;
    typedef Term< Value_boy, List< Value_G, NullItem > > TermG;

    typedef Term< Value_child, List< Value_X, NullItem > > TermChildX;       // child(X).
    typedef Term< Value_boy, List< Value_X, NullItem > > TermBoyX;           // boy(X).
    typedef Term< Value_girl, List< Value_X, NullItem > > TermGirlX;         // girl(X).
    typedef Term< Value_boy, List< Value_alex, NullItem > > TermBoyAlex;     // boy(alex).
    typedef Term< Value_girl, List< Value_alice, NullItem > > TermGirlAlice; // girl(alice).
    typedef Term< Value_child, List< Value_G, NullItem > > TermChildG;       // child(G).

    typedef Rule< TermBoyAlex, NullItem > RuleBoyAlex;                       // boy(alex) :- True.
    typedef Rule< TermGirlAlice, NullItem > RuleGirlAlice;                   // girl(alice) :- True.
    typedef Rule< TermChildX, List< TermBoyX, NullItem > > RuleChildBoy;     // child(X) :- boy(x).
    typedef Rule< TermChildX, List< TermGirlX, NullItem > > RuleChildGirl;   // child(X) :- girl(x).

    typedef List< RuleChildBoy, List< RuleChildGirl,
                                      List< RuleBoyAlex, List< RuleGirlAlice, NullItem > > > > AllRules;

    typedef SearchGoal< TermChildG, AllRules > Result;
    //typedef SearchGoal< TermBoyX, AllRules > Result;
    cout << Result::ret << endl;
    PrintMap< Result::ResultEnv >::Print();

    ////////

    // mother_child(trude, sally)
    typedef Term< Value_mother_child, List< Value_trude, List< Value_sally, NullItem > > > TermMotherChildTS;
    // father_child(tom, sally)
    typedef Term< Value_father_child, List< Value_tom, List< Value_sally, NullItem > > > TermFatherChildTS;
    // father_child(tom, erica)
    typedef Term< Value_father_child, List< Value_tom, List< Value_erica, NullItem > > > TermFatherChildTE;
    // father_child(mike, tom)
    typedef Term< Value_father_child, List< Value_mike, List< Value_tom, NullItem > > > TermFatherChildMT;

    // parent_child(Z, X)
    typedef Term< Value_parent_child, List< Value_Z, List< Value_X, NullItem > > > TermParentChildZX;
    // parent_child(Z, Y)
    typedef Term< Value_parent_child, List< Value_Z, List< Value_Y, NullItem > > > TermParentChildZY;
    // parent_child(X, Y)
    typedef Term< Value_parent_child, List< Value_X, List< Value_Y, NullItem > > > TermParentChildXY;
    // father_child(X, Y)
    typedef Term< Value_father_child, List< Value_X, List< Value_Y, NullItem > > > TermFatherChildXY;
    // mother_child(X, Y)
    typedef Term< Value_mother_child, List< Value_X, List< Value_Y, NullItem > > > TermMotherChildXY;
    // sibling(X, Y)
    typedef Term< Value_sibling, List< Value_X, List< Value_Y, NullItem > > > TermSiblingXY;

    // mother_child(trude, sally).
    typedef Rule< TermMotherChildTS, NullItem > RuleMotherChildTS;
    // father_child(tom, sally).
    typedef Rule< TermFatherChildTS, NullItem > RuleFatherChildTS;
    // father_child(tom, erica).
    typedef Rule< TermFatherChildTE, NullItem > RuleFatherChildTE;
    // father_child(mike, tom).
    typedef Rule< TermFatherChildMT, NullItem > RuleFatherChildMT;

    // sibling(X, Y) :- parent_child(Z, X), parent_child(Z, Y).
    typedef Rule< TermSiblingXY, List< TermParentChildZX, List< TermParentChildZY, NullItem > > > RuleSibling;

    // parent_child(X, Y) :- father_child(X, Y).
    typedef Rule< TermParentChildXY, List< TermFatherChildXY, NullItem > > RuleParentChild1;

    // parent_child(X, Y) :- mother_child(X, Y).
    typedef Rule< TermParentChildXY, List< TermMotherChildXY, NullItem > > RuleParentChild2;

    // sibling(sally, erica).
    typedef Term< Value_sibling, List< Value_sally, List< Value_erica, NullItem > > > TermQuestion;

    typedef List< RuleMotherChildTS,
                  List< RuleFatherChildTS,
                        List< RuleFatherChildTE,
                              List< RuleFatherChildMT,
                                    List< RuleSibling,
                                          List< RuleParentChild1,
                                                List< RuleParentChild2, NullItem > > > > > > > AllRules2;

    typedef SearchGoal< TermQuestion, AllRules2 > Result2;
    cout << Result2::ret << endl;
    PrintMap< Result2::ResultEnv >::Print();
}

int main()
{
    TestProlog();
    return 0;
}
