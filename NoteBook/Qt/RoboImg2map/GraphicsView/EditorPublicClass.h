#ifndef EditorPublicClass_H
#define EditorPublicClass_H

#include <QObject>
#include <QDoubleSpinBox>

class EditorPublicClass : public QObject
{
    Q_OBJECT
public:
    explicit EditorPublicClass(QObject *parent = nullptr);
    static EditorPublicClass *init();
    QDoubleSpinBox *DoubleSpinBox = Q_NULLPTR;

private:
    static EditorPublicClass *pEditorPublicClass;

};

#endif // EditorPublicClass_H
