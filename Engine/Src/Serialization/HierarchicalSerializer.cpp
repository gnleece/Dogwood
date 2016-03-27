#include "Serialization/HierarchicalSerializer.h"

void HierarchicalSerializer::Save(string filepath)
{
    m_document.SaveFile(filepath.c_str());
}

void HierarchicalSerializer::Load(string filepath)
{

}