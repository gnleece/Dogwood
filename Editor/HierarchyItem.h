#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

class HierarchyItem
{
public:
    HierarchyItem(const string &str, HierarchyItem *parent = 0);
    ~HierarchyItem();

    HierarchyItem*   child(int index);
    HierarchyItem*   parent();
    int              childNumber() const;

    string      getData() const;
    bool        setData(const string& v);
    
    int         childCount() const;
    bool        insertChildren(int position, int count);
    bool        removeChildren(int position, int count);

private:
    vector<HierarchyItem*>  childItems;
    string                  data;
    HierarchyItem*          parentItem;
};