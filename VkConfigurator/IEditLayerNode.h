#pragma once
#include <string>
#include <vector>
#include <qvariant.h>
#include <typeindex>

class IEditLayerNode
{
protected:
    std::string m_name;
    std::vector<IEditLayerNode*> m_children;
    IEditLayerNode* m_parent = nullptr;

public:
    IEditLayerNode(const std::string_view name, IEditLayerNode* parent = nullptr) : m_name{ name }, m_parent{ parent }
    {
        //
    }
    virtual bool isContainer()const = 0;
    virtual QVariant checkRole()const = 0;
    virtual QVariant displayRole()const = 0;
    virtual void setCheckState(const Qt::CheckState state) = 0;
    virtual void setValue(const QVariant& value) = 0;
    virtual QVariant getValue()const = 0;
    virtual void append() {};
    virtual void remove() {};
    virtual std::type_index type()const = 0;
    QString name()const { return QString::fromStdString(m_name); }

    constexpr size_t childCount()const { return m_children.size(); }
    using iterator = std::vector<IEditLayerNode*>::iterator;
    constexpr iterator begin() { return m_children.begin(); }
    constexpr iterator end() { return m_children.end(); }
    using const_iterator = std::vector<IEditLayerNode*>::const_iterator;
    constexpr const_iterator cbegin()const { return m_children.cbegin(); }
    constexpr const_iterator cend()const { return m_children.cend(); }

    IEditLayerNode* parent()const { return m_parent; }
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

