#include <gtest/gtest.h>

#include <rb_tree.hpp>

#include <iostream>

using namespace cust;

// Auxiliary
using tree = RBTree<double>;
using node = RBTree<double>::Node;
using color = RBTree<double>::Color;
using childSide = RBTree<double>::ChildSide;

void set_link(tree::node_ptr new_parent, tree::node_ptr new_child, childSide s) {
	if (s == childSide::LEFT) {
		new_parent->left = new_child;
	} else {
		new_parent->right = new_child;
	}
	new_child->parent = new_parent;
}

tree::node_ptr make_node(color c, double val) {
	auto val_ptr = std::make_shared<double>(val);
	return std::make_shared<node>(c, val_ptr, cust::equal_to<double>, cust::less<double>);
}

tree::node_ptr add_child_to_node(tree::node_ptr n, color c, double val, childSide s) {
	auto _n = make_node(c, val);
	set_link(n, _n, s);
	return _n;
}

// Tree examples
tree::node_ptr rb_tree_example_1() {
	auto n4 = make_node(color::BLACK, 4);
	auto n2 = add_child_to_node(n4, color::RED, 2, childSide::LEFT);
	auto n6 = add_child_to_node(n4, color::BLACK, 6, childSide::RIGHT);
	auto n0 = add_child_to_node(n2, color::BLACK, 0, childSide::LEFT);
	auto n3 = add_child_to_node(n2, color::BLACK, 3, childSide::RIGHT);
	auto n5 = add_child_to_node(n6, color::BLACK, 5, childSide::LEFT);
	auto n8 = add_child_to_node(n6, color::BLACK, 8, childSide::RIGHT);
	auto n1 = add_child_to_node(n0, color::RED, 1, childSide::RIGHT);
	auto n7 = add_child_to_node(n8, color::RED, 7, childSide::LEFT);
	auto n9 = add_child_to_node(n8, color::RED, 9, childSide::RIGHT);

	return n4;
}

// AddMethod tests
#ifndef AddMethodTests
#define AddMethodTests

void assert_good_addition(tree& t, tree const& _t, double val) {
	try {
		auto last_size = t._size;
		t.add(val);
		ASSERT_EQ(t, _t);
		ASSERT_EQ(t._size, last_size + 1);
	} catch (...) {
		FAIL();
	}
}

tree preTreeInTest__AddNode8ToEmptyTree__() {
	return tree();
}

tree wantedTreeInTest__AddNode8ToEmptyTree__() {
	tree t = preTreeInTest__AddNode8ToEmptyTree__();
	tree::node_ptr root = make_node(tree::Color::BLACK, 8);
	t.root = root;
	return t;
}

TEST(AddMethod, AddNode8ToEmptyTree) {
	tree t = preTreeInTest__AddNode8ToEmptyTree__();
	tree _t = wantedTreeInTest__AddNode8ToEmptyTree__();
	assert_good_addition(t, _t, 8);
}

tree preTreeInTest__Addition15CauseRecolorWithRoot__() {
	tree t = wantedTreeInTest__AddNode8ToEmptyTree__();
	auto n8 = t.root;
	auto n5 = add_child_to_node(n8, color::RED, 5, childSide::LEFT);
	auto n18 = add_child_to_node(n8, color::RED, 18, childSide::RIGHT);
	return t;
}

tree wantedTreeInTest__Addition15CauseRecolorWithRoot__() {
	tree t = preTreeInTest__Addition15CauseRecolorWithRoot__();
	auto n8  = t.root;    n8->color  = color::BLACK;
	auto n5  = n8->left;  n5->color  = color::BLACK;
	auto n18 = n8->right; n18->color = color::BLACK;
	auto n15 = add_child_to_node(n18, color::RED, 15, childSide::LEFT);
	return t;
}

TEST(AddMethod, Addition15CauseRecolorWithRoot) {
	tree t = preTreeInTest__Addition15CauseRecolorWithRoot__();
	tree _t = wantedTreeInTest__Addition15CauseRecolorWithRoot__();
	assert_good_addition(t, _t, 15);
}

