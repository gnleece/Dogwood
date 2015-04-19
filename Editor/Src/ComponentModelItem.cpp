#include "ComponentModelItem.h"

ComponentModelItem* ComponentModelItem::GetParent()
{
    return m_parent;
}

void ComponentModelItem::SetParent(ComponentModelItem* parent, int position)
{
    // if this object had a parent already, remove it from that parent's list of children
    if (m_parent)
    {
        m_parent->RemoveChild(this);
    }

    m_parent = parent;

    // add this object to the new parent's list of children
    if (m_parent)
    {
        m_parent->AddChild(this, position);
    }
}

ComponentModelItem* ComponentModelItem::GetChild(int index)
{
    if (index >= m_children.size())
        return NULL;

    return m_children[index];
}

int ComponentModelItem::GetChildNumber()
{
    // determine the index of this game object in the parent's list of children
    if (m_parent)
    {
        std::vector<ComponentModelItem*>::iterator iter = std::find(m_parent->m_children.begin(), m_parent->m_children.end(), const_cast<GameObject*>(this));
        return std::distance(m_parent->m_children.begin(), iter);
    }

    return 0;
}

int ComponentModelItem::GetChildCount()
{
    return m_children.size();
}

bool ComponentModelItem::InsertChildren(int position, int count)
{
    if (position < 0)
        return false;

    for (int i = 0; i < count; i++)
    {
        ComponentModelItem* child = new ComponentModelItem();
        child->SetParent(this, position);
        //child->SetName("GameObject");
    }

    return true;
}

bool ComponentModelItem::RemoveChildren(int position, int count)
{
    if (position < 0 || position + count > m_children.size())
        return false;

    m_children.erase(m_children.begin() + position, m_children.begin() + position + count);

    return true;
}

void ComponentModelItem::AddChild(ComponentModelItem* child, int position)
{
    if (position >= 0 && position < m_children.size())
    {
        m_children.insert(m_children.begin() + position, child);
    }
    else
    {
        m_children.push_back(child);
    }
}

void ComponentModelItem::RemoveChild(ComponentModelItem* child)
{
    // TODO fixme
    //m_children.remove(child);
}