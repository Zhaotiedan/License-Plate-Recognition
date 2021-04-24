
#include "../include/Pipeline.h"
using namespace pr;
using namespace std;
using namespace cv;
#include<string>
#include<vector>


void TEST_PIPELINE(){
	//pr
    PipelinePR prc("../lpr/model/cascade.xml",
                      "../lpr/model/HorizonalFinemapping.prototxt","../lpr/model/HorizonalFinemapping.caffemodel",
                      "../lpr/model/Segmentation.prototxt","../lpr/model/Segmentation.caffemodel",
                      "../lpr/model/CharacterRecognization.prototxt","../lpr/model/CharacterRecognization.caffemodel"
                    );

	string ImgName;//打开的文件名 路径
	string ImgName1;//打开的文件中的对应图片名 路径
	//测试集文件下的txt文件路径
	ifstream fin("F:\\github\\bishe\\License-Plate-Recognition\\License Plate Recognition\\picture\\card- name.txt");

	   //若文件不能打开
	if (!fin)
	{
		cout << "文件打开失败" << endl;
	}
	while (getline(fin, ImgName))//fin:用于文本读取的对象，ImgName：存储从读出的文本对象
	{
		cout << "Handle " << ImgName << endl;
		ImgName1 = "F:\\github\\bishe\\License-Plate-Recognition\\License Plate Recognition\\picture\\" + ImgName;
		//cout << "ImgName1: " << ImgName1 << endl;
		Mat image = imread(ImgName1);//选择测试图片 cv

		//cv::Mat image = cv::imread("../7.jpg");//选择测试图片

		if (!image.empty())
		{
			imshow("image", image);
			waitKey(50);

			//使用端到端模型模型进行识别，识别结果将会保存在res里面
			vector<pr::PlateInfo> res = prc.RunPiplineAsImage(image);

			float maxconf = 0.5; //初始最大置信度
			string finaln;
			for (auto st : res) 
			{
				//判断最大置信度下的识别结果		
				if (st.confidence > maxconf) 
				{
					//cout << st.getPlateName() << " " << st.confidence << endl;
					maxconf = st.confidence;
					finaln = st.getPlateName();

				}
			}
			cout << maxconf << endl;
		}
	}

}


int main()
{

   TEST_PIPELINE();
   system("pause");

    return 0 ;
}

