#ifndef RB_TREE_EXCEPTIONS_HPP
#define RB_TREE_EXCEPTIONS_HPP

#include <stdexcept>

namespace cust {
    class NoSuchElement : public std::runtime_error {
    public:
        NoSuchElement(std::string const& message) : std::runtime_error(message) {}

    };

    class NoSuchElementInSubtree : public std::runtime_error {
    public:
        NoSuchElementInSubtree(std::string const& message) : std::runtime_error(message) {}

    };

    class TreeEmpty : public std::runtime_error {
    public:
        TreeEmpty(std::string const& message) : std::runtime_error(message) {}

    };

    class SubtreeEmpty : public std::runtime_error {
    public:
        SubtreeEmpty(std::string const& message) : std::runtime_error(message) {}

    };

    class NoPrevElementInTree : public std::runtime_error {
    public:
        NoPrevElementInTree(std::string const& message) : std::runtime_error(message) {}

    };

    class TreeHasGivenElement : public std::runtime_error {
    public:
        TreeHasGivenElement(std::string const& message) : std::runtime_error(message) {}

    };

    class CanNotAddLeaf : public std::runtime_error {
    public:
        CanNotAddLeaf(std::string const& message) : std::runtime_error(message) {}

    };
        
    class NoLeafParentElementInTree : public std::runtime_error {
    public:
        NoLeafParentElementInTree(std::string const& message) : std::runtime_error(message) {}

    };
};

#endif