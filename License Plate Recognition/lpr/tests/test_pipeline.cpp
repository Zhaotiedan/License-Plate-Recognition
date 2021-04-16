
#pragma execution_character_set("utf-8")
#include "../include/Pipeline.h"
using namespace pr;
using namespace std;
#include<string>
#include<vector>


void TEST_PIPELINE(){

    pr::PipelinePR prc("../lpr/model/cascade.xml",
                      "../lpr/model/HorizonalFinemapping.prototxt","../lpr/model/HorizonalFinemapping.caffemodel",
                      "../lpr/model/Segmentation.prototxt","../lpr/model/Segmentation.caffemodel",
                      "../lpr/model/CharacterRecognization.prototxt","../lpr/model/CharacterRecognization.caffemodel"
                    );

	string ImgName;
	string ImgName1;
	//测试集文件下的txt文件路径
	ifstream fin("F:\\github\\bishe\\License-Plate-Recognition\\License Plate Recognition\\picture\\card- name.txt");
	while (getline(fin, ImgName))
	{
		cout << "Handle" << ImgName << endl;
		ImgName1 = "F:\\github\\bishe\\License-Plate-Recognition\\License Plate Recognition\\picture" + ImgName;
		cv::Mat image = cv::imread(ImgName1);

		//cv::Mat image = cv::imread("../7.jpg");//选择测试图片

		cv::imshow("image", image);
		cv::waitKey(0);

		std::vector<pr::PlateInfo> res = prc.RunPiplineAsImage(image);
		float conf = 0;
		for (auto st : res) {
			if (st.confidence > 0.1) {
				std::cout << st.getPlateName() << " " << st.confidence << std::endl;
				conf += st.confidence;
			}
		}
		std::cout << conf << std::endl;
	}
	system("pause");

}


int main()
{

   TEST_PIPELINE();


    return 0 ;


}