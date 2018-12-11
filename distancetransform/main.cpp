#include <QCoreApplication>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
#include<vector>
#include<QDebug>
using namespace cv;
using namespace std;

pair<Point,double> DetectInCircles(vector<Point>contours,Mat src)
{
   Mat dist_image;
   distanceTransform(src,dist_image,CV_DIST_L2,3);
   int temp = 0,R = 0,cx = 0,cy = 0;
   int d;
   for(int i = 0;i<src.rows;i++)
   {
       for(int j = 0;j<src.cols;j++)
       {
           /*
            * checks if the point is inside the contour, Optionally computes the
            * signed distance from the point to the contour boundary
        */
           d = pointPolygonTest(contours,Point2f(j,i),0);
           if(d>0)
           {
               temp = (int)dist_image.ptr<float>(i)[j];
               if(temp>R)
               {
                   R = temp;
                   cy = i;
                   cx = j;
               }
           }
       }
   }
   return make_pair(Point(cx,cy),R);
}

int main()
{
    //read input binary image
    Mat src = imread("D:/images/hand.jpg");
    if(!src.data)
    {
        qDebug()<<"failed to load image....";
    }
    Mat image;
    cvtColor(src,image,CV_BGR2GRAY);
    vector<vector<Point>> contours;
    //findContours
    findContours(image,
                 contours,
                 CV_RETR_EXTERNAL,
                 CV_CHAIN_APPROX_NONE);
    //print contous length 轮廓的个数
    qDebug()<<"Contours:"<<contours.size();
    vector<vector<Point>>::const_iterator itContours = contours.begin();
    for(;itContours!=contours.end();++itContours)
    {
        qDebug()<<"size:"<<itContours->size();//每个轮廓包含的点数
    }
    //找到最大轮廓
    int index = 0,maxArea = 0;
    for(unsigned int i = 0;i<contours.size();i++)
    {
        int area = contourArea(contours[i]);
        if(area>maxArea)
        {
            index = i;
            maxArea = area;
        }
    }
    //draw black contours on white image
    Mat result(image.size(),CV_8U,Scalar(0));
    drawContours(result,contours,//画出轮廓
                 -1,//draw all contours
                 Scalar(255),//in black
                 2);

    pair<Point,double> m = DetectInCircles(contours[index],image);

    qDebug()<<m.first.x<<" "<<m.first.y<<" "<<m.second;
    circle(src,m.first,3,Scalar(0,0,255),1);
    namedWindow("result");
    imshow("result",src);
    waitKey(0);
    return 0;
}

int mainDistanceDemo(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Mat src = imread("D:/images/classic.jpg");
    imshow("src", src);

    Mat bin;
    cvtColor(src, bin, CV_BGR2GRAY);
    threshold(bin, bin, 80, 255, CV_THRESH_BINARY);
    imshow("bin", bin);
    qDebug()<<"bin channels is "<<bin.channels();

    Mat Dist, Labels;
    distanceTransform(bin, Dist, CV_DIST_L1, 3);
    normalize(Dist, Dist, 0, 1, NORM_MINMAX);
    imshow("distanceTransform", Dist);
    //归一化矩阵
    normalize(Dist,Dist,0,1.,cv::NORM_MINMAX);
    imshow("normal",Dist);
    waitKey(0);
    return a.exec();
}
