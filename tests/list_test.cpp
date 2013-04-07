/* list_seq_test.cpp                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 07 Apr 2013
   FreeBSD-style copyright and disclaimer apply

   Tests for the List linked list.
*/

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include "list.h"
#include "test_utils.h"

#include <boost/test/unit_test.hpp>

using namespace std;
using namespace lockless;

BOOST_AUTO_TEST_CASE(test_node)
{
    cerr << fmtTitle("node", '=') << endl;

    typedef ListNode<size_t> Node;

    Node(); // default constructor.

    Node a(size_t(10));
    locklessCheckEq(a, size_t(10), NullLog);
    locklessCheckEq(a.get(), size_t(10), NullLog);
    locklessCheckEq(a.next(), nullptr, NullLog);

    Node b(20);
    locklessCheckEq(b, size_t(20), NullLog);
    locklessCheckEq(b.get(), size_t(20), NullLog);
    locklessCheckEq(b.next(), nullptr, NullLog);

    Node* exp = nullptr;
    locklessCheck(a.compare_exchange_next(exp, &b), NullLog);
    locklessCheckEq(exp, nullptr, NullLog);

    locklessCheck(!a.compare_exchange_next(exp, &b), NullLog);
    locklessCheckEq(exp, &b, NullLog);

    Node c{Node(a)}; // copy & move constructor.
    locklessCheckEq(c, size_t(10), NullLog);
    locklessCheckEq(c.get(), size_t(10), NullLog);
    locklessCheckEq(c.next(), nullptr, NullLog);

    Node* pNil = b.mark();
    locklessCheck(b.isMarked(), NullLog);
    locklessCheckEq(pNil, nullptr, NullLog);
    locklessCheckEq(b.next(), pNil, NullLog);

    b.reset();
    locklessCheck(!b.isMarked(), NullLog);
    locklessCheckEq(b.next(), nullptr, NullLog);

    Node* pB = a.mark();
    locklessCheck(a.isMarked(), NullLog);
    locklessCheckEq(pB, &b, NullLog);
    locklessCheckEq(a.next(), pB, NullLog);

    a.reset();
    locklessCheck(!a.isMarked(), NullLog);
    locklessCheckEq(a.next(), nullptr, NullLog);
}

BOOST_AUTO_TEST_CASE(test_push_pop)
{
    cerr << fmtTitle("push pop", '=') << endl;

    List<size_t> list;
    auto& log = list.log;

    for (size_t i = 0; i < 10; ++i) {
        auto* node = new ListNode<size_t>(i);
        list.push(node);
        locklessCheckEq(list.head.load(), node, log);
    }

    for (size_t i = 0; i < 10; ++i) {
        ListNode<size_t>* node = list.pop();
        locklessCheckEq(*node, 9-i, log);
        delete node;
    }
}

BOOST_AUTO_TEST_CASE(test_pop_marked)
{
    cerr << fmtTitle("pop marked", '=') << endl;

    List<size_t> list;
    auto& log = list.log;

    cerr << fmtTitle("push") << endl;

    for (size_t i = 0; i < 10; ++i)
        list.push(new ListNode<size_t>(i));

    cerr << fmtTitle("mark") << endl;

    auto* node = list.head.load();
    while (node && node->next()) {
        node->mark();
        locklessCheck(node->isMarked(), list.log);
        node = node->next()->next();
    }

    cerr << fmtTitle("pop") << endl;

    for (size_t i = 0; i < 10; ++i) {
        node = list.popMarked();

        if (node == nullptr) {
            locklessCheckEq(node, nullptr, log);
            list.head.load()->mark();
            i--;
            continue;
        }

        locklessCheck(node, log);
        locklessCheckEq(*node, 9-i, log);
        delete node;
    }

    locklessCheckEq(list.head.load(), nullptr, log);
}