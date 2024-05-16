#ifndef RB_TREE_HPP
#define RB_TREE_HPP

#ifdef RBTREE_TESTING
#define private public
#define protected public
#endif

#include "rb_tree_exceptions.hpp"

#include <functional>
#include <iostream>
#include <iomanip>
#include <memory>

namespace cust {
    template <class T>
    std::function<bool(T const&, T const&)> const equal_to = [](T const& a, T const& b) { return a == b; };

    template <class T>
    std::function<bool(T const&, T const&)> const less = [](T const& a, T const& b) { return a < b; };

    template <class T>
    class RBTree {
    protected:
        enum Color {
            BLACK,
            RED
        };

        enum ChildSide {
        	LEFT,
        	RIGHT
        };

        class Node;
        using node_ptr = std::shared_ptr<Node>;
        using wnode_ptr = std::weak_ptr<Node>;
    public:
        using comparator = std::function<bool(T const&, T const&)>;
        using value_ptr = std::shared_ptr<T>;

        RBTree(comparator const& equal_to = cust::equal_to<T>, 
               comparator const& less = cust::less<T>) : equal_to(equal_to), less(less) {}
        RBTree(RBTree<T>&& other);

        RBTree& operator=(RBTree<T>&& other);

        value_ptr find(T const& value) const;
        void add(T const& value);
        value_ptr remove(T const& value);
        bool empty() const;
        uint64_t size() const;

        void setEqComparator(comparator const& equal_to);

        bool operator==(RBTree<T> const& other) const;

        static void saveInStream(std::ostream& os, node_ptr root);
        void saveInStream(std::ostream& os) const;

        static RBTree<T> readFromStream(std::istream& is,
                comparator const& equal_to = cust::equal_to<T>, 
                comparator const& less = cust::less<T>);

        static void printTree(std::ostream& os, node_ptr root, int ident = 0);
        void printTree(std::ostream& os) const;

    protected:
        class AdditionMethodImplementation;
        class RemovalMethodImplementation;

        static bool subtreesEqual(node_ptr r1, node_ptr r2);
        static node_ptr findInSubtree(node_ptr root, T const& value);

        node_ptr rightRotate(node_ptr node);
        node_ptr leftRotate(node_ptr node);

        void move(RBTree<T>&& other);

        static node_ptr readSubtreeFromStream(std::istream& is,
                comparator const& equal_to, 
                comparator const& less);

    protected:
        node_ptr root;
        uint64_t _size;

        comparator equal_to;
        comparator less;        

    };

    template <class T>
    class RBTree<T>::Node {
    public:
        static size_t count;
        size_t const id;

        Node(Color color, value_ptr value,
             comparator const& equal_to, 
             comparator const& less) : color(color), value(value), id(++count),
             equal_to(equal_to), less(less) {}

        bool operator==(Node const& other) const;

        bool hasNoKids() const;

        std::ostream& print(std::ostream& os) const;
        std::istream& read(std::istream& is);

    public:
        static bool leftIsTheOne(node_ptr node, T const& value);
        static bool rightIsTheOne(node_ptr node, T const& value);

    public:
        node_ptr left;
        node_ptr right;
        wnode_ptr parent;
        Color color;
        value_ptr value;

        comparator const& equal_to;
        comparator const& less;    

    };

    template <class T>
    size_t RBTree<T>::Node::count = 0;

    template <class T>
    class RBTree<T>::AdditionMethodImplementation {
    protected:
        RBTree* const tree;

    public:
        AdditionMethodImplementation(RBTree<T>* const tree)
         : tree(tree), equal_to(tree->equal_to), less(tree->less) {}

        void run(T const& value);

    protected:
        void balanceFrom(node_ptr node);
        node_ptr leftChildParentCaseBalance(node_ptr node);
        node_ptr rightChildParentCaseBalance(node_ptr node);
        void restoreRootProperty();

        node_ptr findLeafParentInSubtree(node_ptr root, T const& value);

