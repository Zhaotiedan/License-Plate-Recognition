

#include "FastDeskew.h"
using namespace std;
using namespace cv;
#include<vector>

namespace pr{


    const int  ANGLE_MIN = 30 ;
    const int ANGLE_MAX = 150 ;
    const int PLATE_H = 36;
    const int PLATE_W = 136;

    int angle(float x,float y)
    {
        return atan2(x,y)*180/3.1415;
    }

    vector<float> avgfilter(vector<float> angle_list,int windowsSize) 
	{
        vector<float> angle_list_filtered(angle_list.size() - windowsSize + 1);
        for (int i = 0; i < angle_list.size() - windowsSize + 1; i++) 
		{
            float avg = 0.00f;
            for (int j = 0; j < windowsSize; j++) 
			{
                avg += angle_list[i + j];
            }
            avg = avg / windowsSize;
            angle_list_filtered[i] = avg;
        }

        return angle_list_filtered;
    }


    void drawHist(vector<float> seq)
	{
        Mat image(300,seq.size(),CV_8U);
        image.setTo(0);

        for(int i = 0;i<seq.size();i++)
        {
            float l = *max_element(seq.begin(),seq.end());

            int p = int(float(seq[i])/l*300);

            line(image,Point(i,300),Point(i,300-p),Scalar(255,255,255));
        }
        imshow("vis",image);
    }

    Mat  correctPlateImage(cv::Mat skewPlate,float angle,float maxAngle)
    {

        Mat dst;

        Size size_o(skewPlate.cols,skewPlate.rows);


        int extend_padding = 0;
//        if(angle<0)
            extend_padding = static_cast<int>(skewPlate.rows*tan(abs(angle)/180* 3.14) );
//        else
//            extend_padding = static_cast<int>(skewPlate.rows/tan(cv::abs(angle)/180* 3.14) );

//        cout<<"extend:"<<extend_padding<<std::endl;

        Size size(skewPlate.cols + extend_padding ,skewPlate.rows);

        float interval = abs(sin((angle /180) * 3.14)* skewPlate.rows);
//        cout<<interval<<std::endl;

        Point2f pts1[4] = {Point2f(0,0),Point2f(0,size_o.height),Point2f(size_o.width,0),Point2f(size_o.width,size_o.height)};
        if(angle>0) 
		{
            Point2f pts2[4] = {Point2f(interval, 0), Point2f(0, size_o.height),
                                   Point2f(size_o.width, 0), Point2f(size_o.width - interval, size_o.height)};
            Mat M  = getPerspectiveTransform(pts1,pts2);
            warpPerspective(skewPlate,dst,M,size);

        }
        else 
		{
            Point2f pts2[4] = {Point2f(0, 0), Point2f(interval, size_o.height), Point2f(size_o.width-interval, 0),
                                   Point2f(size_o.width, size_o.height)};
            Mat M  = getPerspectiveTransform(pts1,pts2);
            warpPerspective(skewPlate,dst,M,size,INTER_CUBIC);

        }
        return  dst;
    }
    Mat fastdeskew(Mat skewImage,int blockSize)
	{


        const int FILTER_WINDOWS_SIZE = 5;
        vector<float> angle_list(180);
        memset(angle_list.data(),0,angle_list.size()*sizeof(int));

        Mat bak;
        skewImage.copyTo(bak);
        if(skewImage.channels() == 3)
            cvtColor(skewImage,skewImage,COLOR_RGB2GRAY);

        if(skewImage.channels() == 1)
        {
            Mat eigen;

            cornerEigenValsAndVecs(skewImage,eigen,blockSize,5);
            for( int j = 0; j < skewImage.rows; j+=blockSize )
            { for( int i = 0; i < skewImage.cols; i+=blockSize )
                {
                    float x2 = eigen.at<Vec6f>(j, i)[4];
                    float y2 = eigen.at<Vec6f>(j, i)[5];
                    int angle_cell = angle(x2,y2);
                    angle_list[(angle_cell + 180)%180]+=1.0;

                }
            }
        }
        vector<float> filtered = avgfilter(angle_list,5);

        int maxPos = max_element(filtered.begin(),filtered.end()) - filtered.begin() + FILTER_WINDOWS_SIZE/2;
        if(maxPos>ANGLE_MAX)
            maxPos = (-maxPos+90+180)%180;
        if(maxPos<ANGLE_MIN)
            maxPos-=90;
        maxPos=90-maxPos;
		Mat deskewed = correctPlateImage(bak, static_cast<float>(maxPos),60.0f);
        return deskewed;
    }



}//namespace pr
