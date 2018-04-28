#include <gtest/gtest.h>


#include "list.h"
typedef list<int> container;

template <typename T>
T const& as_const(T& obj)
{
    return obj;
}

template <typename C, typename T>
void mass_push_back(C& c, std::initializer_list<T> elems)
{
    for (T const& e : elems)
        c.push_back(e);
}

template <typename C, typename T>
void mass_push_front(C& c, std::initializer_list<T> elems)
{
    for (T const& e : elems)
        c.push_front(e);
}

template <typename It, typename T>
void expect_eq(It i1, It e1, std::initializer_list<T> elems)
{
    auto i2 = elems.begin(), e2 = elems.end();

    for (;;)
    {
        if (i1 == e1 || i2 == e2)
        {
            EXPECT_TRUE(i1 == e1 && i2 == e2);
            break;
        }

        EXPECT_EQ(*i2, *i1);
        ++i1;
        ++i2;
    }
}

template <typename C, typename T>
void expect_eq(C const& c, std::initializer_list<T> elems)
{
    expect_eq(c.begin(), c.end(), elems);
}

template <typename C, typename T>
void expect_reverse_eq(C const& c, std::initializer_list<T> elems)
{
    expect_eq(c.rbegin(), c.rend(), elems);
}

TEST(correctness, end_iterator)
{
    container c;
    container::iterator i = c.end();

    EXPECT_EQ(c.begin(), i);
    c.push_back(5);
    --i;
    EXPECT_EQ(5, *i);
}

TEST(correctness, back_front)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4, 5});
    EXPECT_EQ(1, c.front());
    EXPECT_EQ(1, as_const(c).front());
    EXPECT_EQ(5, c.back());
    EXPECT_EQ(5, as_const(c).back());
}

TEST(correctness, push_back)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4});
    expect_eq(c, {1, 2, 3, 4});
}

TEST(correctness, copy_ctor)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4});
    container c2 = c;
    expect_eq(c2, {1, 2, 3, 4});
}

TEST(correctness, copy_ctor_empty)
{
    container c;
    container c2 = c;
    EXPECT_TRUE(c2.empty());
}

TEST(correctness, assignment_operator)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4});
    container c2;
    mass_push_back(c2, {5, 6, 7, 8});
    c2 = c;
    expect_eq(c2, {1, 2, 3, 4});
}

TEST(correctness, self_assignment)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4});
    c = c;
    expect_eq(c, {1, 2, 3, 4});
}

TEST(correctness, pop_back)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4});
    c.pop_back();
    expect_eq(c, {1, 2, 3});
    c.pop_back();
    expect_eq(c, {1, 2});
    c.pop_back();
    expect_eq(c, {1});
    c.pop_back();
    EXPECT_TRUE(c.empty());
}

TEST(correctness, push_front)
{
    container c;
    mass_push_front(c, {1, 2, 3, 4});
    expect_eq(c, {4, 3, 2, 1});
}

TEST(correctness, empty)
{
    container c;
    EXPECT_EQ(c.begin(), c.end());
    EXPECT_TRUE(c.empty());
    c.push_back(1);
    c.begin() != c.end();
    //EXPECT_NE(c.begin(), c.end());
    EXPECT_FALSE(c.empty());
    c.pop_front();
    EXPECT_EQ(c.begin(), c.end());
    EXPECT_TRUE(c.empty());
}

TEST(correctness, reverse_iterators)
{
    container c;
    mass_push_front(c, {1, 2, 3, 4});
    expect_reverse_eq(c, {1, 2, 3, 4});

    EXPECT_EQ(1, *c.rbegin());
    EXPECT_EQ(2, *std::next(c.rbegin()));
    EXPECT_EQ(4, *std::prev(c.rend()));
}

TEST(correctness, iterator_conversions)
{
    container c;
    container::const_iterator i1 = c.begin();
    container::iterator i2 = c.end();
    EXPECT_TRUE(i1 == i1);
    EXPECT_TRUE(i1 == i2);
    EXPECT_TRUE(i2 == i1);
    EXPECT_TRUE(i2 == i2);
    EXPECT_FALSE(i1 != i1);
    EXPECT_FALSE(i1 != i2);
    EXPECT_FALSE(i2 != i1);
    EXPECT_FALSE(i2 != i2);
}

