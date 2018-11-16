#ifndef DIALOGMODELCHOSE_H
#define DIALOGMODELCHOSE_H

#include <QDialog>
#include"ModelCommon.h"
#include<QComboBox>
#include<QFileInfoList>
#include<QDir>
#include "CustomBaseDialog/CustomChildBaseDialog.h"
namespace Ui {
class DialogModelChose;
}

class DialogModelChose : public CustomChildBaseDialog
{
    Q_OBJECT

public:
    explicit DialogModelChose(QWidget *parent = 0);
    ~DialogModelChose();
    ConfigureData readConfigure_model(const QString& filePath);

    QComboBox *getCombox();
    void  loadMaps(QString path);
    void addFileToCombox(QFileInfoList list);
    QFileInfoList GetFileList(QString path);
signals:
    void sig_confirm();

private slots:
    void on_btn_ok_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::DialogModelChose *ui;
};

#endif // DIALOGMODELCHOSE_H
