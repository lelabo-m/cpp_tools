#include "Octree.hpp"

Octree::Tree::Tree(const Vec3D& pos, const Vec3D& size, NodeFunction func, size_t depth) : _size(size), _center(pos), _terminal(func)
{
    _root = new Node(this);
    Register(_root);
    _root->GenChild();
    for (size_t d = 1; d < depth; ++d)
    {
        for (auto node : _generations[d])
            node->GenChild();
    }
    if (depth > 1)
    {
        for (auto it = _generations.begin(); it != _generations.end(); ++it)
            if (it->first != depth)
                _generations.erase(it);
    }
}

void    Octree::Tree::Destroy()
{
    for (auto gen : _generations)
    {
        for (auto node : gen.second)
            delete node;
    }
    _generations.clear();
}

Octree::Tree::~Tree()
{
    Destroy();
    _generations.clear();
}

void            Octree::Tree::Process()
{
    size_t i = 0;
    for (auto gen : _generations)
        i = std::max(i, gen.first);
    for (auto node : _generations[i])
    {
        for (auto content : _contents)
            if (content->InNode(node))
                node->Content().push_back(content);
        node->Expand();
    }
}

bool            Octree::Tree::IsTerminal(Node *n) const
{
    return _terminal(n);
}

void            Octree::Tree::Register(Node* n)
{
    _generations[n->Depth()].push_back(n);
}

Octree::NodeList   Octree::Tree::Grid(size_t level)
{
    if (_generations.find(level) != _generations.end())
        return _generations[level];
    else if (_generations.size())
    {
        size_t i = 0;
        for (auto gen : _generations)
            i = std::max(i, gen.first);
        return _generations[i];
    }
}

Octree::NodeList   Octree::Tree::LastGrid()
{
    size_t i = 0;
    for (auto gen : _generations)
        i = std::max(i, gen.first);
    return _generations[i];
}

std::string     Octree::Tree::to_string(Tree *t)
{
    std::string repr;
    for (auto gen : t->_generations)
    {
        repr += "[Depth = " + std::to_string(gen.first) + "]\n";
        for (auto node : gen.second)
            if (!node->IsEmpty())
                repr += " |-> " + Octree::Node::to_string(node) + "\n";
    }
    return repr;
}