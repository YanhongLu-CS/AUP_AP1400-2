#ifndef BST_H
#define BST_H

#include <cstddef>
#include <functional>
#include <ostream>

class BST
{
public:
    class Node
    {
    public:
	    Node(int value, Node* left, Node* right);
	    Node();
	    Node(const Node& node);

	    int value;
	    Node* left;
	    Node* right;

        bool operator>(int other) const;
        // if we input:
        // BST::Node node{5, nullptr, nullptr};
        // node > 4;
        // then the compiler will understand it as:
        // node.operator>(4);
        // and then do:
        // bool BST::Node::operator>(int other) const
        // {
        //     return value > other;
        // }
        bool operator>=(int other) const;
        bool operator<(int other) const;
        bool operator<=(int other) const;
        bool operator==(int other) const;
        friend bool operator<(int left, const BST::Node& node);
        // friend 不会让函数变成成员函数。
        // friend 只授予访问权限，不会自动实现函数。
        // friend 声明必须放在被访问的那个类里面。

        friend bool operator<=(int left, const BST::Node& node);
        friend bool operator>(int left, const BST::Node& node);
        friend bool operator>=(int left, const BST::Node& node);
        friend bool operator==(int left, const BST::Node& node);


        friend std::ostream& operator<<(std::ostream& os, const Node& node);
        //std::out << ... == operator<<(std::cout, ...)
    };
    Node*& get_root();
    void bfs(std::function<void(Node*& node)> func) const;
    size_t length() const;
    bool add_node(int value);
    Node** find_node(int value);
    Node** find_parrent(int value);
    Node** find_successor(int value);
    bool delete_node(int value);

    friend std::ostream& operator<<(std::ostream& os, const BST& bst);
    //friend can visit private data of the class

private:
    Node* root;
};

#endif //BST_H