tree preTreeInTest__Addition17CauseRLRotationAndRecolor__() {
	return wantedTreeInTest__Addition15CauseRecolorWithRoot__();
}

tree wantedTreeInTest__Addition17CauseRLRotationAndRecolor__() {
	tree t = preTreeInTest__Addition17CauseRLRotationAndRecolor__();
	auto n8 = t.root;
	auto n5 = n8->left;
	auto n18 = n8->right;
	auto n15 = n18->left;

	auto n17 = add_child_to_node(n8, color::BLACK, 17, childSide::RIGHT);

	set_link(n17, n15, childSide::LEFT);
	n15->left = n15->right = nullptr;
	n15->color = color::RED;

	set_link(n17, n18, childSide::RIGHT);
	n18->left = n18->right = nullptr;
	n18->color = color::RED;

	return t;
}

TEST(AddMethod, Addition17CauseRLRotationAndRecolor) {
	tree t = preTreeInTest__Addition17CauseRLRotationAndRecolor__();
	tree _t = wantedTreeInTest__Addition17CauseRLRotationAndRecolor__();
	assert_good_addition(t, _t, 17);
}

tree preTreeInTest__Addition40CauseLLRotationAndRecolor__() {
	tree t = wantedTreeInTest__Addition17CauseRLRotationAndRecolor__();
	auto n8 = t.root;
	auto n17 = n8->right;
	auto n15 = n17->left;
	auto n18 = n17->right;

	n17->color = color::RED;
	n15->color = n18->color = color::BLACK;
	auto n25 = add_child_to_node(n18, color::RED, 25, childSide::RIGHT);
	return t;
}

tree wantedTreeInTest__Addition40CauseLLRotationAndRecolor__() {
	tree t = preTreeInTest__Addition40CauseLLRotationAndRecolor__();
	auto n8 = t.root;
	auto n17 = n8->right;
	auto n18 = n17->right;
	auto n25 = n18->right;
	set_link(n17, n25, childSide::RIGHT);
	set_link(n25, n18, childSide::LEFT);
	n18->left = n18->right = nullptr;
	n18->color = color::RED;
	n25->color = color::BLACK;
	auto n40 = add_child_to_node(n25, color::RED, 40, childSide::RIGHT);
	return t;
}

TEST(AddMethod, Addition40CauseLLRotationAndRecolor) {
	tree t = preTreeInTest__Addition40CauseLLRotationAndRecolor__();
	tree _t = wantedTreeInTest__Addition40CauseLLRotationAndRecolor__();
	assert_good_addition(t, _t, 40);
}

tree preTreeInTest__Addition80CauseRecolorAndLRotationAndRecolor__() {
	return wantedTreeInTest__Addition40CauseLLRotationAndRecolor__();
}

tree wantedTreeInTest__Addition80CauseRecolorAndLRotationAndRecolor__() {
	tree t;
	auto n17 = make_node(color::BLACK, 17);
	auto n8  = add_child_to_node(n17, color::RED,    8, childSide::LEFT);
	auto n25 = add_child_to_node(n17, color::RED,   25, childSide::RIGHT);
	auto n5  = add_child_to_node(n8,  color::BLACK,  5, childSide::LEFT);
	auto n15 = add_child_to_node(n8,  color::BLACK, 15, childSide::RIGHT);
	auto n18 = add_child_to_node(n25, color::BLACK, 18, childSide::LEFT);
	auto n40 = add_child_to_node(n25, color::BLACK, 40, childSide::RIGHT);
	auto n80 = add_child_to_node(n40, color::RED,   80, childSide::RIGHT);
	t.root = n17;
	return t;
}

