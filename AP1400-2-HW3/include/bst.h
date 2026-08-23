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

        friend std::ostream& operator<<(std::ostream& os, const Node& node);
    };
    Node*& get_root();
    void bfs(std::function<void(Node*& node)> func);
    size_t length();
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