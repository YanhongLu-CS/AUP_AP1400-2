#include "bst.h"
#include <functional>
#include <queue>
#include <cstddef>

BST::Node::Node(int value, BST::Node* left, BST::Node* right)
    : value(value), left(left), right(right)
{

}

BST::Node::Node()
    : value(0), left(nullptr), right(nullptr)
{

}

BST::Node::Node(const Node& node)
    : value(node.value), left(node.left), right(node.right)
{

}



bool BST::Node::operator>(int other) const
{
    return value > other;
}

bool BST::Node::operator>=(int other) const
{
    return value >= other;
}

bool BST::Node::operator<(int other) const
{
    return value < other;
}

bool BST::Node::operator<=(int other) const
{
    return value <= other;
}

bool BST::Node::operator==(int other) const
{
    return value == other;
}

bool operator<(int left, const BST::Node& right)
{
    return left < right.value;
}

bool operator<=(int left, const BST::Node& right)
{
    return left <= right.value;
}

bool operator>(int left, const BST::Node& right)
{
    return left > right.value;
}

bool operator>=(int left, const BST::Node& right)
{
    return left >= right.value;
}

bool operator==(int left, const BST::Node& right)
{
    return left == right.value;
}

BST::Node*& BST::get_root()
{
    return this->root;
}

bool BST::add_node(int value)
{
    if(this->root == nullptr) {
        this->root = new Node(value, nullptr, nullptr);
        return true;
    }
    Node* current = this->root;
    while (true) {
        if (value == current->value) {
            return false;
        }
        else if (value < current->value) {
            if (current->left == nullptr) {
                current->left = new Node(value, nullptr, nullptr);
                return true;
            } else {
                current = current->left;
            }
        }
        else {
            if (current->right == nullptr) {
                current->right = new Node(value, nullptr, nullptr);
                return true;
            } else {
                current = current->right;
            }
        }
    }
}

void BST::bfs(std::function<void(Node*& node)> func) const
{
    if (this->root == nullptr) return;
    std::queue<Node*> nodes;
    nodes.push(this->root);
    while(!nodes.empty()) {
        Node* current = nodes.front();
        nodes.pop();
        func(current);
        if (current->left != nullptr) {
            nodes.push(current->left);
        }
        if (current->right != nullptr) {
            nodes.push(current->right);
        }
    }

}

size_t BST::length() const
{
    size_t count = 0;
    bfs([&count](Node*& node){count++;});
    return count;
}

std::ostream& operator<<(std::ostream& os, const BST::Node& node)
{
    os << &node;
    os << "=> value:";
    os << node.value;
    os << " left:";
    os << node.left;
    os << " right:";
    os << node.right;

    return os;
}

std::ostream& operator<<(std::ostream& os, const BST& bst)
{
    os << "********************************************************************************";
    bst.bfs([&os](BST::Node*& node){os << *node << '\n';});
    os << "binary search tree size: " << bst.length() << '\n';
    os << "********************************************************************************";
    return os;

}

BST::Node** BST::find_node(int value)
{
    if (this->root == nullptr) {
        return nullptr;
    }
    Node** current = &this->root;
    while(*current != nullptr) {
        if((*current)->value == value) {
            return current;
        } else if ((*current)->value < value) {
            current = &(*current)->right;
        } else {
            current = &(*current)->left;
        }
    }
    return nullptr;
}

BST::Node** BST::find_parrent(int value)
{
    if (this->root == nullptr) {
        return nullptr;
    }
    Node** current = &this->root;
    Node** prevent = nullptr;
    if ((*current)->value == value) return nullptr;
    else if ((*current)->value < value) {
        prevent = current;
        current = &(*current)->right;
    }
    else {
        prevent = current;
        current = &(*current)->left;
    }
    while(*current != nullptr) {
        if((*current)->value == value) {
            return prevent;
        } else if ((*current)->value < value) {
            prevent = current;
            current = &(*current)->right;
        } else {
            prevent = current;
            current = &(*current)->left;
        }
    }
    return nullptr;
}

BST::Node** BST::find_successor(int value)
{
    if (this->root == nullptr) return nullptr;
    Node** current = &this->root;
    Node** candidate = nullptr;
    while(*current != nullptr) {
        if (value > (*current)->value) {
            candidate = current;
            current = &(*current)->right;
        } else if(value < (*current)->value) {
            current = &(*current)->left;
        } else {
            if ((*current)->left != nullptr) {
                current = &(*current)->left;
                while ((*current)->right != nullptr) {
                    current = &(*current)->right;
                }
                return current;
            } else {
                return candidate;
            }
        }
    }
    return nullptr;
}

bool BST::delete_node(int value)
{
    Node** target_link = find_node(value);
    if (target_link == nullptr) return false;
    Node* target = *target_link;
    if (target->left == nullptr) {
        *target_link = target->right;
        delete target;
    }
    if (target->right == nullptr) target_link = &target->left;

}
