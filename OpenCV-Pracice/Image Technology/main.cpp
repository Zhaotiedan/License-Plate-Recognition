参考blog：
1.高斯模糊：https://blog.csdn.net/sileixinhua/article/details/78232758
2.灰度化处理：https://blog.csdn.net/sileixinhua/article/details/78232579
3.sobel：
  原理介绍：https://www.cnblogs.com/chenzhen0530/p/14660498.html
  代码运用：https://blog.csdn.net/sileixinhua/article/details/78232791
4.二值化处理：
5.闭操作：https://blog.csdn.net/xddwz/article/details/111406534


//测试代码
#define _CRT_SECURE_NO_WARNINGS 

#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;

//1. 高斯模糊
int GaussianBlur()
{
	cv::Mat src;
	cv::Mat dst;
	const char* filename = "F:\\github\\bishe\\OpencvPractice\\test.jpg";

	cv::imread(filename).copyTo(src);
	if (src.empty())
	{
		throw("Faild open file.");
	}
	int ksize1 = 11;
	int ksize2 = 11;
	double sigma1 = 10.0;
	double sigma2 = 20.0;
	cv::GaussianBlur(src, dst, cv::Size(ksize1, ksize2), sigma1, sigma2);
	//Size为高斯模糊的半径,size越大，模糊越深
	//第三，第四，第五参数为高斯模糊的度数

	cv::imshow("src", src);
	cv::imshow("dst", dst);
	cv::imwrite("F:\\github\\bishe\\OpencvPractice\\GaussianBlur-result.jpg", dst);

	cv::waitKey();

	return 0;
}

//2.图像灰度化处理
int GrayScale()
{
	const char* filename = "F:\\github\\bishe\\OpencvPractice\\GaussianBlur-result.jpg";
	cv::Mat src;
	cv::Mat dst;

	cv::imread(filename).copyTo(src);
	if (src.empty()) {
		throw("Faild open file.");
	}
	cv::cvtColor(src, dst, cv::COLOR_RGB2GRAY);

	cv::imshow("src", src);
	cv::imshow("dst", dst);
	cv::imwrite("F:\\github\\bishe\\OpencvPractice\\GrayScale-result.jpg", dst);

	cv::waitKey();

	return 0;
}

//3. sobel算子
int Sobel()
{
	const char* filename = "F:\\github\\bishe\\OpencvPractice\\GrayScale-result.jpg";
	cv::Mat src;
	cv::Mat dst;

	cv::imread(filename).copyTo(src);
	if (src.empty()) {
		throw("Faild open file.");
	}

	Sobel(src, dst, 1, 0, 1);
	//第三个参数为xorder x 方向上的差分阶数
	//第四个参数为yorder y 方向上的差分阶数
	//第五个参数为ksize 扩展 Sobel 核的大小，必须是 1, 3, 5 或 7

	cv::imshow("src", src);
	cv::imshow("dst", dst);
	cv::imwrite("F:\\github\\bishe\\OpencvPractice\\Sobel-result.jpg", dst);

	cv::waitKey();

	return 0;
	
}

//4.二值化 有bug

int Threshold()
{
	const char* filename = "F:\\github\\bishe\\OpencvPractice\\Sobel-result.jpg";
	cv::Mat src;
	cv::Mat dst;
	double thresh = 60.0, maxval = 180.0;
	cv::imread(filename).copyTo(src);
	if (src.empty()) {
		throw("Faild open file.");
	}

	cv::equalizeHist(src, dst);
	thresh = 80.0;
	maxval = 210.0;
	cv::threshold(src, dst, thresh, maxval, CV_THRESH_BINARY);

	cv::imshow("src", src);
	cv::imshow("dst", dst);
	cv::imwrite("F:\\github\\bishe\\OpencvPractice\\threshold-result.jpg", dst);

	cv::waitKey();

	return 0;
}

//5.闭操作
void Mor()
{
	using namespace cv;
	Mat src;
	src = imread("F:\\github\\bishe\\OpencvPractice\\Mortest.jpg");
	Mat element;
	element = getStructuringElement(MORPH_RECT, cv::Size(15, 15));
	Mat dst;
	//    第一种实现方法
	morphologyEx(src, dst, MORPH_CLOSE, element);
	imshow("src", src);
	imshow("dst", dst);
	imwrite("F:\\github\\bishe\\OpencvPractice\\close-result.jpg", dst);
	cv::waitKey();
}

int main()
{
	//GaussianBlur();
	//GrayScale();
	//Sobel();
	//Threshold();
	Mor();
	system("pause");
	return 0;
}
