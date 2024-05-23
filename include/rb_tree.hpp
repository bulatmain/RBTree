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
    struct EqualTo {
        static constexpr bool cmp(T const& a, T const& b) {
            return a == b;
        }
    };

    template <class T>
    struct Less {
        static constexpr bool cmp(T const& a, T const& b) {
            return a < b;
        }
    };

    template <class T, typename EqualTo = EqualTo<T>, typename Less = Less<T>>
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

        RBTree() = default;
        RBTree(RBTree<T, EqualTo, Less>&& other);

        RBTree& operator=(RBTree<T, EqualTo, Less>&& other);

        value_ptr find(T const& value) const;
        void add(T const& value);
        value_ptr remove(T const& value);
        bool empty() const;
        uint64_t size() const;

        bool operator==(RBTree<T, EqualTo, Less> const& other) const;

        static void saveInStream(std::ostream& os, node_ptr root);
        void saveInStream(std::ostream& os) const;

        static RBTree<T, EqualTo, Less> readFromStream(std::istream& is);

        static void printTree(std::ostream& os, node_ptr root, int ident = 0);
        void printTree(std::ostream& os) const;

    protected:
        class AdditionMethodImplementation;
        class RemovalMethodImplementation;

        static bool subtreesEqual(node_ptr r1, node_ptr r2);
        static node_ptr findInSubtree(node_ptr root, T const& value);

        node_ptr rightRotate(node_ptr node);
        node_ptr leftRotate(node_ptr node);

        void move(RBTree<T, EqualTo, Less>&& other);

        static node_ptr readSubtreeFromStream(std::istream& is);

    protected:
        node_ptr root;
        uint64_t _size;

    };

    template <class T, typename EqualTo, typename Less>
    class RBTree<T, EqualTo, Less>::Node {
    public:

        Node(Color color, value_ptr value) 
            : color(color), value(value), id(++count) {}

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
        
        static size_t count;
        size_t const id;

    };

    template <class T, typename EqualTo, typename Less>
    size_t RBTree<T, EqualTo, Less>::Node::count = 0;

    template <class T, typename EqualTo, typename Less>
    class RBTree<T, EqualTo, Less>::AdditionMethodImplementation {
    protected:
        RBTree* const tree;

    public:
        AdditionMethodImplementation(RBTree<T, EqualTo, Less>* const tree)
         : tree(tree) {}

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

    };


    template <class T, typename EqualTo, typename Less>
    class RBTree<T, EqualTo, Less>::RemovalMethodImplementation {
    protected:
        RBTree* const tree;

    public:
        RemovalMethodImplementation(RBTree<T, EqualTo, Less>* const tree)
         : tree(tree) {}

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

    };

    // RBTree class methods implementation
