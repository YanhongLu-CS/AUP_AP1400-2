#include "bst.h"

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

std::ostream& operator<<(std::ostream& os, const BST::Node& node)
{
    os << 
}