        static node_ptr recolorParentAndGrandfather(node_ptr node);
        static node_ptr recolorParentAndUncleAndGrandfather(node_ptr node);

        static bool redNode(node_ptr node);
        static bool redParent(node_ptr node);
        static bool redUncleCase(node_ptr node);
        static bool noGrandFatherCase(node_ptr node);
        static bool leftChildParentCase(node_ptr node);
        static bool rightChildParentCase(node_ptr node);
        static bool leftChildNodeCase(node_ptr node);
        static bool rightChildNodeCase(node_ptr node);
        

        node_ptr makeNode(Color color, T const& value);
        node_ptr addToLeafOfSubtree(node_ptr root, T const& value);
        node_ptr addNodeToLeaf(node_ptr node, T const& value);
        node_ptr addNodeToLeftLeaf(node_ptr node, T const& value);
        node_ptr addNodeToRightLeaf(node_ptr node, T const& value);

        comparator const& equal_to;
        comparator const& less;

    };


    template <class T>
    class RBTree<T>::RemovalMethodImplementation {
    protected:
        RBTree* const tree;

    public:
        RemovalMethodImplementation(RBTree<T>* const tree)
         : tree(tree), equal_to(tree->equal_to), less(tree->less) {}

        value_ptr run(T const& value);

    protected:
        node_ptr removeNode(node_ptr node);
        void fixBlackHeight(node_ptr parent, ChildSide problemSide);
        void fixBlackHeightForLeft(node_ptr parent);
        void fixBlackHeightForRight(node_ptr parent);

        void runFixFromGrandFather(node_ptr node);

        static bool blackChildrenCase(node_ptr node);
        static bool redLeftChildCase(node_ptr node);
        static bool redRightChildCase(node_ptr node);

        static bool childlessNodeCase(node_ptr);
        static bool nodeWithOneChildCase(node_ptr);

        node_ptr removeChildlessNode(node_ptr);
        node_ptr removeNodeWithOneChild(node_ptr);
        node_ptr removeNodeWithTwoChildren(node_ptr);

        static node_ptr findLeastLargestNodeFromNodeWithTwoChildren(node_ptr);

    protected:
        comparator const& equal_to;
        comparator const& less;

    };

    // RBTree class methods implementation
#ifdef RB_TREE_HPP
#define RB_TREE_HPP

    template <class T>
    RBTree<T>::RBTree(RBTree<T>&& other) {
        move(std::move(other));
    }

    template <class T>
    RBTree<T>& RBTree<T>::operator=(RBTree<T>&& other) {
        move(std::move(other));
        return *this;
    }

    template <class T>
    void RBTree<T>::move(RBTree<T>&& other) {
        root = other.root;
        _size = other._size;
        equal_to = other.equal_to;
        less = other.less;

        other.root = nullptr;
        other._size = 0;
        other.equal_to = cust::equal_to<T>;
        other.less = cust::less<T>;
    }

    template <class T>
    typename RBTree<T>::value_ptr RBTree<T>::find(T const& value) const {
        try {
            node_ptr node = findInSubtree(root, value);
            return node->value;
        } catch (NoSuchElementInSubtree const& e) {
            throw NoSuchElement("Error: no such element in RBTree!");
        }
    }

    template <class T>
    void RBTree<T>::add(T const& value) {
        AdditionMethodImplementation impl(this);
        impl.run(value);
    }

    template <class T>
    typename RBTree<T>::value_ptr RBTree<T>::remove(T const& value) {
        RemovalMethodImplementation impl(this);
        return impl.run(value);
    }

    template <class T>
    bool RBTree<T>::empty() const {
        return !root;
    }

    template <class T>
    uint64_t RBTree<T>::size() const {
        return _size;
    }

    template <class T>
    void RBTree<T>::setEqComparator(comparator const& equal_to) {
        this->equal_to = equal_to;
    }

