#include <iostream>
#include <vector>
#include <functional>
#include "Octree.hpp"

std::string to_string(std::vector<int> v)
{
    std::string m = "[";
    for (auto i : v)
    {
        m += std::to_string(i);
        m += "/";
    }
    return m + "]";
}

int main()
{
    std::cout << "BEGIN TEST OCTREE" << std::endl;
    std::cout << "LEVEL 0 = " << Octree::LevelNode<0>() << std::endl;
    std::cout << "LEVEL 1 = " << Octree::LevelNode<1>() << std::endl;
    std::cout << "LEVEL 2 = " << Octree::LevelNode<2>() << std::endl;
    std::cout << "LEVEL 3 = " << Octree::LevelNode<3>() << std::endl;
    std::cout << "LEVEL TOTAL 0 = " << Octree::LevelTotal<0>() << std::endl;
    std::cout << "LEVEL TOTAL 1 = " << Octree::LevelTotal<1>() << std::endl;
    std::cout << "LEVEL TOTAL 2 = " << Octree::LevelTotal<2>() << std::endl;
    std::cout << "LEVEL TOTAL 3 = " << Octree::LevelTotal<3>() << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    Octree::Vec3D   size = Octree::SizeFromBoundingBox(0, 10, 0, 10, 0, 10);
    Octree::Vec3D   center = Octree::CenterFromBoundingBox(0, 10, 0, 10, 0, 10);
    Octree::Tree    tree = Octree::Tree(center, size, Octree::Terminal(2));
    std::function<bool(Octree::Node*, Octree::Vec3D)>   f = [](Octree::Node* n, Octree::Vec3D a)
    {
        if (a[0] > n->Center()[0] - n->Size()[0] &&
        a[0] <= n->Center()[0] + n->Size()[0] &&
        a[1] > n->Center()[1] - n->Size()[1] &&
        a[1] <= n->Center()[1] + n->Size()[1] &&
        a[2] > n->Center()[2] - n->Size()[2] &&
        a[2] <= n->Center()[2] + n->Size()[2])
            return true;
        return false;
    };
    Octree::Content<Octree::Vec3D>  p1({{ 1.0, 1.0, 1.0}}, f);
    Octree::Content<Octree::Vec3D>  p21({{ 8.0, 1.0, 1.0}}, f);
    Octree::Content<Octree::Vec3D>  p22({{ 6.0, 3.0, 3.0}}, f);
    Octree::Content<Octree::Vec3D>  p3({{ 3.0, 5.5, 1.0}}, f);
    Octree::Content<Octree::Vec3D>  p4({{ 6.0, 5.7, 1.0}}, f);
    std::vector<Octree::Content<Octree::Vec3D>*> list;
    list.push_back(&p1);
    list.push_back(&p21);
    list.push_back(&p22);
    list.push_back(&p3);
    list.push_back(&p4);
    tree.AddContents(list);
    tree.Process();
    std::cout << Octree::Tree::to_string(&tree) << std::endl;
    return 0;
}