TEST(AddMethod, Addition80CauseRecolorAndLRotationAndRecolor) {
	tree t = preTreeInTest__Addition80CauseRecolorAndLRotationAndRecolor__();
	tree _t = wantedTreeInTest__Addition80CauseRecolorAndLRotationAndRecolor__();
	assert_good_addition(t, _t, 80);
}

TEST(FindInSubtreeMethod, EmptyRBTreeBad) {
	std::shared_ptr<node> root;
	ASSERT_THROW(
		{tree::findInSubtree(root, 0);},
		NoSuchElementInSubtree
	);
}

void rb_tree_example_1_find_parent_element_good(double val, double prev) {
	tree t; t.root = rb_tree_example_1();
	tree::AdditionMethodImplementation impl(&t);

	std::shared_ptr<node> n;
	try {
		n = impl.findLeafParentInSubtree(t.root, val);
	} catch(...) {
		FAIL();
	}
	ASSERT_TRUE(*n->value == prev);
}

void rb_tree_example_1_find_parent_element_bad(double val) {
	tree t; t.root = rb_tree_example_1();
	tree::AdditionMethodImplementation impl(&t);
	ASSERT_THROW(
		{impl.findLeafParentInSubtree(t.root, val);},
		NoLeafParentElementInTree
	);
}

TEST(FindParentInSubtreeMethod, RBTreeExample1FindPrevOf8Is6) {
	rb_tree_example_1_find_parent_element_good(10, 9);
}

TEST(FindParentInSubtreeMethod, RBTreeExample1FindPrevOf1Is0) {
	rb_tree_example_1_find_parent_element_good(-1, 0);
}

TEST(FindParentInSubtreeMethod, RBTreeExample1FindPrevOf7_5Is7) {
	rb_tree_example_1_find_parent_element_good(7.5, 7);
}

TEST(FindParentInSubtreeMethod, RBTreeExample1FindPrevOf4Bad) {
	rb_tree_example_1_find_parent_element_bad(4);
}

TEST(AddToLeafOfSubtreeMethod, RBTreeExample1Add7_5ToLeaf) {
	tree t; t.root = rb_tree_example_1();
	tree::AdditionMethodImplementation impl(&t);
	auto node = impl.addToLeafOfSubtree(t.root, 7.5);
	auto parent = node->parent.lock();
	ASSERT_TRUE(parent);
	ASSERT_EQ(*(parent->value), 7.0);
	ASSERT_EQ(parent->right, node);
}

TEST(AddToLeafOfSubtreeMethod, RBTreeExample1Addm1ToLeaf) {
	tree t; t.root = rb_tree_example_1();
	tree::AdditionMethodImplementation impl(&t);
	auto node = impl.addToLeafOfSubtree(t.root, -1);
	auto parent = node->parent.lock();
	ASSERT_TRUE(parent);
	ASSERT_EQ(*(parent->value), 0);
	ASSERT_EQ(parent->left, node);
}

TEST(AddToLeafOfSubtreeMethod, RBTreeEmptyAdd0ToLeaf) {
	tree t; t.root = nullptr;
	tree::AdditionMethodImplementation impl(&t);
	ASSERT_THROW(
		{ auto node = impl.addToLeafOfSubtree(t.root, 0); },
		TreeEmpty
	);
}
#endif

// FindMethod tests
#ifndef FindMethodTests
#define FindMethodTests

void rb_tree_example_1_find_element_good(double val) {
	auto root = rb_tree_example_1();

	std::shared_ptr<node> n;
	try {
		n = tree::findInSubtree(root, val);
	} catch(...) {
		FAIL();
	}
	ASSERT_TRUE(*n->value == val);
}

void rb_tree_example_1_find_element_bad(double val) {
	auto root = rb_tree_example_1();
	ASSERT_THROW(
		{tree::findInSubtree(root, val);},
		NoSuchElementInSubtree
	);
}

TEST(FindInSubtreeMethod, RBTreeExample1FindAllNodes) {
	auto root = rb_tree_example_1();
	for (double i = 0; i < 10; ++i) {
		rb_tree_example_1_find_element_good(i);
	}
}

