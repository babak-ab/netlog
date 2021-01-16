#ifndef SETTING_H
#define SETTING_H

#include <QDebug>
#include <QObject>

class Setting : public QObject {
    Q_OBJECT
public:
    enum InputType {
        InputType_Dec,
        InputType_Bin,
        InputType_Hex,
        InputType_ASCII,
    };
    explicit Setting(QObject* parent = Q_NULLPTR);

    static Setting* instance();

    InputType inputType() const;
    void setInputType(InputType inputType);

    InputType sendType() const;
    void setSendType(const InputType& sendType);

private:
    bool m_hasOxPrefix = false;
    InputType m_inputType = InputType_Hex;
    InputType m_sendType = InputType_Hex;

    static Setting* m_instance;

Q_SIGNALS:
    void valueChanged();
};

#endif // SETTING_H
