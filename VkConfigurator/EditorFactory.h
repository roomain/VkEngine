#pragma once
#include <typeindex>
#include <unordered_map>
#include "Reflective_traits.h"
#include "Editor.h"

class EditorFactory
{
private:
    std::unordered_map<std::type_index, EditGeneratorInterfaceUPtr> m_factoryDatabase;

    EditorFactory();

public:
    [[nodiscard]] static EditorFactory& instance()
    {
        static EditorFactory s_instance;
        return s_instance;
    }

    template<typename Type>
    void setTypeFactory(EditGeneratorInterfaceUPtr&& a_factory)
    {
        m_factoryDatabase.emplace(std::type_index(typeid(Type)), std::move(a_factory));
    }

    template<typename Type>
    QWidget* generateInternal()
    {
        if constexpr (is_reflective_v<Type>)
        {
            //
        }
        else if constexpr (is_std_array_v<Type>)
        {
            //
        }
        else if constexpr (is_std_list_v<Type> || is_std_vector<Type>)
        {
            //
        }
        else if constexpr (is_optional_reflective_v<Type>)
        {
            //
        }
        else if constexpr (is_std_optional_v<Type>)
        {
            //
        }
        else if (auto iter = m_factoryDatabase.find(std::type_index(typeid(Type))); iter != m_factoryDatabase.cend())
        {
            auto widget = iter->second->generateEditor();
            if (auto editor = dynamic_cast<Editor<Type>*>(widget) == nullptr)
            {
                // todo
                return widget;
            }
            else
            {
                delete widget;
                return nullptr;
            }
        }
        return nullptr;
    }
};