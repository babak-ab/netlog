#include "setting.h"
Setting* Setting::m_instance = Q_NULLPTR;
Setting::Setting(QObject* parent)
    : QObject(parent)
{
    m_instance = this;
}

Setting* Setting::instance()
{
    if (m_instance == Q_NULLPTR)
        m_instance = new Setting;
    return m_instance;
}

Setting::InputType Setting::inputType() const
{
    return m_inputType;
}

void Setting::setInputType(InputType inputType)
{
    m_inputType = inputType;
    Q_EMIT valueChanged();
}
