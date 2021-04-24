#ifndef SWIFTPR_PIPLINE_H
#define SWIFTPR_PIPLINE_H

#include <mysql.h>
#include "PlateDetection.h"
#include "PlateSegmentation.h"
#include "CNNRecognizer.h"
#include "PlateInfo.h"
#include "FastDeskew.h"
#include "FineMapping.h"
#include "Recognizer.h"

namespace pr {
	class PipelinePR {
	public:
		GeneralRecognizer *generalRecognizer;
		PlateDetection *plateDetection;
		PlateSegmentation *plateSegmentation;
		FineMapping *fineMapping;
		PipelinePR(std::string detector_filename,
			std::string finemapping_prototxt, std::string finemapping_caffemodel,
			std::string segmentation_prototxt, std::string segmentation_caffemodel,
			std::string charRecognization_proto, std::string charRecognization_caffemodel
		) {
			plateDetection = new PlateDetection(detector_filename);
			fineMapping = new FineMapping(finemapping_prototxt, finemapping_caffemodel);
			plateSegmentation = new PlateSegmentation(segmentation_prototxt, segmentation_caffemodel);
			generalRecognizer = new CNNRecognizer(charRecognization_proto, charRecognization_caffemodel);
		}

		~PipelinePR() {

			delete plateDetection;
			delete fineMapping;
			delete plateSegmentation;
			delete generalRecognizer;
		}

		std::vector<std::string> chars_code{ "京","沪","津","渝","冀","晋","蒙","辽","吉","黑","苏","浙","皖","闽","赣","鲁","豫","鄂","湘","粤","桂","琼","川","贵","云","藏","陕","甘","青","宁","新","0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","J","K","L","M","N","P","Q","R","S","T","U","V","W","X","Y","Z" };

		std::vector<std::string> plateRes;
		std::vector<PlateInfo> RunPiplineAsImage(cv::Mat plateImage) {
			std::vector<PlateInfo> results;
			std::vector<pr::PlateInfo> plates;
			plateDetection->plateDetectionRough(plateImage, plates);


			//数据库的连接
			MYSQL mysql;    //一个数据库结构体
			MYSQL_RES* res; //一个结果集结构体
			MYSQL_ROW row;  //char** 二维数组，存放一条条记录
			//初始化数据库
			mysql_init(&mysql);
			//设置编码方式
			mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
			//连接数据库
			//判断如果连接失败就输出连接失败。
			//注意你连接的账户名密码
			if (mysql_real_connect(&mysql, "localhost", "root", "123456", "plate", 3306, NULL, 0) == NULL)
			{
				cout << "连接失败！" << endl;	//注意你连接的账户名密码
			}

			for (pr::PlateInfo plateinfo : plates) {

				cv::Mat image_finemapping = plateinfo.getPlateImage();
				image_finemapping = fineMapping->FineMappingVertical(image_finemapping);
				image_finemapping = pr::fastdeskew(image_finemapping, 5);
				image_finemapping = fineMapping->FineMappingHorizon(image_finemapping, 2, 5);
				cv::resize(image_finemapping, image_finemapping, cv::Size(136, 36));
				plateinfo.setPlateImage(image_finemapping);
				std::vector<cv::Rect> rects;
				plateSegmentation->segmentPlatePipline(plateinfo, 1, rects);
				plateSegmentation->ExtractRegions(plateinfo, rects);
				cv::copyMakeBorder(image_finemapping, image_finemapping, 0, 0, 0, 20, cv::BORDER_REPLICATE);

				plateinfo.setPlateImage(image_finemapping);
				generalRecognizer->SegmentBasedSequenceRecognition(plateinfo);
				plateinfo.decodePlateNormal(chars_code);
				results.push_back(plateinfo);

				std::cout << plateinfo.getPlateName() << std::endl;


			//插入数据
				//先将cv::String转换为std::的string，然后再将该string转换为c中的char*字符串
				std::string c = plateinfo.getPlateName();
				const char* a = c.c_str();
				
				//测试  cout << c;
				char que[200];
				//sprintf()函数构造sql语句
				sprintf_s(que, "insert into  plateinfor(ReadTime,Name) values(now(),'%s');", a);
				int ret = mysql_query(&mysql, que);
				if (ret)
				{
					cout << "插入数据失败" << endl;
				}
				else
				{
					cout << "插入数据成功,共插入：" << mysql_affected_rows(&mysql) << "行" << endl;
				}

			}
			return results;
		}

	};
}
#endif //SWIFTPR_PIPLINE_H