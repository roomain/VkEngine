#pragma once
#include "IEditNode.h"

template<typename Type>
class ITEditNode : public IEditNode
{
protected:
    template<typename MemberType>
    void resetChild(MemberType& data, size_t& index)
    {
        auto pChild = dynamic_cast<ITEditNode<MemberType>*>(m_children[index]);
        ++index;
        pChild->reset(data);
    }

public:
    ITEditNode() = delete;
    explicit ITEditNode(const std::string_view name, IEditNode* parent = nullptr) : IEditNode{ name, parent }{}

    virtual void reset(Type& data) = 0;

    constexpr bool isContainer()const override
    {
        return is_std_vector_v<Type> || is_std_list_v<Type>;
    }

    constexpr bool isBoolean() const
    {
        return std::is_same_v<Type, bool>;
    }

    bool isStructure() const override
    {
        return is_reflective_v<Type>;
    }

    
    std::type_index type()const override
    {
        return std::type_index{ typeid(Type) };
    }
};