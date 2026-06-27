#pragma once
#include "ITEditNode.h"

template<typename Type>
class TEditNode : public ITEditNode<Type>
{
private:
    std::reference_wrapper<Type> m_ref;

    template<typename ParentType>
    void genChildrenFromParentMembers(ParentType& parent)
    {
        if constexpr (has_base<ParentType>::value)
        {
            // add parent members
            genChildrenFromParentMembers<ParentType::Base>(parent);
        }

        std::apply(
            [this, &parent](auto&... tupleArgs)
            {
                (appendChild(IEditNode::m_name, tupleArgs.first, parent.*(tupleArgs.second)), ...);
            },
            ParentType::s_reflectiveCtx);
    }

    template<typename MemberType>
    void appendChild(const std::string& baseName, std::string_view memberName, MemberType& member)
    {
        auto pChild = new TEditNode<MemberType>(baseName + "." + std::string(memberName), member, this);
        IEditNode::m_children.push_back(pChild);
    }

    template<typename MemberType>
    void appendChild(const std::string& memberName, MemberType& member)
    {
        auto pChild = new TEditNode<MemberType>(memberName, member, this);
        IEditNode::m_children.push_back(pChild);
    }

    void genChildrenFromMembers()
    {
        std::apply(
            [this](auto&... tupleArgs)
            {
                (appendChild(IEditNode::m_name, tupleArgs.first, m_ref.get().*(tupleArgs.second)), ...);
            },
            Type::s_reflectiveCtx);
    }

    //------------------------------------------------------------------------------------------------------------
    template<typename ParentType>
    void resetChildrenFromParentMembers(ParentType& parent)
    {
        size_t index = 0;
        if constexpr (has_base<ParentType>::value)
        {
            // add parent members
            resetChildrenFromParentMembers<ParentType::Base>(parent);
        }

        std::apply(
            [this, &parent, &index](auto&... tupleArgs)
            {
                (ITEditNode<Type>::resetChild(m_ref.get().*(tupleArgs.second), index), ...);
            },
            ParentType::s_reflectiveCtx);
    }

    void resetChildrenFromMembers()
    {
        size_t index = 0;
        std::apply(
            [this, &index](auto&... tupleArgs)
            {
                (ITEditNode<Type>::resetChild(m_ref.get().*(tupleArgs.second), index), ...);
            },
            Type::s_reflectiveCtx);
    }

    

    void resetSelfContainer()
    {
        if constexpr (is_std_array_v<Type> || is_std_vector_v<Type> || is_std_list_v<Type>)
        {
            if (m_ref.get().size() < IEditNode::m_children.size())
            {
                size_t index = 0;
                for (auto& data : m_ref.get())
                    ITEditNode<Type>::resetChild(data, index);
                
                IEditNode::m_children.erase(IEditNode::m_children.begin() + index, IEditNode::m_children.end());
            }
            else
            {
                bool addNewChild = IEditNode::m_children.empty();
                size_t index = 0;
                for (auto& data : m_ref.get())
                {
                    if (!addNewChild)
                    {
                        ITEditNode<Type>::resetChild(data, index);
                        addNewChild = index >= IEditNode::m_children.size();
                    }
                    else
                    {
                        appendChild<Type::value_type>(std::format("{}[{}]", IEditNode::m_name, index), data);
                        ++index;
                    }

                }
            }
        }
    }

public:
    TEditNode() = delete;
    explicit TEditNode(const std::string_view name, Type& data, IEditNode* parent = nullptr) : ITEditNode<Type>{ name, parent }, m_ref{ data }
    {
        if constexpr (is_reflective_v<Type>)
        {
            if constexpr (has_base<Type>::value)
            {
                // add parent members
                genChildrenFromParentMembers<Type::Base>(m_ref.get());
            }
            genChildrenFromMembers();
        }
        else if constexpr (is_std_array_v<Type> || is_std_vector_v<Type> || is_std_list_v<Type>)
        {
            int index = 0;
            for (auto& data : m_ref.get())
            {
                appendChild<Type::value_type>(std::format("{}[{}]", IEditNode::m_name, index), data);
                ++index;
            }
        }
    }

    virtual ~TEditNode() = default;

    void reset(Type& data) final
    {
        // reset data of vector or list
        m_ref = data;

        if constexpr (is_reflective_v<Type>)
        {
            if constexpr (has_base<Type>::value)
            {
                // add parent members
                resetChildrenFromParentMembers<Type::Base>(data);
            }
            resetChildrenFromMembers();
        }
        else if constexpr (is_std_array_v<Type> || is_std_vector_v<Type> || is_std_list_v<Type>)
        {
            resetSelfContainer();
        }
    }

    void append()override
    {
        if constexpr (is_std_vector_v<Type> || is_std_list_v<Type>)
        {
            typename Type::value_type newData;
            m_ref.get().push_back(newData);
        }
        resetSelfContainer();
    }

    void remove()override
    {
        if constexpr (is_std_vector_v<Type> || is_std_list_v<Type>)
        {
            if(!m_ref.get().empty())
                m_ref.get().pop_back();
        }
        resetSelfContainer();
    }

    QVariant checkRole()const override
    {
        if constexpr (is_std_optional_v<Type>)
            return m_ref.has_value() ? Qt::Checked : Qt::Unchecked;
        return QVariant();
    }

    void setCheckState(const Qt::CheckState state) override
    {
        if constexpr (is_std_optional_v<Type>)
        {
            if (Qt::Unchecked)
            {
                m_ref.reset();
            }
            else
            {
                m_ref.get() = Type::value_type();
            }
        }
    }

    void setValue(const QVariant& value) override
    {
        if constexpr (std::is_same_v<Type, std::string>)
        {
            auto str = value.toString();
            m_ref.get() = str.toStdString();
        }
        else
        {
            m_ref.get() = value.value<Type>();
        }
    }

    QVariant getValue()const override
    {
        if constexpr (is_std_array_v<Type> || is_std_vector_v<Type> || is_std_list_v<Type>)
            return QVariant();

        if constexpr (is_std_optional_v<Type>)
        {
            if (!m_ref.get().has_value())
                return QVariant();

            return QVariant::fromValue(m_ref.get().value());
        }

        if constexpr (std::is_same_v<Type, std::string>)
            return QString::fromStdString(m_ref.get());
        return QVariant::fromValue(m_ref.get());
    }

    std::reference_wrapper<Type> reference()
    {
        return m_ref;
    }

};
