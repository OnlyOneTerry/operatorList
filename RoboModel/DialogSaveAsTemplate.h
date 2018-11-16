#ifndef DIALOGSAVEASTEMPLATE_H
#define DIALOGSAVEASTEMPLATE_H

#include <QDialog>
#include"ModelCommon.h"
#include "CustomBaseDialog/CustomChildBaseDialog.h"
namespace Ui {
class DialogSaveAsTemplate;
}

class DialogSaveAsTemplate : public CustomChildBaseDialog
{
    Q_OBJECT

public:
    explicit DialogSaveAsTemplate(QWidget *parent = 0);
    ~DialogSaveAsTemplate();
    //获取机器人目录下模型文件数量
    QStringList getTmpNames();
signals:
    void sig_confirm(QString name);
    void sig_cancel();
private slots:
    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::DialogSaveAsTemplate *ui;
};

#endif // DIALOGSAVEASTEMPLATE_H
