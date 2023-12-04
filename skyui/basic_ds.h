#ifndef BASIC_DS_H
#define BASIC_DS_H

#include <cstddef>
#include <vector>
#include <string>
#include <bits/shared_ptr.h>
#include "utils.h"

namespace jlib {

    class Size {
    public:
        Size() = default;

        Size(const Size &) = default;

        Size(size_t w, size_t h) : width(w), height(h) {}

        Size &operator=(const Size &) = default;

        bool operator==(const Size &sz) const { return sz.width == width && sz.height == height; }

        bool operator!=(const Size &sz) const { return sz.width != width || sz.height != height; }

        size_t width, height;
    };

    class Point {
    public:
        Point() = default;

        Point(const Point &) = default;

        Point &operator=(const Point &) = default;

        int x, y;
    };

    template<typename T>
    class TreeNode {
    public:
        using AutoTreeNode = std::shared_ptr<TreeNode<T>>;

        explicit TreeNode(T data) : id_(generate_uuid()), data_(data), parent_(nullptr), children_() {}

        TreeNode(T data, std::string id) : id_(std::move(id)), data_(data), parent_(nullptr), children_() {}

        TreeNode(const TreeNode &) = delete;

        ~TreeNode() = default;

        T data() { return data_; }

        AutoTreeNode parent() { return parent_; }

        std::vector<AutoTreeNode> &children() { return children_; }

        static AutoTreeNode build(T data, std::string id)
        { return std::make_shared<TreeNode<T>>(data, id.empty() ? generate_uuid() : std::move(id)); }

        static AutoTreeNode find(const AutoTreeNode &root, const std::string &id) {
            if (root.id_ == id) return root;
            AutoTreeNode p;
            for (const auto &child: root.children_) {
                p = find(id);
                if (p != nullptr)
                    return p;
            }
            return nullptr;
        }

        static void addChild(const AutoTreeNode &root, const std::string &id, const AutoTreeNode &child) {
            auto p = find(root, id);
            if (p != nullptr) {
                child->parent_ = p;
                p->children.emplace_back(child);
            }
        }

        static void setParent(const AutoTreeNode &node, const AutoTreeNode &parent) {
            node->parent_ = parent;
            parent->children_.emplace_back(node);
        }

    private:
        T data_;
        std::string id_;
        std::shared_ptr<TreeNode> parent_;
        std::vector<AutoTreeNode> children_;
    };

    template<typename T>
    class Tree {
    public:

    private:

    };

} // jlib

#endif //BASIC_DS_H