    template <class T>
    bool RBTree<T>::operator==(RBTree<T> const& other) const {
        return subtreesEqual(root, other.root);
    }


    template <class T>
    void RBTree<T>::saveInStream(std::ostream& os, node_ptr root) {
        if (root) {
            root->print(os);
            saveInStream(os, root->left);
            saveInStream(os, root->right);
        } else {
            os << "N";
        }
    }

    template <class T>
    void RBTree<T>::saveInStream(std::ostream& os) const {
        RBTree<T>::saveInStream(os, root);
    }
    
    template <class T>
    RBTree<T> RBTree<T>::readFromStream(std::istream& is,
            comparator const& equal_to, 
            comparator const& less) {
        RBTree<T> tree;
        auto root = readSubtreeFromStream(is, equal_to, less);
        tree.root = root;
        return tree;
    }

    template <class T>
    typename RBTree<T>::node_ptr RBTree<T>::readSubtreeFromStream(std::istream& is,
            comparator const& equal_to, 
            comparator const& less) {
        auto root = std::make_shared<RBTree<T>::Node>(Color::BLACK, nullptr, equal_to, less);
        try {
            root->read(is);
        } catch (...) {
            return nullptr;
        }
        root->left = readSubtreeFromStream(is, equal_to, less);
        if (root->left) {
            root->left->parent = root;
        }
        root->right = readSubtreeFromStream(is, equal_to, less);
        if (root->right) {
            root->right->parent = root;
        }
        return root;
    }

    template <class T>
    void RBTree<T>::printTree(std::ostream& os, node_ptr root, int indent) {
        if(root != NULL) {
            if(root->right) {
                printTree(os, root->right, indent + 4);
            } if (indent) {
                os << std::setw(indent) << ' ';
            } if (root->right) {
                os <<" /\n" << std::setw(indent) << ' ';
            }
    		root->print(os) << "\n ";
            if (root->left) {
                os << std::setw(indent) << ' ' <<" \\\n";
                printTree(os, root->left, indent + 4);
            }
        }
    }

    template <class T>
    void RBTree<T>::printTree(std::ostream& os) const {
        RBTree<T>::printTree(os, root);
    }

    template <class T>
    typename RBTree<T>::node_ptr RBTree<T>::findInSubtree(node_ptr root, T const& value) {
        if (!root) {
            throw NoSuchElementInSubtree("Error: no such element in subtree");
        } else if (root->equal_to(*root->value, value)) {
            return root;
        } else if (root->less(*root->value, value)) {
            return findInSubtree(root->right, value);
        } else {
            return findInSubtree(root->left, value);
        }
    }

    template <class T>
    bool RBTree<T>::subtreesEqual(node_ptr r1, node_ptr r2) {
        if (static_cast<bool>(r1) != static_cast<bool>(r2)) {
            return false;
        } else {
            return  !(r1 || r2) ||
                    (*r1 == *r2) && 
                    subtreesEqual(r1->left, r2->left) &&
                    subtreesEqual(r1->right, r2->right);
        }
    }

    template <class T>
    typename RBTree<T>::node_ptr RBTree<T>::leftRotate(node_ptr node) {
        auto parent = node->parent.lock();
        
        // Set names
        auto root = node;
        auto pivot = node->right;
        auto midSubTree = pivot->left;
        if (root == this->root) {
            this->root = pivot;
        }

        // Set link between root and midSubTree
        root->right = midSubTree;
        if (midSubTree) {
            midSubTree->parent = root;
        }

        // Set link between root and pivot
        pivot->left = root;
        root->parent = pivot;

        // Set link between pivot and parent
        if (parent) {
            if (parent->left == node) {
                parent->left = pivot;
            } else {
                parent->right = pivot;
            }
        }
        pivot->parent = parent;

        return pivot;
    }

