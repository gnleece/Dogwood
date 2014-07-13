#include "HierarchyItem.h"

#include <algorithm>

HierarchyItem::HierarchyItem(const string& v, HierarchyItem *parent)
{
    parentItem = parent;
    data = v;
}

HierarchyItem::~HierarchyItem()
{
    // TODO delete children
}

HierarchyItem *HierarchyItem::child(int index)
{
    return childItems[index];
}

HierarchyItem *HierarchyItem::parent()
{
    return parentItem;
}

int HierarchyItem::childNumber() const
{
    if (parentItem)
    {
        std::vector<HierarchyItem*>::iterator iter = std::find(parentItem->childItems.begin(), parentItem->childItems.end(), const_cast<HierarchyItem*>(this));
        return std::distance(parentItem->childItems.begin(), iter);
    }

    return 0;
}

string HierarchyItem::getData() const
{
    return data;
}

bool HierarchyItem::setData(const string& v)
{
    data = v;
    return true;
}

int HierarchyItem::childCount() const
{
    return childItems.size();
}

bool HierarchyItem::insertChildren(int position, int count)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int index = 0; index < count; index++)
    {
        HierarchyItem *item = new HierarchyItem("new child item string", this);
        childItems.insert(childItems.begin()+position, item);
    }

    return true;
}

bool HierarchyItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    childItems.erase(childItems.begin() + position, childItems.begin() + position + count);
    return true;
}