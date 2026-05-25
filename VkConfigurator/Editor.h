#pragma once
#include <memory>

class QWidget;

/*@brief base class for editor*/
template<typename Type>
class Editor
{
protected:
    bool m_modified = false;

public:
    virtual void init(const Type& a_data) = 0;
    virtual bool setEdited(Type& a_data) = 0;
};

class EditGeneratorInterface
{
public:
    virtual QWidget* generateEditor() = 0;
};

using EditGeneratorInterfaceUPtr = std::unique_ptr<EditGeneratorInterface>;