    template <class T>
    typename RBTree<T>::node_ptr RBTree<T>::rightRotate(node_ptr node) {
        auto parent = node->parent.lock();
        
        // Set names
        auto root = node;
        auto pivot = node->left;
        auto midSubTree = pivot->right;
        if (root == this->root) {
            this->root = pivot;
        }

        // Set link between root and midSubTree
        root->left = midSubTree;
        if (midSubTree) {
            midSubTree->parent = root;
        }

        // Set link between root and pivot
        pivot->right = root;
        root->parent = pivot;
        
        // Set link between pivot and parent
        if (parent) {
            if (parent->left == root) {
                parent->left = pivot;
            } else {
                parent->right = pivot;
            }
        }
        pivot->parent = parent;
        return pivot;
    }

#endif

    // Node class methods implementation
#ifdef RB_TREE_HPP
#define RB_TREE_HPP
    template <class T>
    bool RBTree<T>::Node::leftIsTheOne(node_ptr node, T const& value) {
        return (node != nullptr) && (node->left != nullptr) && node->equal_to(*node->left->value, value);
    }

    template <class T>
    bool RBTree<T>::Node::rightIsTheOne(node_ptr node, T const& value) {
        return (node != nullptr) && (node->right != nullptr) && node->equal_to(*node->right->value == value);
    }

    template <class T>
    bool RBTree<T>::Node::operator==(Node const& other) const {
        return (this->color == other.color) && equal_to(*this->value, *other.value);
    }

    template <class T>
    bool RBTree<T>::Node::hasNoKids() const {
        return (left == nullptr) && (right == nullptr);
    }

    template <class T>
    std::ostream& RBTree<T>::Node::print(std::ostream& os) const {
        return os << "(" << *value << ", " << color << ")";
    }

    template <class T>
    std::istream& RBTree<T>::Node::read(std::istream& is) {
        char _format_char;
        is >> _format_char;
        if (_format_char == 'N') {
            throw std::runtime_error("Error: bad reading RBTree node!");
        }
        value = std::make_shared<T>();
        is >> *value;
        is >> _format_char;
        char coloc_ch;
        is >> coloc_ch;
        color = (coloc_ch == '0' ? BLACK : RED);
        is >> _format_char;
        return is;
    }

#endif

    // AdditionMethodImplementation class methods implementation
#ifdef RB_TREE_HPP
#define RB_TREE_HPP
    template <class T>
    void RBTree<T>::AdditionMethodImplementation::run(T const& value) {
        if (tree->empty()) {
            tree->root = makeNode(BLACK, value);
        } else {
            auto node = addToLeafOfSubtree(tree->root, value);
            balanceFrom(node);
        }
        ++tree->_size;
    }

    template <class T>
    typename RBTree<T>::node_ptr RBTree<T>::AdditionMethodImplementation::findLeafParentInSubtree(node_ptr root, T const& value) {
        if (!root) {
            throw TreeEmpty("Error: empty tree");
        } else if (equal_to(*root->value, value)) {
            throw NoLeafParentElementInTree("Error: no leaf parent in subtree");
        } else if (less(*root->value, value)) {
            if (!root->right) {
                return root;
            } else {
                return findLeafParentInSubtree(root->right, value);
            }
        } else {
            if (!root->left) {
                return root;
            } else {
                return findLeafParentInSubtree(root->left, value);
            }
        }
    }

    template <class T>
    typename RBTree<T>::node_ptr RBTree<T>::AdditionMethodImplementation::addToLeafOfSubtree(node_ptr root, T const& value) {
        try {
            node_ptr node = findLeafParentInSubtree(root, value);
            return addNodeToLeaf(node, value);
        } catch (TreeEmpty const& e) {
            throw e;
        } catch (...) {
            throw TreeHasGivenElement("Error: tree has element with given value");
        }
    }

    template <class T>
    void RBTree<T>::AdditionMethodImplementation::balanceFrom(node_ptr node) {
        while (redNode(node) && redParent(node)) {
            if (redUncleCase(node)) {
                node = recolorParentAndUncleAndGrandfather(node);
            } else if (leftChildParentCase(node)) {
                node = leftChildParentCaseBalance(node);
            } else {
                node = rightChildParentCaseBalance(node);
            }
        }
        restoreRootProperty();
    }