TEST(correctness, iterators_postfix)
{
    container s;
    mass_push_back(s, {1, 2, 3});
    container::iterator i = s.begin();
    EXPECT_EQ(1, *i);
    container::iterator j = i++;
    EXPECT_EQ(2, *i);
    EXPECT_EQ(1, *j);
    j = i++;
    EXPECT_EQ(3, *i);
    EXPECT_EQ(2, *j);
    j = i++;
    EXPECT_EQ(s.end(), i);
    EXPECT_EQ(3, *j);
    j = i--;
    EXPECT_EQ(3, *i);
    EXPECT_EQ(s.end(), j);
}

TEST(correctness, const_iterators_postfix)
{
    container s;
    mass_push_back(s, {1, 2, 3});
    container::const_iterator i = s.begin();
    EXPECT_EQ(1, *i);
    container::const_iterator j = i++;
    EXPECT_EQ(2, *i);
    EXPECT_EQ(1, *j);
    j = i++;
    EXPECT_EQ(3, *i);
    EXPECT_EQ(2, *j);
    j = i++;
    EXPECT_EQ(s.end(), i);
    EXPECT_EQ(3, *j);
    j = i--;
    EXPECT_EQ(3, *i);
    EXPECT_EQ(s.end(), j);
}

TEST(correctness, insert_begin)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4});
    c.insert(c.begin(), 0);
    expect_eq(c, {0, 1, 2, 3, 4});
}

TEST(correctness, insert_middle)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4});
    c.insert(std::next(c.begin(), 2), 5);
    expect_eq(c, {1, 2, 5, 3, 4});
}

TEST(correctness, insert_end)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4});
    c.insert(c.end(), 5);
    expect_eq(c, {1, 2, 3, 4, 5});
}

TEST(correctness, insert_iterators)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4});

    container::iterator i2 = c.begin();
    ++i2;
    container::iterator i3 = i2;
    ++i3;

    c.insert(i3, 5);
    --i3;
    EXPECT_EQ(5, *i3);
    ++i2;
    EXPECT_EQ(5, *i2);
    --i3;
    EXPECT_EQ(2, *i3);
    ++i2;
    EXPECT_EQ(3, *i2);
}

TEST(correctness, erase_begin)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4});
    c.erase(c.begin());
    expect_eq(c, {2, 3, 4});
}

TEST(correctness, erase_middle)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4});
    c.erase(std::next(c.begin(), 2));
    //expect_eq(c, {1, 2, 4});
}

TEST(correctness, erase_end)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4});
    c.erase(std::prev(c.end()));
    expect_eq(c, {1, 2, 3});
}

TEST(correctness, erase_iterators)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4});

    container::iterator i2 = c.begin();
    ++i2;
    container::iterator i3 = i2;
    ++i3;
    container::iterator i4 = i3;
    ++i4;

    c.erase(i3);
    --i4;
    ++i2;
    EXPECT_EQ(2, *i4);
    EXPECT_EQ(4, *i2);
}

TEST(correctness, erase_end_whole)
{
    container c;
    mass_push_back(c, {1, 2, 3, 4});
    c.erase(c.begin(), c.end());
    EXPECT_TRUE(c.empty());
    EXPECT_EQ(c.begin(), c.end());
}

TEST(correctness, splice_begin_begin)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(c1.begin(), c2, c2.begin(), std::next(c2.begin(), 2));
    expect_eq(c1, {5, 6, 1, 2, 3, 4});
    expect_eq(c2, {7, 8});
}

TEST(correctness, splice_begin_middle)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(c1.begin(), c2, std::next(c2.begin()), std::next(c2.begin(), 2));
    expect_eq(c1, {6, 1, 2, 3, 4});
    expect_eq(c2, {5, 7, 8});
}

TEST(correctness, splice_begin_end)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(c1.begin(), c2, std::next(c2.begin(), 2), c2.end());
    expect_eq(c1, {7, 8, 1, 2, 3, 4});
    expect_eq(c2, {5, 6});
}

TEST(correctness, splice_begin_whole)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(c1.begin(), c2, c2.begin(), c2.end());
    expect_eq(c1, {5, 6, 7, 8, 1, 2, 3, 4});
    EXPECT_TRUE(c2.empty());
}

TEST(correctness, splice_middle_begin)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(std::next(c1.begin(), 2), c2, c2.begin(), std::next(c2.begin(), 2));
    expect_eq(c1, {1, 2, 5, 6, 3, 4});
    expect_eq(c2, {7, 8});
}

