#pragma once
#include <string>
#include <vector>
#include <qvariant.h>
#include <typeindex>
#include "valueEditionLayer.h"

class IEditNode
{
protected:
    std::string m_name;
    std::vector<IEditNode*> m_children;
    IEditNode* m_parent = nullptr;

public:
    IEditNode(const std::string_view name, IEditNode* parent = nullptr) : m_name{ name }, m_parent{ parent }
    {
        //
    }
    ~IEditNode()
    {
        for (auto pNode : m_children)
            delete pNode;
    }
    
    virtual QVariant checkRole()const { return QVariant(); }
    virtual QVariant displayRole()const
    {
        return QString::fromStdString(m_name);
    }
    virtual QVariant getValue()const { return QVariant(); }

    virtual bool isBoolean() const { return false; }
    virtual bool isContainer()const = 0;
    virtual bool isStructure() const = 0;
    virtual bool isEditable() const { return !isStructure() && !isContainer(); }

    virtual void setCheckState(const Qt::CheckState state){}
    virtual void setValue(const QVariant& value) {}
    virtual void append() {};
    virtual void remove() {};
    virtual std::type_index type()const = 0;
    QString name()const { return QString::fromStdString(m_name); }

    constexpr size_t childCount()const { return m_children.size(); }
    IEditNode* const child(const int index) { return m_children[index]; }
    using iterator = std::vector<IEditNode*>::iterator;
    constexpr iterator begin() { return m_children.begin(); }
    constexpr iterator end() { return m_children.end(); }
    using const_iterator = std::vector<IEditNode*>::const_iterator;
    constexpr const_iterator cbegin()const { return m_children.cbegin(); }
    constexpr const_iterator cend()const { return m_children.cend(); }

    IEditNode* parent()const { return m_parent; }
    int indexInparent()
    {
        if (m_parent)
        {
            auto iter = std::ranges::find(m_parent->m_children, this);
            return iter - m_parent->m_children.begin();
        }
        return -1;
    }
};