    template <class T>
    bool RBTree<T>::AdditionMethodImplementation::redNode(node_ptr node) {
        return (node) && (node->color == RED);
    }

    template <class T>
    bool RBTree<T>::AdditionMethodImplementation::redParent(node_ptr node) {
        auto parent = node->parent.lock();
        return (parent) && (parent->color == Color::RED);
    }

    template <class T>
    bool RBTree<T>::AdditionMethodImplementation::noGrandFatherCase(node_ptr node) {
        auto parent = node->parent.lock();
        if (!parent) return true;
        return !parent->parent.lock();
    } 

    template <class T>
    bool RBTree<T>::AdditionMethodImplementation::leftChildParentCase(node_ptr node) {
        auto parent = node->parent.lock();
        if (!parent) return false;
        auto grandparent = parent->parent.lock();
        if (!grandparent) return false;
        return grandparent->left == parent;
    }

    template <class T>
    typename RBTree<T>::node_ptr RBTree<T>::AdditionMethodImplementation::leftChildParentCaseBalance(node_ptr node) {
        if (rightChildNodeCase(node)) {
            node = node->parent.lock();
            node = tree->leftRotate(node)->left;
        }
        auto grandfather = recolorParentAndGrandfather(node);
        return tree->rightRotate(grandfather);
    }

    template <class T>
    typename RBTree<T>::node_ptr RBTree<T>::AdditionMethodImplementation::rightChildParentCaseBalance(node_ptr node) {
        if (leftChildNodeCase(node)) {
            node = node->parent.lock();
            node = tree->rightRotate(node)->right;
        }
        auto grandfather = recolorParentAndGrandfather(node);
        return tree->leftRotate(grandfather);
    }

    template <class T>
    bool RBTree<T>::AdditionMethodImplementation::redUncleCase(node_ptr node) {
        auto parent = node->parent.lock();
        if (!parent) return false;
        auto grandfather = parent->parent.lock();
        if (!grandfather) return false;
        node_ptr uncle;
        if (parent == grandfather->left) {
            uncle = grandfather->right;
        } else {
            uncle = grandfather->left;
        }
        return (uncle) && (uncle->color == RED);
    } 

    template <class T>
    typename RBTree<T>::node_ptr RBTree<T>::AdditionMethodImplementation::recolorParentAndGrandfather(node_ptr node) {
        auto parent = node->parent.lock();
        auto grandfather = parent->parent.lock();
        parent->color = BLACK;
        grandfather->color = RED;
        return grandfather;
    }

    template <class T>
    typename RBTree<T>::node_ptr RBTree<T>::AdditionMethodImplementation::recolorParentAndUncleAndGrandfather(node_ptr node) {
        auto parent = node->parent.lock();
        auto grandfather = parent->parent.lock();
        node_ptr uncle;
        if (parent == grandfather->right) {
            uncle = grandfather->left;
        } else {
            uncle = grandfather->right;
        }
        parent->color = uncle->color = BLACK;
        grandfather->color = RED;
        return grandfather;
    }

    template <class T>
    bool RBTree<T>::AdditionMethodImplementation::rightChildNodeCase(node_ptr node) {
        auto parent = node->parent.lock();
        return (parent) && (parent->right == node);
    }

    template <class T>
    bool RBTree<T>::AdditionMethodImplementation::leftChildNodeCase(node_ptr node) {
        auto parent = node->parent.lock();
        return (parent) && (parent->left == node);
    }

    template <class T>
    void RBTree<T>::AdditionMethodImplementation::restoreRootProperty() {
        tree->root->color = BLACK;
    }

