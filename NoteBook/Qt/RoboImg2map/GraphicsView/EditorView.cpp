
/******************************************************************************
   HotShots: Screenshot utility
   Copyright(C) 2011-2014  xbee@xbee.net

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *******************************************************************************/

// Qt
#include <QtDebug>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QLineEdit>
#include <QMessageBox>
//#include <QtOpenGL/QGLWidget>
#include "EditorView.h"
#include "UiClass.h"
#include "PublicClass.h"
#include <QMimeData>
#include <QUrl>
#include <QFileInfo>
#include "ChipItem.h"
#include <QApplication>

const double defaultScaleFactor = 1.3;

EditorView::EditorView(QWidget * map) : QGraphicsView(map)
{
    // OpenGL
    //setViewport(new QGLWidget(this));

    // GUI
    setContentsMargins(1,1,1,1);
    // Initialize properties
    setMouseTracking(true);
//    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode (QGraphicsView::FullViewportUpdate); // because artifact (view rect diff scene rect !!)
    setRenderHint(QPainter::Antialiasing);//抗锯齿
    // Center
//    m_centerPoint = QPointF(0.0f,0.0f);
//    centerOn(m_centerPoint);
    initBackground();
    initScene();
}

EditorView::~EditorView()
{
    if(pEditorScene){
        delete pEditorScene;
    }
}
void EditorView::initScene()
{
    if(!pEditorScene){
        pEditorScene = new EditorScene(this);
        connect(pEditorScene,SIGNAL(sigCusor(QString,QCursor)),
                this,SLOT(slotCusor(QString,QCursor)));
    }
    this->setScene(pEditorScene);
}
void EditorView::updateSceneRect(const QRectF &rect)
{
    // the view can "see" sceneRect x 2
    setSceneRect( rect.adjusted(-rect.width() / 2,-rect.height() / 2,rect.width() / 2,rect.height() / 2) );
}

void EditorView::paintEvent(QPaintEvent* event)
{
    QPainter painter( viewport() );

    // draw background
    painter.drawTiledPixmap(rect(), m_tileBg);
    QGraphicsView::paintEvent(event);
}

bool EditorView::event(QEvent *e)
{
    switch ( e->type() )
    {
    case QEvent::Leave:
        emit mouseMoved( QPointF() );
        break;
    default:
        break;
    }
    return QGraphicsView::event(e);
}

void EditorView::initBackground()
{
    // recreate background tile
    m_tileBg = QPixmap(16, 16);
    m_tileBg.fill(Qt::white);
    QPainter pt(&m_tileBg);
    QColor color(202, 202, 202);
    pt.fillRect(0, 0, 8, 8, color);
    pt.fillRect(8, 8, 8, 8, color);
    pt.end();
}

void EditorView::fitScale()
{
    if ( scene() )
    {
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio );
        setCenter( scene()->sceneRect().center() );
        emit scaleChanged( transform().m22() );
    }
}
void EditorView::leaveEvent(QEvent *event)
{
    QGraphicsView::leaveEvent(event);
}
void EditorView::enterEvent(QEvent *event)
{
    QGraphicsView::enterEvent(event);
}
void EditorView::resetScale()
{
    setTransform( QTransform() );
    setCenter( scene()->sceneRect().center() );
    emit scaleChanged( transform().m22() );
}

/*
   void EditorView::setImage(const QImage &newImg)
   {
       m_navigatorWindow->setImage(QImage());  // clear overview

       m_image = newImg;
       m_imgRect = QRectF(0, 0, newImg.width(), newImg.height());

       m_worldMatrix.reset();
       updateImageMatrix();

       m_navigatorWindow->setImage(m_image);

       update();

   }
 */

/**
   *Handles the mouse move event
 */
void EditorView::mouseMoveEvent(QMouseEvent* event)
{
    QPointF mousePos = QGraphicsView::mapToScene( event->pos() );
    emit mouseMoved(mousePos);
    m_centerPoint = QGraphicsView::mapToScene( rect().center() );
    QGraphicsView::mouseMoveEvent(event);
}

/**
 * Zoom the view in and out.
 */

void EditorView::zoomPlus()
{
    zoom(defaultScaleFactor);
}

void EditorView::zoomMinus()
{
    zoom(1.0 / defaultScaleFactor);
}

void EditorView::zoom(double scaleFactor)
{
    scale(scaleFactor, scaleFactor);
    emit scaleChanged( transform().m22());
}

