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
//Node a;
//Node b;
// Node* p = &a;
// Node*& ref = p;
// 此时：
// p = ref ------> a
// ref 是 p 的别名
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

// BST::Node** BST::find_successor(int value)
// {
//     if (this->root == nullptr) return nullptr;

//     Node** current = &this->root;
//     Node** candidate = nullptr;

//     while (*current != nullptr) {

//         if (value < (*current)->value) {
//             // 当前节点比 value 大，
//             // 它可能是 successor，先记下来
//             candidate = current;

//             // 但左边可能还有一个更小、同时仍然 > value 的节点
//             current = &((*current)->left);
//         }
//         else if (value > (*current)->value) {
//             // 当前节点太小，不可能是 successor
//             current = &((*current)->right);
//         }
//         else {
//             // 找到了 value 对应的节点

//             // Case 1:
//             // 有右子树，则 successor 是右子树中的最小节点
//             if ((*current)->right != nullptr) {

//                 current = &((*current)->right);

//                 while ((*current)->left != nullptr) {
//                     current = &((*current)->left);
//                 }

//                 return current;
//                 // 1. this guys is bigger than all the nodes under the target node,
//                 // 2. this guys is smaller than all the candidates , as the target value is smaller 
//                 // than all the candidate and locate in the left path of all the candidate
//                 // so if this guys is bigger than a perticular candidate, it will go to the 
//                 // right path.
//             }

//             // Case 2:
//             // 没有右子树，则 successor 是搜索过程中记录的祖先
//             return candidate;
//         }
//     }

//     return nullptr;
// }
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
    if (target->left == nullptr && target->right == nullptr) {
        *target_link = nullptr;
         delete target;
    }
    else if (target->left == nullptr) {
        *target_link = target->right;
        /*
        For example: 
        8
       / \
      4   12
            \
            15
        if we want to delete 12,
        as [8].right ---> [12], we need to let [8].right point to another node.
        but node* target can only tell us where does [12] locate in, and we can not modify that as we do not know where it locates.
        instead, node** target ---> [8].right, which allow us to modify [8].right by using *target.

        */
        delete target;
    }
    else if (target->right == nullptr) {
        *target_link = target->left;
        delete target; // we do not need to store the location of [12].
    }
    else {
        Node** successor_link = find_successor(value);
        Node* successor = *successor_link;
        *successor_link = successor->right;
        *target_link = successor;
        successor->left = target->left;
        successor->right = target->right;
        delete target;
    }
    return true;
}


BST::BST(std::initializer_list<int> values)
    : root(nullptr)
{
    for (int value : values) {
        add_node(value);
    }
}

BST& BST::operator++() {
    bfs([&](Node*& node){node->value++;});
    return *this;
}
// operator++()
// 表示：
// 这是 ++ 运算符重载，而且因为括号里没有那个占位 int，所以它对应的是前置 ++bst。

BST BST::operator++(int) {
    BST old = BST(*this);
    ++(*this);
    return old;
}
//表示：
//bst++，本体加1，返回旧值

BST::BST(const BST& other)
    : root(nullptr)
{
    other.bfs([this](Node*& node){this->add_node(node->value);});
}

BST::BST(BST&& other) noexcept
    : root(other.root)
{
    other.root = nullptr;
}

BST::~BST()
{
    std::vector<Node*> nodes;
    bfs([&nodes](BST::Node*& node){nodes.push_back(node);});
    for(auto& node: nodes)
    delete node;
}


BST& BST::operator=(const BST& other) {
    if (this == &other) {
        return *this;
    } else {
        BST Temp(other);
        std::swap(this->root, Temp.root);
        return *this;
    }
}

BST& BST::operator=(BST&& other) noexcept
{
    if (this == &other)
        return *this;

    BST temp(std::move(other));
    std::swap(this->root, temp.root);

    return *this;
}