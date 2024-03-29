#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "CSTool.h"

int strToInt(const char* str)
{
	int res = 0;
	for (size_t i = 0; str[i]; i++)
		res = res * 10 + (str[i] - '0');
	return res;
}

int main(int argc, char** argv)
{
	CSTool::Image image;
	int out_height = 0, out_width = 0;
	CSTool::ColorSpace out_color_space = CSTool::ColorSpace::Undefined_CS;

	for (size_t i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-i") == 0)
			image.setInputFileName(argv[++i]);
		else if (strcmp(argv[i], "-iw") == 0)
			image.height = strToInt(argv[++i]);
		else if (strcmp(argv[i], "-ih") == 0)
			image.width = strToInt(argv[++i]);
		else if (strcmp(argv[i], "-ic") == 0)
			image.input_CS = CSTool::StrToColorSpace(argv[++i]);
		else if (strcmp(argv[i], "-o") == 0)
			image.setOutputFileName(argv[++i]);
		else if (strcmp(argv[i], "-ow") == 0)
			out_width = strToInt(argv[++i]);
		else if (strcmp(argv[i], "-oh") == 0)
			out_height = strToInt(argv[++i]);
		else if (strcmp(argv[i], "-oc") == 0)
			out_color_space = CSTool::StrToColorSpace(argv[++i]);
	}

	if (image.height != out_height && image.width != out_width)
	{
		std::cout << "Error! Images' resolution do not match." << std::endl;
		system("pause");
		exit(-1);
	}

	if (image.getInputFileName() == nullptr)
	{
		std::cout << "Error! Input file name is not specified." << std::endl;
		system("pause");
		exit(-1);
	}

	if (image.getOutputFileName() == nullptr)
	{
		std::cout << "Error! Output file name is not specified." << std::endl;
		system("pause");
		exit(-1);
	}

	image.convertColorSpaceTo(out_color_space);
	return 0;
}