void EditorView::wheelEvent(QWheelEvent* event)
{
    //Get the position of the mouse before scaling, in scene coords
    QPointF pointBeforeScale( QGraphicsView::mapToScene( event->pos() ) );

    //Get the original screen centerpoint
    QPointF screenCenter = mapToScene( rect().center() );

    if(event->delta() > 0 )
        zoomPlus();
    else
        zoomMinus();

    //Get the position after scaling, in scene coords
    QPointF pointAfterScale( QGraphicsView::mapToScene( event->pos() ) );

    //Get the offset of how the screen moved
    QPointF offset = pointBeforeScale - pointAfterScale;

    //Adjust to the new center for correct zooming
    QPointF newCenter = screenCenter + offset;
    setCenter(newCenter);

    // mouse position has changed!!
    emit mouseMoved( QGraphicsView::mapToScene( event->pos() ) );
}

void EditorView::setCenter(const QPointF& position)
{
    m_centerPoint = position;
    centerOn(m_centerPoint);
}

void EditorView::resizeEvent(QResizeEvent* event)
{
    //Scale the view ie. do the zoom
    double heightscale;

    // Not on the width direction widthscale = (double)event->size().width() / event->oldSize().width();
    heightscale = (double)event->size().height() / event->oldSize().height();

    // Not on the width direction double doscale = widthscale<heightscale?widthscale:heightscale;
    if ( heightscale > 0 )
    {
        scale(heightscale,heightscale);
        emit scaleChanged( transform().m22() );
    }

    // Set center
    centerOn(m_centerPoint);

    //Call the subclass resize so the scrollbars are updated correctly
    QGraphicsView::resizeEvent(event);
}

//TODO  ------------- Drag and drop -----------
void EditorView::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData* mimeData = event->mimeData();

    if ( !mimeData->hasUrls() )
    {
        event->ignore();
        return;
    }

    QList<QUrl> urls = mimeData->urls();

    if(urls.count() != 1)
    {
        event->ignore();
        return;
    }

    QUrl url = urls.at(0);
    QString filename = url.toLocalFile();

    // We don't test extension
    if ( !QFileInfo(filename).exists() )
    {
        event->ignore();
        return;
    }

    if ( !QFileInfo(filename).isFile() )
    {
        event->ignore();
        return;
    }

    event->acceptProposedAction();
}

void EditorView::dragMoveEvent(QDragMoveEvent *event)
{
    if ( event->source() == this )
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        event->acceptProposedAction();
    }
}
EditorScene* EditorView::cScene()
{
    return pEditorScene;
}
void EditorView::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    QList<QUrl> urls = mimeData->urls();
    QUrl url = urls.at(0);

    QString filename = url.toLocalFile();
    qDebug()<<"dropEvent:filename";
    QStringList list = QString("%1").arg(Load_image_filter).split("*");

    QString lastFileName = filename.split(".").last();
    if(list.contains(lastFileName)){
        loadPixmap(filename);
        //        populateScene(filename);
    }else{
        UiClass::init()->showDialog(tr("Only images in the following formats are supported [%1]").arg(Load_image_filter));
    }
}
void EditorView::loadPixmap(const QPixmap &pix)
{
    if ( pEditorScene->hasItems() )
    {
//        QMessageBox msgBox;
//        msgBox.setText( tr("There is existing annotation items, what do you want to do?") );
//        QPushButton *clearButton = msgBox.addButton(tr("clear annotation items ..."), QMessageBox::ActionRole);
//        QPushButton *updateButton = msgBox.addButton(tr("update background image ..."), QMessageBox::ActionRole);

//        msgBox.exec();

//        if (msgBox.clickedButton() == clearButton)
//        {
            // clear
            pEditorScene->clearItems();
//        }
//        else if (msgBox.clickedButton() == updateButton)
//        {
//            // update
//        }
    }
    pEditorScene->setUnderlayImage( pix.toImage());
    this->fitScale();
}
void EditorView::populateScene(const QString &imagePath)
{
    QImage image(imagePath);

    // Populate scene
    int xx = 0;
    int nitems = 0;
    for (int i = -11000; i < 11000; i += 110) {
        ++xx;
        int yy = 0;
        for (int j = -7000; j < 7000; j += 70) {
            ++yy;
            qreal x = (i + 11000) / 22000.0;
            qreal y = (j + 7000) / 14000.0;

            QColor color(image.pixel(int(image.width() * x), int(image.height() * y)));
            QGraphicsItem *item = new ChipItem(color, xx, yy);
            item->setPos(QPointF(i, j));
            pEditorScene->addItem(item);

            ++nitems;
        }
    }
}
void EditorView::slotCusor(QString cusor,QCursor sor)
{
    qDebug()<<"slotCusor:"<<cusor;
    this->setCursor(sor);
}