    template <class T>
    typename RBTree<T>::node_ptr RBTree<T>::AdditionMethodImplementation::addNodeToLeaf(node_ptr node, T const& value) {
        node_ptr leaf;
        if (equal_to(*node->value, value)) {
            throw TreeHasGivenElement("Error: can not add repeating element to leaf!");
        } else if (less(*node->value, value)) {
            leaf = addNodeToRightLeaf(node, value);
        } else {
            leaf = addNodeToLeftLeaf(node, value);
        }
        leaf->parent = node;
        return leaf;
    }

    template <class T>
    typename RBTree<T>::node_ptr RBTree<T>::AdditionMethodImplementation::addNodeToLeftLeaf(node_ptr node, T const& value) {
        if (!node->left) {
            return node->left = makeNode(Color::RED, value); 
        } else {
            throw TreeHasGivenElement("Error: can not add leaf to node!");
        }
    }

    template <class T>
    typename RBTree<T>::node_ptr RBTree<T>::AdditionMethodImplementation::addNodeToRightLeaf(node_ptr node, T const& value) {
        if (!node->right) {
            return node->right = makeNode(Color::RED, value); 
        } else {
            throw TreeHasGivenElement("Error: can not add leaf to node!");
        }
    }

    template <class T>
    typename RBTree<T>::node_ptr RBTree<T>::AdditionMethodImplementation::makeNode(Color color, T const& value) {
        auto v_ptr = std::make_shared<T>(value);
        return std::make_shared<Node>(color, v_ptr, equal_to, less);
    }

#endif

// RemovalMethodImplementation class methods implementation
#ifdef RB_TREE_HPP
#define RB_TREE_HPP

template <class T>
typename RBTree<T>::value_ptr RBTree<T>::RemovalMethodImplementation::run(T const& value) {
    if (tree->empty()) {
        throw TreeEmpty("Error: can not remove node from empty RBTree!");
    } try {
        auto node = tree->findInSubtree(tree->root, value);
        auto value = node->value;
        node = removeNode(node);
        auto parent = node->parent.lock();
        if (!parent) {
            node->color = BLACK;
        } else if (node->color == BLACK) {
            auto side = (tree->less(*node->value, *parent->value) ? LEFT : RIGHT);
            fixBlackHeight(parent, side);
            tree->root->color = BLACK;
        }
        --tree->_size;
        return value;
    } catch(NoSuchElementInSubtree const& e) {
        throw NoSuchElement(e.what());
    }
}

template <class T>
typename RBTree<T>::node_ptr RBTree<T>::RemovalMethodImplementation::removeNode(node_ptr node) {
    if (childlessNodeCase(node)) {
        node = removeChildlessNode(node);
    } else if (nodeWithOneChildCase(node)) {
        node = removeNodeWithOneChild(node);
    } else {
        node = removeNodeWithTwoChildren(node);
    }
    return node;
}

template <class T>
bool RBTree<T>::RemovalMethodImplementation::childlessNodeCase(node_ptr node) {
    return !(node->left || node->right);
}
template <class T>
bool RBTree<T>::RemovalMethodImplementation::nodeWithOneChildCase(node_ptr node) {
    return (node->left && !node->right) || (!node->left && node->right);
}

template <class T>
typename RBTree<T>::node_ptr  RBTree<T>::RemovalMethodImplementation::removeChildlessNode(node_ptr node) {
    auto parent = node->parent.lock();
    if (!parent) {
        tree->root = nullptr;
    } else if (node == parent->left) {
        parent->left = nullptr;
    } else {
        parent->right = nullptr;
    }
    return node;
}

template <class T>
typename RBTree<T>::node_ptr  RBTree<T>::RemovalMethodImplementation::removeNodeWithOneChild(node_ptr node) {
    auto parent = node->parent.lock();
    auto child = (node->left ? node->left : node->right);
    if (!parent) {
        tree->root = child;
        child->parent = wnode_ptr();
        tree->root->color = BLACK;
    } else if (node == parent->left) {
        parent->left = child;
        child->parent = parent;
    } else {
        parent->right = child;
        child->parent = parent;
    }
    return node;
}

template <class T>
typename RBTree<T>::node_ptr  RBTree<T>::RemovalMethodImplementation::removeNodeWithTwoChildren(node_ptr node) {
    auto next = findLeastLargestNodeFromNodeWithTwoChildren(node);
    node->value = next->value;
    if (next->right) {
        return removeNodeWithOneChild(next);
    } else {
        return removeChildlessNode(next);
    }
}

template <class T>
typename RBTree<T>::node_ptr RBTree<T>::RemovalMethodImplementation::findLeastLargestNodeFromNodeWithTwoChildren(node_ptr node) {
    auto next = node->right;
    while (next->left) {
        next = next->left;
    }
    return next;
}


template <class T>
void RBTree<T>::RemovalMethodImplementation::fixBlackHeight(node_ptr parent, ChildSide problemSide) {
    auto grandparent = parent->parent.lock();
    auto child = (problemSide == LEFT ? parent->left : parent->right);
    if (child && child->color == RED) {
        child->color = BLACK;
        return;
    } else if (problemSide == LEFT) {
        fixBlackHeightForLeft(parent);
    } else {
        fixBlackHeightForRight(parent);
    }
}


template <class T>
void RBTree<T>::RemovalMethodImplementation::fixBlackHeightForLeft(node_ptr parent) {
    auto brother = parent->right;
    if (brother->color == RED) {
        tree->leftRotate(parent);
        auto grandparent =  parent->parent.lock();
        parent->color = RED;
        grandparent->color = BLACK;
        fixBlackHeightForLeft(parent);
    } else {
        if (blackChildrenCase(brother)) {
            brother->color = RED;
            runFixFromGrandFather(parent);
        } else if (redRightChildCase(brother)) {
            tree->leftRotate(parent);
            brother->color = parent->color;
            parent->color = brother->right->color = BLACK;
        } else {
            tree->rightRotate(brother);
            brother->color = RED;
            brother->parent.lock()->color = BLACK;
            fixBlackHeightForLeft(parent);
        }
    }
}


template <class T>
void RBTree<T>::RemovalMethodImplementation::fixBlackHeightForRight(node_ptr parent) {
    auto brother = parent->left;
    if (brother->color == RED) {
        tree->rightRotate(parent);
        auto grandparent =  parent->parent.lock();
        parent->color = RED;
        grandparent->color = BLACK;
        fixBlackHeightForRight(parent);
    } else {
        if (blackChildrenCase(brother)) {
            brother->color = RED;
            runFixFromGrandFather(parent);
        } else if (redLeftChildCase(brother)) {
            tree->rightRotate(parent);
            brother->color = parent->color;
            parent->color = brother->left->color = BLACK;
        } else {
            tree->leftRotate(brother);
            brother->color = RED;
            brother->parent.lock()->color = BLACK;
            fixBlackHeightForRight(parent);
        }
    }
}

template <class T>
void RBTree<T>::RemovalMethodImplementation::runFixFromGrandFather(node_ptr parent) {
    auto grandfather = parent->parent.lock();
    if (!grandfather) {
        return;
    } else if (parent == grandfather->left) {
        fixBlackHeight(grandfather, LEFT);
    } else {
        fixBlackHeight(grandfather, RIGHT);
    }
}


template <class T>
bool RBTree<T>::RemovalMethodImplementation::blackChildrenCase(node_ptr node) {
    return (!node->left || node->left->color == BLACK) && 
           (!node->right || node->right->color == BLACK);
}

template <class T>
bool RBTree<T>::RemovalMethodImplementation::redRightChildCase(node_ptr node) {
    return (node->right && node->right->color == RED);
}

template <class T>
bool RBTree<T>::RemovalMethodImplementation::redLeftChildCase(node_ptr node) {
    return (node->left && node->left->color == RED);
}


#endif


};

#endif