TEST(FindInSubtreeMethod, RBTreeExample1Find10Bad) {
	rb_tree_example_1_find_element_bad(10);
}
#endif

// EmptyMethod tests
#ifndef EmptyMethodTests
#define EmptyMethodTests
TEST(EmptyMethod, TreeEmptyAfterConstruction) {
	RBTree<double> tree;
	ASSERT_TRUE(tree.empty());
}
#endif

// RemoveMethod tests
// #define RemoveMethodTests

#ifndef RemoveMethodTests
#define RemoveMethodTests

TEST(RemoveMethod, RemoveFromEmptyTreeBad) {
	tree t;
	ASSERT_THROW(
		{ t.remove(0); },
		TreeEmpty
	);
}

void assert_good_removal(tree& t, tree const& _t, double val) {
	tree::value_ptr val_ptr;
	try {
		val_ptr = t.remove(val);
	} catch (...) {
		FAIL();
	}
	ASSERT_EQ(t, _t);
	ASSERT_EQ(*val_ptr, val);
}

tree preTreeInTest__RemoveSoloRootGood__() {
	tree t;
	auto n8 = make_node(color::BLACK, 8);
	t.root = n8;
	return t;
}

tree wantedTreeInTest__RemoveSoloRootGood__() {
	tree _t;
	return _t;
}

TEST(RemoveMethod, RemoveSoloRoot8Good) {
	tree t = preTreeInTest__RemoveSoloRootGood__();
	tree _t = wantedTreeInTest__RemoveSoloRootGood__();
	assert_good_removal(t, _t, 8);
}

tree preTreeInTest__RedNode18WithoutKids__() {
	tree t;
	auto n8 = make_node(color::BLACK, 8);
	auto n18 = add_child_to_node(n8, color::RED, 18,  childSide::RIGHT);
	t.root = n8;
	return t;
} 

tree wantedTreeInTest__RedNode18WithoutKids__() {
	tree _t;
	auto n8 = make_node(color::BLACK, 8);
	_t.root = n8;
	return _t;
} 

TEST(RemoveMethod, RedNode18WithoutKids) {
	tree t = preTreeInTest__RedNode18WithoutKids__();
	tree _t = wantedTreeInTest__RedNode18WithoutKids__();
	assert_good_removal(t, _t, 18);
}

tree preTreeInTest__BlackNode18WithOneRedChild__() {
	tree t;
	auto n8 = make_node(color::BLACK, 8);
	auto n5 = add_child_to_node(n8, color::BLACK, 5, childSide::LEFT);
	auto n18 = add_child_to_node(n8, color::BLACK, 18,  childSide::RIGHT);
	auto n17 = add_child_to_node(n18, color::RED, 17, childSide::LEFT);
	t.root = n8;
	return t;
}

tree wantedTreeInTest__BlackNode18WithOneRedChild__() {
	tree _t;
	auto n8 = make_node(color::BLACK, 8);
	auto n5 = add_child_to_node(n8, color::BLACK, 5, childSide::LEFT);
	auto n17 = add_child_to_node(n8, color::BLACK, 17, childSide::RIGHT);
	_t.root = n8;
	return _t;
}

TEST(RemoveMethod, BlackNode18WithOneRedChild) {
	tree t = preTreeInTest__BlackNode18WithOneRedChild__();
	tree _t = wantedTreeInTest__BlackNode18WithOneRedChild__();
	assert_good_removal(t, _t, 18);
}

tree preTreeInTest__BlackNode5WithBlackBrotherWithLeftRedChild__() {
	tree t;
	auto n8 = make_node(color::BLACK, 8);
	auto n5 = add_child_to_node(n8, color::BLACK, 5,  childSide::LEFT);
	auto n18 = add_child_to_node(n8, color::BLACK, 18,  childSide::RIGHT);
	auto n15 = add_child_to_node(n18, color::RED, 15,  childSide::LEFT);
	t.root = n8;
	return t;
}