TEST(correctness, splice_middle_middle)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(std::next(c1.begin(), 2), c2, std::next(c2.begin()), std::next(c2.begin(), 3));
    expect_eq(c1, {1, 2, 6, 7, 3, 4});
    expect_eq(c2, {5, 8});
}

TEST(correctness, splice_middle_end)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(std::next(c1.begin(), 2), c2, std::next(c2.begin(), 2), c2.end());
    expect_eq(c1, {1, 2, 7, 8, 3, 4});
    expect_eq(c2, {5, 6});
}

TEST(correctness, splice_middle_whole)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(std::next(c1.begin(), 2), c2, c2.begin(), c2.end());
    expect_eq(c1, {1, 2, 5, 6, 7, 8, 3, 4});
    EXPECT_TRUE(c2.empty());
}

TEST(correctness, splice_middle_empty)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(std::next(c1.begin(), 2), c2, std::next(c2.begin(), 2), std::next(c2.begin(), 2));
    expect_eq(c1, {1, 2, 3, 4});
    expect_eq(c2, {5, 6, 7, 8});
}

TEST(correctness, splice_end_begin)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(c1.end(), c2, c2.begin(), std::next(c2.begin(), 2));
    expect_eq(c1, {1, 2, 3, 4, 5, 6});
    expect_eq(c2, {7, 8});
}

TEST(correctness, splice_end_middle)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(c1.end(), c2, std::next(c2.begin()), std::next(c2.begin(), 3));
    expect_eq(c1, {1, 2, 3, 4, 6, 7});
    expect_eq(c2, {5, 8});
}

TEST(correctness, splice_end_end)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(c1.end(), c2, std::next(c2.begin(), 2), c2.end());
    expect_eq(c1, {1, 2, 3, 4, 7, 8});
    expect_eq(c2, {5, 6});
}

TEST(correctness, splice_end_whole)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(c1.end(), c2, c2.begin(), c2.end());
    expect_eq(c1, {1, 2, 3, 4, 5, 6, 7, 8});
    EXPECT_TRUE(c2.empty());
}

TEST(correctness, splice_empty_begin)
{
    container c1, c2;
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(c1.end(), c2, c2.begin(), std::next(c2.begin(), 2));
    expect_eq(c1, {5, 6});
    expect_eq(c2, {7, 8});
}

TEST(correctness, splice_empty_middle)
{
    container c1, c2;
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(c1.end(), c2, std::next(c2.begin(), 1), std::next(c2.begin(), 3));
    expect_eq(c1, {6, 7});
    expect_eq(c2, {5, 8});
}

TEST(correctness, splice_empty_end)
{
    container c1, c2;
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(c1.end(), c2, std::next(c2.begin(), 2), c2.end());
    expect_eq(c1, {7, 8});
    expect_eq(c2, {5, 6});
}

TEST(correctness, splice_empty_whole)
{
    container c1, c2;
    mass_push_back(c2, {5, 6, 7, 8});
    c1.splice(c1.end(), c2, c2.begin(), c2.end());
    expect_eq(c1, {5, 6, 7, 8});
    EXPECT_TRUE(c2.empty());
}

TEST(correctness, splice_self)
{
    container c1;
    mass_push_back(c1, {1, 2, 3, 4, 5});
    c1.splice(std::next(c1.begin()), c1, std::next(c1.begin(), 2), std::prev(c1.end()));
    expect_eq(c1, {1, 3, 4, 2, 5});
}

TEST(correctness, splice_iterators)
{
    container c1;
    container c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    container::const_iterator i = std::next(c1.begin(), 2);
    container::const_iterator j = std::next(c2.begin());
    container::const_iterator k = std::prev(c2.end());
    c1.splice(i, c2, j, k);
    expect_eq(c1, {1, 2, 6, 7, 3, 4});
    expect_eq(c2, {5, 8});

    EXPECT_EQ(3, *i);
    EXPECT_EQ(6, *j);
    EXPECT_EQ(8, *k);

    EXPECT_EQ(7, *std::prev(i));
    EXPECT_EQ(2, *std::prev(j));
    EXPECT_EQ(5, *std::prev(k));
}

TEST(correctness, swap)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    mass_push_back(c2, {5, 6, 7, 8});
    swap(c1, c2);
    expect_eq(c1, {5, 6, 7, 8});
    expect_eq(c2, {1, 2, 3, 4});
}

TEST(correctness, swap_self)
{
    container c1;
    mass_push_back(c1, {1, 2, 3, 4});
    swap(c1, c1);
}