#ifdef RB_TREE_HPP
#define RB_TREE_HPP

    template <class T, typename EqualTo, typename Less>
    RBTree<T, EqualTo, Less>::RBTree(RBTree&& other) {
        move(std::move(other));
    }

    template <class T, typename EqualTo, typename Less>
    RBTree<T, EqualTo, Less>& RBTree<T, EqualTo, Less>::operator=(RBTree<T, EqualTo, Less>&& other) {
        move(std::move(other));
        return *this;
    }

    template <class T, typename EqualTo, typename Less>
    void RBTree<T, EqualTo, Less>::move(RBTree&& other) {
        root = other.root;
        _size = other._size;

        other.root = nullptr;
        other._size = 0;
    }

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::value_ptr RBTree<T, EqualTo, Less>::find(T const& value) const {
        try {
            node_ptr node = findInSubtree(root, value);
            return node->value;
        } catch (NoSuchElementInSubtree const& e) {
            throw NoSuchElement("Error: no such element in RBTree!");
        }
    }

    template <class T, typename EqualTo, typename Less>
    void RBTree<T, EqualTo, Less>::add(T const& value) {
        AdditionMethodImplementation impl(this);
        impl.run(value);
    }

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::value_ptr RBTree<T, EqualTo, Less>::remove(T const& value) {
        RemovalMethodImplementation impl(this);
        return impl.run(value);
    }

    template <class T, typename EqualTo, typename Less>
    bool RBTree<T, EqualTo, Less>::empty() const {
        return !root;
    }

    template <class T, typename EqualTo, typename Less>
    uint64_t RBTree<T, EqualTo, Less>::size() const {
        return _size;
    }

    template <class T, typename EqualTo, typename Less>
    bool RBTree<T, EqualTo, Less>::operator==(RBTree<T, EqualTo, Less> const& other) const {
        return subtreesEqual(root, other.root);
    }


    template <class T, typename EqualTo, typename Less>
    void RBTree<T, EqualTo, Less>::saveInStream(std::ostream& os, node_ptr root) {
        if (root) {
            root->print(os);
            saveInStream(os, root->left);
            saveInStream(os, root->right);
        } else {
            os << "N";
        }
    }

    template <class T, typename EqualTo, typename Less>
    void RBTree<T, EqualTo, Less>::saveInStream(std::ostream& os) const {
        RBTree<T, EqualTo, Less>::saveInStream(os, root);
    }
    
    template <class T, typename EqualTo, typename Less>
    RBTree<T, EqualTo, Less> RBTree<T, EqualTo, Less>::readFromStream(std::istream& is) {
        RBTree<T, EqualTo, Less> tree;
        auto root = readSubtreeFromStream(is);
        tree.root = root;
        return tree;
    }

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::readSubtreeFromStream(std::istream& is) {
        auto root = std::make_shared<RBTree<T, EqualTo, Less>::Node>(Color::BLACK, nullptr);
        try {
            root->read(is);
        } catch (...) {
            return nullptr;
        }
        root->left = readSubtreeFromStream(is);
        if (root->left) {
            root->left->parent = root;
        }
        root->right = readSubtreeFromStream(is);
        if (root->right) {
            root->right->parent = root;
        }
        return root;
    }

    template <class T, typename EqualTo, typename Less>
    void RBTree<T, EqualTo, Less>::printTree(std::ostream& os, node_ptr root, int indent) {
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

    template <class T, typename EqualTo, typename Less>
    void RBTree<T, EqualTo, Less>::printTree(std::ostream& os) const {
        RBTree<T, EqualTo, Less>::printTree(os, root);
    }

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::findInSubtree(node_ptr root, T const& value) {
        node_ptr result = nullptr;
        if (!root) {
            throw NoSuchElementInSubtree("Error: no such element in subtree");
        } else if (EqualTo::cmp(*root->value, value)) {
            result =  root;
        } else if (Less::cmp(*root->value, value)) {
            result =  findInSubtree(root->right, value);
        } else {
            result =  findInSubtree(root->left, value);
        }
        return result;
    }

    template <class T, typename EqualTo, typename Less>
    bool RBTree<T, EqualTo, Less>::subtreesEqual(node_ptr r1, node_ptr r2) {
        if (static_cast<bool>(r1) != static_cast<bool>(r2)) {
            return false;
        } else {
            return  !(r1 || r2) ||
                    ((*r1 == *r2) && 
                    subtreesEqual(r1->left, r2->left) &&
                    subtreesEqual(r1->right, r2->right));
        }
    }

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::leftRotate(node_ptr node) {
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

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::rightRotate(node_ptr node) {
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
    template <class T, typename EqualTo, typename Less>
    bool RBTree<T, EqualTo, Less>::Node::leftIsTheOne(node_ptr node, T const& value) {
        return (node != nullptr) && (node->left != nullptr) && EqualTo::cmp(*node->left->value, value);
    }

    template <class T, typename EqualTo, typename Less>
    bool RBTree<T, EqualTo, Less>::Node::rightIsTheOne(node_ptr node, T const& value) {
        return (node != nullptr) && (node->right != nullptr) && EqualTo::cmp(*node->right->value == value);
    }

    template <class T, typename EqualTo, typename Less>
    bool RBTree<T, EqualTo, Less>::Node::operator==(Node const& other) const {
        return (this->color == other.color) && EqualTo::cmp(*this->value, *other.value);
    }

    template <class T, typename EqualTo, typename Less>
    bool RBTree<T, EqualTo, Less>::Node::hasNoKids() const {
        return (left == nullptr) && (right == nullptr);
    }

    template <class T, typename EqualTo, typename Less>
    std::ostream& RBTree<T, EqualTo, Less>::Node::print(std::ostream& os) const {
        return os << "(" << *value << ", " << static_cast<int>(color) << ")";
    }

    template <class T, typename EqualTo, typename Less>
    std::istream& RBTree<T, EqualTo, Less>::Node::read(std::istream& is) {
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
    template <class T, typename EqualTo, typename Less>
    void RBTree<T, EqualTo, Less>::AdditionMethodImplementation::run(T const& value) {
        if (tree->empty()) {
            tree->root = makeNode(BLACK, value);
        } else {
            auto node = addToLeafOfSubtree(tree->root, value);
            balanceFrom(node);
        }
        ++tree->_size;
    }

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::AdditionMethodImplementation::findLeafParentInSubtree(node_ptr root, T const& value) {
        node_ptr result = nullptr;
        if (!root) {
            throw TreeEmpty("Error: empty tree");
        } else if (EqualTo::cmp(*root->value, value)) {
            throw NoLeafParentElementInTree("Error: no leaf parent in subtree");
        } else if (Less::cmp(*root->value, value)) {
            if (!root->right) {
                result = root;
            } else {
                result = findLeafParentInSubtree(root->right, value);
            }
        } else {
            if (!root->left) {
                result = root;
            } else {
                result = findLeafParentInSubtree(root->left, value);
            }
        }
        return result;
    }

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::AdditionMethodImplementation::addToLeafOfSubtree(node_ptr root, T const& value) {
        try {
            node_ptr node = findLeafParentInSubtree(root, value);
            return addNodeToLeaf(node, value);
        } catch (TreeEmpty const& e) {
            throw e;
        } catch (...) {
            throw TreeHasGivenElement("Error: tree has element with given value");
        }
    }

    template <class T, typename EqualTo, typename Less>
    void RBTree<T, EqualTo, Less>::AdditionMethodImplementation::balanceFrom(node_ptr node) {
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

    template <class T, typename EqualTo, typename Less>
    bool RBTree<T, EqualTo, Less>::AdditionMethodImplementation::redNode(node_ptr node) {
        return (node) && (node->color == RED);
    }

    template <class T, typename EqualTo, typename Less>
    bool RBTree<T, EqualTo, Less>::AdditionMethodImplementation::redParent(node_ptr node) {
        auto parent = node->parent.lock();
        return (parent) && (parent->color == Color::RED);
    }

    template <class T, typename EqualTo, typename Less>
    bool RBTree<T, EqualTo, Less>::AdditionMethodImplementation::noGrandFatherCase(node_ptr node) {
        auto parent = node->parent.lock();
        if (!parent) return true;
        return !parent->parent.lock();
    } 

    template <class T, typename EqualTo, typename Less>
    bool RBTree<T, EqualTo, Less>::AdditionMethodImplementation::leftChildParentCase(node_ptr node) {
        auto parent = node->parent.lock();
        if (!parent) return false;
        auto grandparent = parent->parent.lock();
        if (!grandparent) return false;
        return grandparent->left == parent;
    }

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::AdditionMethodImplementation::leftChildParentCaseBalance(node_ptr node) {
        if (rightChildNodeCase(node)) {
            node = node->parent.lock();
            node = tree->leftRotate(node)->left;
        }
        auto grandfather = recolorParentAndGrandfather(node);
        return tree->rightRotate(grandfather);
    }

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::AdditionMethodImplementation::rightChildParentCaseBalance(node_ptr node) {
        if (leftChildNodeCase(node)) {
            node = node->parent.lock();
            node = tree->rightRotate(node)->right;
        }
        auto grandfather = recolorParentAndGrandfather(node);
        return tree->leftRotate(grandfather);
    }

    template <class T, typename EqualTo, typename Less>
    bool RBTree<T, EqualTo, Less>::AdditionMethodImplementation::redUncleCase(node_ptr node) {
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

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::AdditionMethodImplementation::recolorParentAndGrandfather(node_ptr node) {
        auto parent = node->parent.lock();
        auto grandfather = parent->parent.lock();
        parent->color = BLACK;
        grandfather->color = RED;
        return grandfather;
    }

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::AdditionMethodImplementation::recolorParentAndUncleAndGrandfather(node_ptr node) {
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

    template <class T, typename EqualTo, typename Less>
    bool RBTree<T, EqualTo, Less>::AdditionMethodImplementation::rightChildNodeCase(node_ptr node) {
        auto parent = node->parent.lock();
        return (parent) && (parent->right == node);
    }

    template <class T, typename EqualTo, typename Less>
    bool RBTree<T, EqualTo, Less>::AdditionMethodImplementation::leftChildNodeCase(node_ptr node) {
        auto parent = node->parent.lock();
        return (parent) && (parent->left == node);
    }

    template <class T, typename EqualTo, typename Less>
    void RBTree<T, EqualTo, Less>::AdditionMethodImplementation::restoreRootProperty() {
        tree->root->color = BLACK;
    }

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::AdditionMethodImplementation::addNodeToLeaf(node_ptr node, T const& value) {
        node_ptr leaf;
        if (EqualTo::cmp(*node->value, value)) {
            throw TreeHasGivenElement("Error: can not add repeating element to leaf!");
        } else if (Less::cmp(*node->value, value)) {
            leaf = addNodeToRightLeaf(node, value);
        } else {
            leaf = addNodeToLeftLeaf(node, value);
        }
        leaf->parent = node;
        return leaf;
    }

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::AdditionMethodImplementation::addNodeToLeftLeaf(node_ptr node, T const& value) {
        if (!node->left) {
            return node->left = makeNode(Color::RED, value); 
        } else {
            throw TreeHasGivenElement("Error: can not add leaf to node!");
        }
    }

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::AdditionMethodImplementation::addNodeToRightLeaf(node_ptr node, T const& value) {
        if (!node->right) {
            return node->right = makeNode(Color::RED, value); 
        } else {
            throw TreeHasGivenElement("Error: can not add leaf to node!");
        }
    }

    template <class T, typename EqualTo, typename Less>
    typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::AdditionMethodImplementation::makeNode(Color color, T const& value) {
        auto v_ptr = std::make_shared<T>(value);
        return std::make_shared<Node>(color, v_ptr);
    }

#endif

// RemovalMethodImplementation class methods implementation
#ifdef RB_TREE_HPP
#define RB_TREE_HPP

template <class T, typename EqualTo, typename Less>
typename RBTree<T, EqualTo, Less>::value_ptr RBTree<T, EqualTo, Less>::RemovalMethodImplementation::run(T const& value) {
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
            auto side = (Less::cmp(*node->value, *parent->value) ? LEFT : RIGHT);
            fixBlackHeight(parent, side);
            tree->root->color = BLACK;
        }
        --tree->_size;
        return value;
    } catch(NoSuchElementInSubtree const& e) {
        throw NoSuchElement(e.what());
    }
}

template <class T, typename EqualTo, typename Less>
typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::RemovalMethodImplementation::removeNode(node_ptr node) {
    if (childlessNodeCase(node)) {
        node = removeChildlessNode(node);
    } else if (nodeWithOneChildCase(node)) {
        node = removeNodeWithOneChild(node);
    } else {
        node = removeNodeWithTwoChildren(node);
    }
    return node;
}

template <class T, typename EqualTo, typename Less>
bool RBTree<T, EqualTo, Less>::RemovalMethodImplementation::childlessNodeCase(node_ptr node) {
    return !(node->left || node->right);
}
template <class T, typename EqualTo, typename Less>
bool RBTree<T, EqualTo, Less>::RemovalMethodImplementation::nodeWithOneChildCase(node_ptr node) {
    return (node->left && !node->right) || (!node->left && node->right);
}

template <class T, typename EqualTo, typename Less>
typename RBTree<T, EqualTo, Less>::node_ptr  RBTree<T, EqualTo, Less>::RemovalMethodImplementation::removeChildlessNode(node_ptr node) {
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

template <class T, typename EqualTo, typename Less>
typename RBTree<T, EqualTo, Less>::node_ptr  RBTree<T, EqualTo, Less>::RemovalMethodImplementation::removeNodeWithOneChild(node_ptr node) {
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

template <class T, typename EqualTo, typename Less>
typename RBTree<T, EqualTo, Less>::node_ptr  RBTree<T, EqualTo, Less>::RemovalMethodImplementation::removeNodeWithTwoChildren(node_ptr node) {
    auto next = findLeastLargestNodeFromNodeWithTwoChildren(node);
    node->value = next->value;
    if (next->right) {
        return removeNodeWithOneChild(next);
    } else {
        return removeChildlessNode(next);
    }
}

template <class T, typename EqualTo, typename Less>
typename RBTree<T, EqualTo, Less>::node_ptr RBTree<T, EqualTo, Less>::RemovalMethodImplementation::findLeastLargestNodeFromNodeWithTwoChildren(node_ptr node) {
    auto next = node->right;
    while (next->left) {
        next = next->left;
    }
    return next;
}


template <class T, typename EqualTo, typename Less>
void RBTree<T, EqualTo, Less>::RemovalMethodImplementation::fixBlackHeight(node_ptr parent, ChildSide problemSide) {
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


template <class T, typename EqualTo, typename Less>
void RBTree<T, EqualTo, Less>::RemovalMethodImplementation::fixBlackHeightForLeft(node_ptr parent) {
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


template <class T, typename EqualTo, typename Less>
void RBTree<T, EqualTo, Less>::RemovalMethodImplementation::fixBlackHeightForRight(node_ptr parent) {
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

template <class T, typename EqualTo, typename Less>
void RBTree<T, EqualTo, Less>::RemovalMethodImplementation::runFixFromGrandFather(node_ptr parent) {
    auto grandfather = parent->parent.lock();
    if (!grandfather) {
        return;
    } else if (parent == grandfather->left) {
        fixBlackHeight(grandfather, LEFT);
    } else {
        fixBlackHeight(grandfather, RIGHT);
    }
}


template <class T, typename EqualTo, typename Less>
bool RBTree<T, EqualTo, Less>::RemovalMethodImplementation::blackChildrenCase(node_ptr node) {
    return (!node->left || node->left->color == BLACK) && 
           (!node->right || node->right->color == BLACK);
}

template <class T, typename EqualTo, typename Less>
bool RBTree<T, EqualTo, Less>::RemovalMethodImplementation::redRightChildCase(node_ptr node) {
    return (node->right && node->right->color == RED);
}

template <class T, typename EqualTo, typename Less>
bool RBTree<T, EqualTo, Less>::RemovalMethodImplementation::redLeftChildCase(node_ptr node) {
    return (node->left && node->left->color == RED);
}


#endif


};

#endif