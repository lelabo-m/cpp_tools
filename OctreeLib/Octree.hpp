#ifndef OCTREELIB_H_
#define OCTREELIB_H_

#include <array>
#include <map>
#include <vector>
#include <cmath>
#include <string>
#include <functional>
#include <algorithm>

namespace Octree
{
    // ##### Template calcul #####
    template <size_t n> inline size_t  LevelNode() { return std::pow(8.0f, n); }
    template <>         inline size_t  LevelNode<0>() { return 0; }
    template <size_t n> inline size_t  LevelTotal() { return (n == 1) ? (LevelNode<1>()) : (LevelNode<n>() + LevelTotal<n - 1>()); }
    template <>         inline size_t  LevelTotal<0>() { return 0; }
    // ##### Types #####
    class Node;
    class IContent;
    typedef std::vector<Node*>              NodeList;
    typedef std::vector<IContent*>          ContentList;
    typedef std::map<size_t, NodeList >     GenrationGrid;
    typedef std::array<double, 3>           Vec3D;
    typedef std::function<bool(Node*)>      NodeFunction;
    // ##### Classes #####
    class IContent
    {
    public:
        virtual bool    InNode(Node*) = 0;
        virtual ~IContent() {};
    };

    template <typename T>
    class Content : public IContent
    {
    typedef std::function<bool(Node*, T)>   NodeTest;
    public:
        Content(T value, NodeTest func) : _value(value), _test(func) {}
        bool        InNode(Node* n)     { return _test(n, _value); }
        T&          Value()             { return _value; }
    private:
        T           _value;
        NodeTest    _test;
    };

    class Tree
    {
    public:
        static std::string  to_string(Tree *);
    public:
        Tree(const Vec3D& pos, const Vec3D& size, NodeFunction func, size_t depth = 0);
        void    Destroy();
        ~Tree();
        template <typename T>
        void    AddContents(T& list)
        {
            _contents.insert(_contents.end(), list.begin(), list.end());
        }

        void            Process();
        const Vec3D&    Size() const { return _size; }
        const Vec3D&    Center() const { return _center; }
        bool            IsTerminal(Node *n) const;
        void            Register(Node* n);
        NodeList        Grid(size_t);
        NodeList        LastGrid();
    private:
        Vec3D           _size;
        Vec3D           _center;
        NodeFunction    _terminal;
        Node*           _root;
        ContentList     _contents;
        GenrationGrid   _generations;
    };

    class Node
    {
    public:
        static std::string  to_string(Node *);
    public:
        Node(Tree* t);
        Node(Node *parent);
        ~Node() {}
        void            GenChild();
        void            Expand();
        const Vec3D&    Size() const { return _size; }
        const Vec3D&    Center() const { return _center; }
        bool            IsRoot() const  { return _root; }
        size_t          Depth() const   { return _depth; }
        ContentList&    Content()       { return _contents; }
        bool            IsEmpty() const { return !_contents.size(); }
    private:
        Tree*                   _tree;
        Node*                   _parent;
        bool                    _root;
        size_t                  _depth;
        NodeList                _childs;
        Vec3D                   _size;
        Vec3D                   _center;
        ContentList             _contents;
    };

    // ##### Tools #####
    inline std::string  to_string(const Vec3D& v)
    {
        return "[" + std::to_string(v[0])
                + " / " + std::to_string(v[1])
                + " / " + std::to_string(v[2]) + "]";
    }

    inline Vec3D    CenterFromBoundingBox(double Xmin, double Xmax, double Ymin, double Ymax, double Zmin, double Zmax)
    {
        return Vec3D{{ Xmin + (Xmax - Xmin) / 2, Ymin + (Ymax - Ymin) / 2, Zmin + (Zmax - Zmin) / 2}};
    }

    inline Vec3D    SizeFromBoundingBox(double Xmin, double Xmax, double Ymin, double Ymax, double Zmin, double Zmax)
    {
        return Vec3D{{ (Xmax - Xmin) / 2, (Ymax - Ymin) / 2, (Zmax - Zmin) / 2}};
    }

    inline NodeFunction     Terminal(size_t level)
    {
        return [level](Node *n){ return level == n->Depth();};
    }
    // ##### Math #####
    static std::array<Vec3D, 8> matrix{
            Octree::Vec3D{-1.0f, -1.0f, -1.0f}, Octree::Vec3D{+1.0f, -1.0f, -1.0f},
            Octree::Vec3D{-1.0f, +1.0f, -1.0f}, Octree::Vec3D{+1.0f, +1.0f, -1.0f},
            Octree::Vec3D{-1.0f, -1.0f, +1.0f}, Octree::Vec3D{+1.0f, -1.0f, +1.0f},
            Octree::Vec3D{-1.0f, +1.0f, +1.0f}, Octree::Vec3D{+1.0f, +1.0f, +1.0f} };

    inline  Vec3D   Div(const Vec3D& ar, size_t n)
    {
        return Vec3D{{ ar[0] / n, ar[1] / n, ar[2] / n}};
    }

    inline  Vec3D   Mult(const Vec3D& a, const Vec3D& b)
    {
        return Vec3D{{ a[0] * b[0], a[1] * b[1], a[2] * b[2] }};
    }

    inline  Vec3D   Add(const Vec3D& a, const Vec3D& b)
    {
        return Vec3D{{ a[0] + b[0], a[1] + b[1], a[2] + b[2]}};
    }
}

#endif /* !OCTREELIB_H_ */