TEST(correctness, swap_empty)
{
    container c1, c2;
    mass_push_back(c1, {1, 2, 3, 4});
    swap(c1, c2);
    EXPECT_TRUE(c1.empty());
    expect_eq(c2, {1, 2, 3, 4});
    swap(c1, c2);
    expect_eq(c1, {1, 2, 3, 4});
    EXPECT_TRUE(c2.empty());
}

TEST(correctness, swap_empty_empty)
{
    container c1, c2;
    swap(c1, c2);
}

TEST(correctness, swap_empty_self)
{
    container c1;
    swap(c1, c1);
}

TEST(invalid, pop_front_empty) {
    EXPECT_EXIT(
    {
        container c;
        c.pop_front();
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, pop_back_empty) {
    EXPECT_EXIT(
    {
        container c;
        c.pop_back();
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, front_empty) {
    EXPECT_EXIT(
    {
        container c;
        c.front();
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, back_empty) {
    EXPECT_EXIT(
    {
        container c;
        c.back();
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, const_front_empty) {
    EXPECT_EXIT(
    {
        container c;
        as_const(c).front();
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, const_back_empty) {
    EXPECT_EXIT(
    {
        container c;
        as_const(c).back();
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, insert_other_list) {
    EXPECT_EXIT(
    {
        container c1;
        container c2;
        mass_push_back(c1, {1, 2, 3, 4});
        mass_push_back(c2, {5, 6, 7, 8});
        c1.insert(c2.end(), 9);
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, insert_other_empty_list) {
    EXPECT_EXIT(
    {
        container c1;
        container c2;
        mass_push_back(c1, {1, 2, 3, 4});
        c1.insert(c2.end(), 9);
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, erase_other_list) {
    EXPECT_EXIT(
    {
        container c1;
        container c2;
        mass_push_back(c1, {1, 2, 3, 4});
        mass_push_back(c2, {5, 6, 7, 8});
        c1.erase(std::next(c2.begin(), 2));
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, erase_range_other_list_1) {
    EXPECT_EXIT(
    {
        container c1;
        container c2;
        mass_push_back(c1, {1, 2, 3, 4});
        mass_push_back(c2, {5, 6, 7, 8});
        c1.erase(std::next(c2.begin(), 2), c2.end());
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, erase_range_other_list_2) {
    EXPECT_EXIT(
    {
        container c1;
        container c2;
        mass_push_back(c1, {1, 2, 3, 4});
        mass_push_back(c2, {5, 6, 7, 8});
        c1.erase(c1.begin(), std::next(c2.begin(), 2));
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, splice_wrong_pos) {
    EXPECT_EXIT(
    {
        container c1;
        container c2;
        mass_push_back(c1, {1, 2, 3, 4});
        mass_push_back(c2, {5, 6, 7, 8});
        c1.splice(std::next(c2.begin(), 2), c2, c2.begin(), c2.end());
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, splice_wrong_first) {
    EXPECT_EXIT(
    {
        container c1;
        container c2;
        mass_push_back(c1, {1, 2, 3, 4});
        mass_push_back(c2, {5, 6, 7, 8});
        c1.splice(std::next(c1.begin(), 2), c2, std::next(c1.begin(), 3), c2.end());
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, splice_wrong_last) {
    EXPECT_EXIT(
    {
        container c1;
        container c2;
        mass_push_back(c1, {1, 2, 3, 4});
        mass_push_back(c2, {5, 6, 7, 8});
        c1.splice(std::next(c1.begin(), 2), c2, c2.begin(), std::next(c1.begin(), 3));
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, deref_default_iterator) {
    EXPECT_EXIT(
    {
        container::iterator i;
        *i;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, deref_default_const_iterator) {
    EXPECT_EXIT(
    {
        container::const_iterator i;
        *i;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, preinc_default_iterator) {
    EXPECT_EXIT(
    {
        container::iterator i;
        ++i;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, preinc_default_const_iterator) {
    EXPECT_EXIT(
    {
        container::const_iterator i;
        ++i;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, postinc_default_iterator) {
    EXPECT_EXIT(
    {
        container::iterator i;
        i++;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, postinc_default_const_iterator) {
    EXPECT_EXIT(
    {
        container::const_iterator i;
        i++;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, predec_default_iterator) {
    EXPECT_EXIT(
    {
        container::iterator i;
        --i;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, predec_default_const_iterator) {
    EXPECT_EXIT(
    {
        container::const_iterator i;
        --i;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, postdec_default_iterator) {
    EXPECT_EXIT(
    {
        container::iterator i;
        i--;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, postdec_default_const_iterator) {
    EXPECT_EXIT(
    {
        container::const_iterator i;
        i--;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, deref_end) {
    EXPECT_EXIT(
    {
        container c;
        mass_push_back(c, {1, 2, 3, 4});
        *c.end();
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, deref_const_end) {
    EXPECT_EXIT(
    {
        container c;
        mass_push_back(c, {1, 2, 3, 4});
        *as_const(c).end();
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, inc_end) {
    EXPECT_EXIT(
    {
        container c;
        mass_push_back(c, {1, 2, 3, 4});
        container::iterator i = c.end();
        ++i;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, inc_const_end) {
    EXPECT_EXIT(
    {
        container c;
        mass_push_back(c, {1, 2, 3, 4});
        container::const_iterator i = c.end();
        ++i;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, dec_begin) {
    EXPECT_EXIT(
    {
        container c;
        mass_push_back(c, {1, 2, 3, 4});
        container::iterator i = c.begin();
        --i;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, dec_const_begin) {
    EXPECT_EXIT(
    {
        container c;
        mass_push_back(c, {1, 2, 3, 4});
        container::const_iterator i = c.begin();
        --i;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, deref_erased) {
    EXPECT_EXIT(
    {
        container c;
        mass_push_back(c, {1, 2, 3, 4});
        container::iterator i = c.begin();
        ++i;
        ++i;
        container::iterator j = c.begin();
        ++j;
        ++j;
        c.erase(i);
        *j;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, inc_erased) {
    EXPECT_EXIT(
    {
        container c;
        mass_push_back(c, {1, 2, 3, 4});
        container::iterator i = c.begin();
        ++i;
        ++i;
        container::iterator j = c.begin();
        ++j;
        ++j;
        c.erase(i);
        ++j;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, dec_erased) {
    EXPECT_EXIT(
    {
        container c;
        mass_push_back(c, {1, 2, 3, 4});
        container::iterator i = c.begin();
        ++i;
        ++i;
        container::iterator j = c.begin();
        ++j;
        ++j;
        c.erase(i);
        --j;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, deref_destroyed)
{
    EXPECT_EXIT(
    {
        container::iterator i;
        {
            container c;
            mass_push_back(c, {1, 2, 3, 4});
            i = c.begin();
        }
        *i;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, deref_destroyed_empty)
{
    EXPECT_EXIT(
    {
        container::iterator i;
        {
            container c;
            i = c.begin();
        }
        *i;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, eq_default)
{
    EXPECT_EXIT(
    {
        container::iterator i;
        container c;
        i == c.begin();
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, neq_default)
{
    EXPECT_EXIT(
    {
        container::iterator i;
        container c;
        i == c.begin();
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, eq_destroyed)
{
    EXPECT_EXIT(
    {
        container::iterator i;
        {
            container c;
            i = c.end();
        }
        container d;
        i == d.begin();
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, deref_after_assignment)
{
    EXPECT_EXIT(
    {
        container c;
        mass_push_back(c, {1, 2, 3, 4});

        container c2;
        mass_push_back(c, {5, 6, 7, 8});

        container::const_iterator i = std::next(c.begin(), 2);
        c = c2;
        *i;
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, deref_after_self_assignment)
{
    EXPECT_EXIT(
    {
        container c;
        mass_push_back(c, {1, 2, 3, 4});

        container::const_iterator i = std::next(c.begin(), 2);
        c = c;
        *i;
    }, ::testing::KilledBySignal(SIGABRT), "");
}


TEST(invalid, splice_inside_range)
{
    EXPECT_EXIT(
    {
        container c1;
        container c2;
        mass_push_back(c1, {1, 2, 3, 4});
        c1.splice(std::next(c1.begin(), 2), c1, std::next(c1.begin()), std::prev(c1.end()));
    }, ::testing::KilledBySignal(SIGABRT), "");
}

TEST(invalid, splice_reversed_range)
{
    EXPECT_EXIT(
    {
        container c1;
        container c2;
        mass_push_back(c1, {1, 2, 3, 4});
        mass_push_back(c2, {5, 6, 7, 8});
        c1.splice(std::next(c1.begin(), 2), c2, std::prev(c2.end()), std::next(c2.begin()));
    }, ::testing::KilledBySignal(SIGABRT), "");
}