tree wantedTreeInTest__BlackNode5WithBlackBrotherWithLeftRedChild__() {
	tree _t;
	auto n15 = make_node(color::BLACK, 15);
	auto n8 = add_child_to_node(n15, color::BLACK, 8,  childSide::LEFT);
	auto n18 = add_child_to_node(n15, color::BLACK, 18,  childSide::RIGHT);
	_t.root = n15;
	return _t;
}

TEST(RemoveMethod, BlackNode5WithBlackBrotherWithLeftRedChild) {
	tree t = preTreeInTest__BlackNode5WithBlackBrotherWithLeftRedChild__();
	tree _t = wantedTreeInTest__BlackNode5WithBlackBrotherWithLeftRedChild__();
	assert_good_removal(t, _t, 5);
}

tree preTreeInTest__BlackNode5WithBlackBrotherWithRightRedChild__() {
	tree t;
	auto n8 = make_node(color::BLACK, 8);
	auto n5 = add_child_to_node(n8, color::BLACK, 5,  childSide::LEFT);
	auto n17 = add_child_to_node(n8, color::BLACK, 17,  childSide::RIGHT);
	auto n15 = add_child_to_node(n17, color::RED, 15,  childSide::LEFT);
	auto n18 = add_child_to_node(n17, color::RED, 18,  childSide::RIGHT);
	t.root = n8;
	return t;
}

tree wantedTreeInTest__BlackNode5WithBlackBrotherWithRightRedChild__() {
	tree _t;
	auto n17 = make_node(color::BLACK, 17);
	auto n8 = add_child_to_node(n17, color::BLACK, 8,  childSide::LEFT);
	auto n15 = add_child_to_node(n8, color::RED, 15,  childSide::RIGHT);
	auto n18 = add_child_to_node(n17, color::BLACK, 18,  childSide::RIGHT);
	_t.root = n17;
	return _t;
}

TEST(RemoveMethod, BlackNode5WithBlackBrotherWithRightRedChild) {
	tree t = preTreeInTest__BlackNode5WithBlackBrotherWithRightRedChild__();
	tree _t = wantedTreeInTest__BlackNode5WithBlackBrotherWithRightRedChild__();
	assert_good_removal(t, _t, 5);
}

tree preTreeInTest__BlackNode5WithRightRedBrother__() {
	tree t;
	auto n8 = make_node(color::BLACK, 8);
	auto n5 = add_child_to_node(n8, color::BLACK, 5,  childSide::LEFT);
	auto n17 = add_child_to_node(n8, color::RED, 17,  childSide::RIGHT);
	auto n15 = add_child_to_node(n17, color::BLACK, 15,  childSide::LEFT);
	auto n18 = add_child_to_node(n17, color::BLACK, 18,  childSide::RIGHT);
	auto n26 = add_child_to_node(n18, color::RED, 25,  childSide::RIGHT);
	t.root = n8;
	return t;
}

tree wantedTreeInTest__BlackNode5WithRightRedBrother__() {
	tree _t;
	auto n17 = make_node(color::BLACK, 17);
	auto n8 = add_child_to_node(n17, color::BLACK, 8,  childSide::LEFT);
	auto n15 = add_child_to_node(n8, color::RED, 15,  childSide::RIGHT);
	auto n18 = add_child_to_node(n17, color::BLACK, 18,  childSide::RIGHT);
	auto n25 = add_child_to_node(n18, color::RED, 25,  childSide::RIGHT);
	_t.root = n17;
	return _t;
}

TEST(RemoveMethod, BlackNode5WithRightRedBrother) {
	tree t = preTreeInTest__BlackNode5WithRightRedBrother__();
	tree _t = wantedTreeInTest__BlackNode5WithRightRedBrother__();
	assert_good_removal(t, _t, 5);
}


