

template<typename T>
class Node {
    public:
    T data;
    Node* left;
    Node* right;

    Node(T value) : data(value), left(nullptr), right(nullptr) {}

    bool operator>(const Node<T> &node) {
        return data > node.data;
    }
    bool operator<=(const Node<T> &node) {
        return data <= node.data;
    }

};

template<typename T>
class BinaryTree {
    private: 
    Node<T>* root;

    public:
    void insertNote(T value) {
        // newNode becomes the root if the root is null
        if(root == nullptr) {
            root.data = value;
            return;
        }
        // get the next node in the tree
        Node<T>* next_node = (root.data > value) ? root.left : root.right;

        // traverses the tree until we find a null spot
        while(next_node != nullptr) {
            next_node = (new_node.data > value) ? root.left : root.right;
        }

        






    }
}