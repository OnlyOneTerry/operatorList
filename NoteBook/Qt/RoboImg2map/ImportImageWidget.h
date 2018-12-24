#ifndef ImportImageWidget_H
#define ImportImageWidget_H

#include "CustomBaseDialog/CustomChildBaseDialog.h"
#include <QActionGroup>
#include "PublicClass.h"
#include "GraphicsView/EditorView.h"
#include "ImportImageThread.h"
#include "ZoomIn.h"
#include <QToolButton>
#include <QAction>

namespace Ui {
class ImportImageWidget;
}

class ImportImageWidget : public CustomChildBaseDialog
{
    Q_OBJECT

public:
    explicit ImportImageWidget(QWidget *parent = Q_NULLPTR);
    ~ImportImageWidget();

    void initEditViewToolBar();
    QAction *createAction(ActionData *data);
    void setBackgroundPixmap(const QPixmap &pixmap);
    bool isLineEditListEmpty();
    int currentLineEditIndex();
    bool eventFilter(QObject *watched, QEvent *event);
    void setButtonStatus(bool isRunning);
    void releaseAllItems();
    int stopImportImage();

    void init();
    bool willCloseWidget();
public slots:

    void slotEditViewToolBarTriggered(QAction *ac);

protected:
    void keyPressEvent(QKeyEvent *event);

protected slots:

    void slotImportImageThread();
    void slotThreadProgress(qint64 value, qint64 max);
    void slotZoomIn(int index, const QString &str);
    void slotMessage(const QString &msg, int type = 0);
private slots:
    void on_pushButton_startImport_clicked();
    void on_pushButton_zoomIn_clicked();
//    void on_pushButton_addLineEdit_clicked();

signals:
    void sigOpenImportSmap(const QString& path);
private:
    Ui::ImportImageWidget *ui = Q_NULLPTR;
    QList<QToolButton*>ToolButtonList;
    EditorView *pEditorView = Q_NULLPTR;
    ImportImageThread *pImportImageThread = Q_NULLPTR;

    QList<QLineEdit*>LineEditList;
    QString _filePath = "";
    ZoomIn *pZoomIn = Q_NULLPTR;
    int _currentLineEditIndex = 0;
};

#endif // ImportImageWidget_H
