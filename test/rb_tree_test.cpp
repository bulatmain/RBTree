#include <gtest/gtest.h>

#include <rb_tree.hpp>

#include <iostream>

using namespace cust;

using tree = RBTree<double>;
using node = RBTree<double>::Node;
using color = RBTree<double>::Color;

enum ChildSide {
	LEFT,
	RIGHT
};

void set_link(tree::node_ptr new_parent, tree::node_ptr new_child, ChildSide s) {
	if (s == ChildSide::LEFT) {
		new_parent->left = new_child;
	} else {
		new_parent->right = new_child;
	}
	new_child->parent = new_parent;
}

tree::node_ptr add_child_to_node(tree::node_ptr n, color c, double val, ChildSide s) {
	auto _n = std::make_shared<node>(c, val);
	set_link(n, _n, s);
	return _n;
}

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
	tree::node_ptr root = std::make_shared<node>(tree::Color::BLACK, 8);
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
	auto n5 = add_child_to_node(n8, color::RED, 5, ChildSide::LEFT);
	auto n18 = add_child_to_node(n8, color::RED, 18, ChildSide::RIGHT);
	return t;
}

tree wantedTreeInTest__Addition15CauseRecolorWithRoot__() {
	tree t = preTreeInTest__Addition15CauseRecolorWithRoot__();
	auto n8  = t.root;    n8->color  = color::BLACK;
	auto n5  = n8->left;  n5->color  = color::BLACK;
	auto n18 = n8->right; n18->color = color::BLACK;
	auto n15 = add_child_to_node(n18, color::RED, 15, ChildSide::LEFT);
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

	auto n17 = add_child_to_node(n8, color::BLACK, 17, ChildSide::RIGHT);

	set_link(n17, n15, ChildSide::LEFT);
	n15->left = n15->right = nullptr;
	n15->color = color::RED;

	set_link(n17, n18, ChildSide::RIGHT);
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
	auto n25 = add_child_to_node(n18, color::RED, 25, ChildSide::RIGHT);
	return t;
}

tree wantedTreeInTest__Addition40CauseLLRotationAndRecolor__() {
	tree t = preTreeInTest__Addition40CauseLLRotationAndRecolor__();
	auto n8 = t.root;
	auto n17 = n8->right;
	auto n18 = n17->right;
	auto n25 = n18->right;
	set_link(n17, n25, ChildSide::RIGHT);
	set_link(n25, n18, ChildSide::LEFT);
	n18->left = n18->right = nullptr;
	n18->color = color::RED;
	n25->color = color::BLACK;
	auto n40 = add_child_to_node(n25, color::RED, 40, ChildSide::RIGHT);
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
	auto n17 = std::make_shared<node>(color::BLACK, 17);
	auto n8  = add_child_to_node(n17, color::RED,    8, ChildSide::LEFT);
	auto n25 = add_child_to_node(n17, color::RED,   25, ChildSide::RIGHT);
	auto n5  = add_child_to_node(n8,  color::BLACK,  5, ChildSide::LEFT);
	auto n15 = add_child_to_node(n8,  color::BLACK, 15, ChildSide::RIGHT);
	auto n18 = add_child_to_node(n25, color::BLACK, 18, ChildSide::LEFT);
	auto n40 = add_child_to_node(n25, color::BLACK, 40, ChildSide::RIGHT);
	auto n80 = add_child_to_node(n40, color::RED,   80, ChildSide::RIGHT);
	t.root = n17;
	return t;
}

TEST(AddMethod, Addition80CauseRecolorAndLRotationAndRecolor) {
	tree t = preTreeInTest__Addition80CauseRecolorAndLRotationAndRecolor__();
	tree _t = wantedTreeInTest__Addition80CauseRecolorAndLRotationAndRecolor__();
	assert_good_addition(t, _t, 80);
}


TEST(EmptyMethod, TreeEmptyAfterConstruction) {
	RBTree<double> tree;
	ASSERT_TRUE(tree.empty());
}

TEST(FindInSubtreeMethod, EmptyRBTreeBad) {
	std::shared_ptr<node> root;
	ASSERT_THROW(
		{tree::findInSubtree(root, 0);},
		NoSuchElementInSubtree
	);
}

	
tree::node_ptr rb_tree_example_1() {
	auto n4 = std::make_shared<node>(color::BLACK, 4);
	auto n2 = add_child_to_node(n4, color::RED, 2, ChildSide::LEFT);
	auto n6 = add_child_to_node(n4, color::BLACK, 6, ChildSide::RIGHT);
	auto n0 = add_child_to_node(n2, color::BLACK, 0, ChildSide::LEFT);
	auto n3 = add_child_to_node(n2, color::BLACK, 3, ChildSide::RIGHT);
	auto n5 = add_child_to_node(n6, color::BLACK, 5, ChildSide::LEFT);
	auto n8 = add_child_to_node(n6, color::BLACK, 8, ChildSide::RIGHT);
	auto n1 = add_child_to_node(n0, color::RED, 1, ChildSide::RIGHT);
	auto n7 = add_child_to_node(n8, color::RED, 7, ChildSide::LEFT);
	auto n9 = add_child_to_node(n8, color::RED, 9, ChildSide::RIGHT);

	return n4;
}

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


void rb_tree_example_1_find_parent_element_good(double val, double prev) {
	auto root = rb_tree_example_1();

	std::shared_ptr<node> n;
	try {
		n = tree::findLeafParentInSubtree(root, val);
	} catch(...) {
		FAIL();
	}
	ASSERT_TRUE(*n->value == prev);
}

void rb_tree_example_1_find_parent_element_bad(double val) {
	auto root = rb_tree_example_1();
	ASSERT_THROW(
		{tree::findLeafParentInSubtree(root, val);},
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
	auto root = rb_tree_example_1();
	auto node = tree::addToLeafOfSubtree(root, 7.5);
	auto parent = node->parent.lock();
	ASSERT_TRUE(parent);
	ASSERT_EQ(*(parent->value), 7.0);
	ASSERT_EQ(parent->right, node);
}

TEST(AddToLeafOfSubtreeMethod, RBTreeExample1Addm1ToLeaf) {
	auto root = rb_tree_example_1();
	auto node = tree::addToLeafOfSubtree(root, -1.0);
	auto parent = node->parent.lock();
	ASSERT_TRUE(parent);
	ASSERT_EQ(*(parent->value), 0);
	ASSERT_EQ(parent->left, node);
}

TEST(AddToLeafOfSubtreeMethod, RBTreeEmptyAdd0ToLeaf) {
	tree::node_ptr root = nullptr;
	ASSERT_THROW(
		{ auto node = tree::addToLeafOfSubtree(root, 0); },
		TreeEmpty
	);
}


int main(int argc, char **argv)
{
  	::testing::InitGoogleTest(&argc, argv);
  
	return RUN_ALL_TESTS();
}