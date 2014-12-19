#include "Octree.hpp"

Octree::Node::Node(Tree* t) : _tree(t), _parent(nullptr), _root(true), _depth(0)
{
    _center = t->Center();
    _size = t->Size();
}

Octree::Node::Node(Node *parent) : _tree(parent->_tree), _parent(parent), _root(false), _depth(parent->_depth + 1)
{
    _size = Div(parent->_size, 2);
    _center = Add(parent->_center, Mult(matrix[parent->_childs.size()], _size));
}

void            Octree::Node::GenChild()
{
    for (size_t i = 0; i < 8; ++i)
    {
        Node* child = new Node(this);
        _childs.push_back(child);
        _tree->Register(child);
    }
}

void            Octree::Node::Expand()
{
    if (!_childs.size())
        this->GenChild();
    if (!_tree->IsTerminal(this) && !this->IsEmpty())
    {
        for (auto child : _childs)
        {
            for (auto content : _contents)
                if (content->InNode(child))
                    child->Content().push_back(content);
            child->Expand();
        }
    }
}

std::string     Octree::Node::to_string(Node *n)
{
    return "[ Size = " + Octree::to_string(n->Size())
            + " Center = " + Octree::to_string(n->Center()) + " Content = " + std::to_string(n->_contents.size())+ " ]";
}