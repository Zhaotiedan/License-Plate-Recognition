

#include "../include/Pipeline.h"
using namespace pr;


//mine
#include<string>
using namespace std;



#if 0
void TEST_PIPELINE(){

    pr::PipelinePR prc("../lpr/model/cascade.xml",
                      "../lpr/model/HorizonalFinemapping.prototxt","../lpr/model/HorizonalFinemapping.caffemodel",
                      "../lpr/model/Segmentation.prototxt","../lpr/model/Segmentation.caffemodel",
                      "../lpr/model/CharacterRecognization.prototxt","../lpr/model/CharacterRecognization.caffemodel"
                    );

    cv::Mat image = cv::imread("../7.jpg");//选择测试图片
    cv::imshow("image",image);
    cv::waitKey(0);

    std::vector<pr::PlateInfo> res = prc.RunPiplineAsImage(image);
    float conf = 0 ;
    for(auto st:res) {
        if(st.confidence>0.1) {
            std::cout << st.getPlateName() << " " << st.confidence << std::endl;
            conf += st.confidence;
        }
    }
    std::cout<<conf<<std::endl;
	system("pause");


}

#endif

void TEST_PIPELINE()
{
	string ImgName;
	string ImgName1;

	//测试集文件下的txt文件路径
	ifstream fin("F:\\github\\HyperLPR-master-master\\HyperLPR-master-master\\Prj-Win\\检测车牌照片\\车牌文件名.txt");
	//打开要输出的.csv文件
	ofstream oFile;
	oFile.open("检测车牌信息.csv", ios::out | ios::trunc);
	//先写入表格标题
	oFile << "车牌号" << "," << "测试文件名" << endl;
	//判断是否读完txt
	while (getline(fin, ImgName)) {
		cout << "Handle" << ImgName << endl;
		ImgName1 = "F:\\github\\HyperLPR - master - master\\HyperLPR - master - master\\Prj - Win\\检测车牌照片" + ImgName;
		//image = imread(ImgName); 
		cv::Mat image = cv::imread(ImgName1);
		if (!image.empty()) {
			//使用端到端模型模型进行识别，识别结果将会保存在res里面
			std::vector<pr::PlateInfo> res = prc.RunPiplineAsImage(image);
			float maxconf = 0.5; //初始最大置信度
			string finaln;
			for (auto st : res) {
				//判断最大置信度下的识别结果		
				if (st.confidence > maxconf) {
					//std::cout << st.getPlateName() << " " << st.confidence << std::endl;
					maxconf = st.confidence;
					finaln = st.getPlateName();

				}
			}
			//将结果写入到.csv文件中
			oFile << finaln << "," << ImgName1 << endl;
			cout << "Done" << endl;
		}
	}
	oFile.close();
	cvWaitKey(0);

}
int main()
{

   TEST_PIPELINE();


    return 0 ;


}