tree preTreeInTest__BlackNode10WithLeftRedBrother__() {
	tree t;
	auto n8 = make_node(color::BLACK, 8);
	auto n10 = add_child_to_node(n8, color::BLACK, 10,  childSide::RIGHT);
	auto n5 = add_child_to_node(n8, color::RED, 5,  childSide::LEFT);
	auto n7 = add_child_to_node(n5, color::BLACK, 7,  childSide::RIGHT);
	auto n4 = add_child_to_node(n5, color::BLACK, 4,  childSide::LEFT);
	auto n3 = add_child_to_node(n4, color::RED, 3,  childSide::LEFT);
	t.root = n8;
	return t;
}

tree wantedTreeInTest__BlackNode10WithLeftRedBrother__() {
	tree _t;
	auto n5 = make_node(color::BLACK, 5);
	auto n8 = add_child_to_node(n5, color::BLACK, 8, childSide::RIGHT);
	auto n7 = add_child_to_node(n8, color::RED, 7,  childSide::LEFT);
	auto n4 = add_child_to_node(n5, color::BLACK, 4,  childSide::LEFT);
	auto n3 = add_child_to_node(n4, color::RED, 3, childSide::LEFT);
	_t.root = n5;
	return _t;
}

TEST(RemoveMethod, BlackNode10WithLeftRedBrother) {
	tree t = preTreeInTest__BlackNode10WithLeftRedBrother__();
	tree _t = wantedTreeInTest__BlackNode10WithLeftRedBrother__();
	assert_good_removal(t, _t, 10);
}

tree preTreeInTest__RemoveRoot2OfTree__() {
	tree t;
	auto n2 = make_node(color::BLACK, 2);
	auto n1 = add_child_to_node(n2, color::BLACK, 1, childSide::LEFT);
	auto n3 = add_child_to_node(n2, color::BLACK, 3, childSide::RIGHT);
	auto n4 = add_child_to_node(n3, color::RED, 4, childSide::RIGHT);
	t.root = n2;
	return t;
}

tree wantedTreeInTest__RemoveRoot2OfTree__() {
	tree t;
	auto n3 = make_node(color::BLACK, 3);
	auto n1 = add_child_to_node(n3, color::BLACK, 1, childSide::LEFT);
	auto n4 = add_child_to_node(n3, color::BLACK, 4, childSide::RIGHT);
	t.root = n3;
	return t;
}

TEST(RemoveMethod, RemoveRoot2OfTree) {
	tree t = preTreeInTest__RemoveRoot2OfTree__();
	tree _t = wantedTreeInTest__RemoveRoot2OfTree__();
	assert_good_removal(t, _t, 2);
}

// tree preTreeInTest__RecolorRootAfterRemoving__() {
// 	tree t;
// 	auto n2 = make_node(color::BLACK, 2);
// 	auto n1 = add_child_to_node(n2, color::BLACK, 1, childSide::LEFT);
// 	auto n3 = add_child_to_node(n2, color::BLACK, 3, childSide::RIGHT);
// 	auto n4 = add_child_to_node(n3, color::RED, 4, childSide::RIGHT);
// 	t.root = n2;
// 	return t;
// }

// tree wantedTreeInTest__RecolorRootAfterRemoving__() {
// 	tree t;
// 	auto n3 = make_node(color::BLACK, 3);
// 	auto n1 = add_child_to_node(n3, color::BLACK, 1, childSide::LEFT);
// 	auto n4 = add_child_to_node(n3, color::BLACK, 4, childSide::RIGHT);
// 	t.root = n3;
// 	return t;
// }

// TEST(RemoveMethod, RecolorRootAfterRemoving) {
// 	tree t = preTreeInTest__RemoveRoot2OfTree__();
// 	tree _t = wantedTreeInTest__RemoveRoot2OfTree__();
// 	assert_good_removal(t, _t, 2);
// }


#endif

int main(int argc, char **argv)
{
  	::testing::InitGoogleTest(&argc, argv);
  
	return RUN_ALL_TESTS();
}