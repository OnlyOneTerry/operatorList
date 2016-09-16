#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

//获取字符串最后的名称
QString getFileName(const QString & path)
{
    int  index = path.lastIndexOf('/');

    QString fileName = path.right(path.length()-index-1);

    return fileName;
}

//getParentPath

QString getParentPath(const QString &path)
{
    QString parentPth = "";
    int index = path.lastIndexOf('/');

    parentPth = path.left(index);

    return parentPth;
}


//combinString
QString  getString(int num , QString str )
{
   QList<QString>  tempStrlist = str.split('/');
   QString tempStr = "";

   for(int i = 0 ; i < num ; i++)
   {
       tempStr = tempStr + tempStrlist[i] +'/';
   }
//  qDebug()<<"tempStr is %s "<<tempStr;
   return tempStr;
}

//getChildenPath
QList<QString> getChildPath(const QString & path,QList<QString>& videoList)
{
    QList<QString> tempVideolist ;

     QList<QString> splitList = path.split('/');

     int num = splitList.length();

   for(int i = 0 ; i < videoList.length();i++)
   {
       if((videoList[i].split('/').length() == num + 1) && (0 == (path+'/').compare(getString(num,videoList[i]))) )
       {
           tempVideolist.push_back(videoList[i]);

       }
   }

   return tempVideolist;
}

//判断是否为文件夹
bool isFolder(const QString& lastString)
{
    int index = lastString.lastIndexOf('.');
    if(-1 == index)
    {
        return true;
    }
    else
        return false;
}

//

//获取相对路径文件及文件夹
QList<QString> getAllfile_RelativePath(const QString & path, QList<QString>& videolist )
{
   int length = path.length()+1;
   QString  topPth = path+'/';
   QList<QString> tempPth ;
   QList<QString>::iterator iter = videolist.begin();

   for(;iter < videolist.end(); iter++)
   {
      QString temp = *iter;

      if(0 == topPth.compare(temp.left(length)))
      {
//          qDebug()<<"%s"<<temp;

          tempPth.push_back(temp);
      }
   }

   return tempPth;
}

//sort the list to put the directory in the front of  the list

QList<QString> sortList(QList<QString>& videolist)
{
    QList<QString> tempList ;
    QList<QString>::iterator  iter1 = videolist.begin();
    QList<QString>::iterator  iter2 = videolist.begin();

    for(;iter1 < videolist.end();iter1++)
    {
        if(isFolder(*iter1))
        {
//            qDebug()<<"dir is "<<*iter1;
            tempList.push_back(*iter1);
        }
    }

    for(;iter2 < videolist.end();iter2++)
    {
        if(!isFolder(*iter2))
        {
//            qDebug()<<"file is "<<*iter2;
            tempList.push_back(*iter2);
        }
    }

    return tempList;

}
//打印列表内容
void display( QList<QString>& videolist)
{

      QList<QString> tempList = sortList(videolist);

     QList<QString>::iterator iter = tempList.begin();

    for(; iter < tempList.end() ; iter++)
    {
        qDebug()<<"%s"<<*iter;
    }

}


int main(int argc, char *argv[])
{


    QString dirPath = "home";
    QString dirPath1 = "home/jindong1/ctj";
    QString dirPath4 = "home/jindong2/ctj";
    QString dirPath2 = "home/jindong2";
    QString dirPath3 = "home/jindong2/videos";
    QString path1 = "home/jindong/myheartWillgoOn.mp4";
    QString path2= "home/jindong/myheart.mp4";
    QString path3 = "home/jindong/tangrui/mygirl.mp4";
    QString path4= "home/jindong/chinaVoice.mp4";
    QString path5 = "home/jindong1/ctj/myheartWillgoOn.mp4";
    QString path6= "home/jindong1/myheart.mp4";
    QString path7 = "home/jindong1/tangrui/mygirl.mp4";
    QString path8= "home/jindong1/ctj/chinaVoice.mp4";
    QString path9 = "home/jindong/myheart.mp4";
    QString path10 = "home/jindong/myheartWillgoOn.mp4";
    QString path11 = "home/jindong/myheartWillgo.mp4";
    QString path12 = "home/jindong2/ctj/nizhan.mp4";
    QString path13 = "home/jindong2/myheartWillgoOn.mp4";
    QString path14 = "home/jindong2/myheartWillgo.mp4";
    QString path15 = "home/jindong/myheartWillgoOn.mp4";
    QString path16 = "home/jindong/myheartWillgo.mp4";
    QString path17 = "home/jindong2/myheart.mp4";
    QString path18 = "home/jindong2/myheartWillgoOn.mp4";
    QString path19 = "home/jindong2/myheartWillgo.mp4";
    QString fileName  = getFileName(path1);
    //if the file is a dir then insert it to videodirList
    //所有视频文件列表
    QList<QString> videofileLists;
    videofileLists.push_back(dirPath);
    videofileLists.push_back(dirPath1);
    videofileLists.push_back(dirPath2);
    videofileLists.push_back(dirPath3);
    videofileLists.push_back(dirPath4);
    videofileLists.push_back(path1);
    videofileLists.push_back(path2);
    videofileLists.push_back(path3);
    videofileLists.push_back(path4);
    videofileLists.push_back(path5);
    videofileLists.push_back(path6);
    videofileLists.push_back(path7);
    videofileLists.push_back(path8);
    videofileLists.push_back(path9);
    videofileLists.push_back(path10);
    videofileLists.push_back(path11);
    videofileLists.push_back(path12);
    videofileLists.push_back(path13);
    videofileLists.push_back(path14);
    videofileLists.push_back(path15);
    videofileLists.push_back(path16);
    videofileLists.push_back(path17);
    videofileLists.push_back(path18);
    videofileLists.push_back(path19);

//所有文件夹列表
    QList<QString>videodirList;

    QList<QString>::iterator iter = videofileLists.begin();

    for (; iter != videofileLists.end(); iter ++)
    {
        QString tempPth = *iter;
        QString tempName = getFileName(tempPth);
        if(isFolder(tempName))
        {
            videodirList.push_back(*iter);
        }
    }

    display(videofileLists);

//打印相关的文件夹下的文件
//    for(int i = 0 ; i < videodirList.length();i++)
//    {

//        QList<QString> relativFileOrFile =  getAllfile_RelativePath(videodirList[i],videofileLists);

//           display(relativFileOrFile);
//    }

//    getParentPath(videodirList[0]);

//     qDebug()<<"current path is %s "<< videodirList[0];
//     qDebug()<<"parent path is %s "<< getParentPath(videodirList[0]);

//     QList<QString> relativFileOrFile =  getAllfile_RelativePath( getParentPath(videodirList[0]),videofileLists);
//           QList<QString> list =  getChildPath(videodirList[0],videofileLists);

//    qDebug()<<"the child file or dir is :\n ";
//        display(list);
    return 0;
}

