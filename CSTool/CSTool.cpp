#include "CSTool.h"

//All color space values between 0 and 255

CSTool::YUV::YUV(uint8_t _Y, uint8_t _U, uint8_t _V) :
	Y(_Y),
	U(_U),
	V(_V)
{}

CSTool::AYUV::AYUV(uint8_t _A, uint8_t _Y, uint8_t _U, uint8_t _V) :
	YUV(_Y, _U, _V),
	A(_A)
{}

CSTool::AYUV::AYUV(const YUV& yuv, uint8_t _A) :
	A(_A),
	YUV(yuv)
{}

CSTool::VUYA::VUYA(uint8_t _V, uint8_t _U, uint8_t _Y, uint8_t _A) :
	YUV(_Y, _U, _V),
	A(_A)
{}

CSTool::VUYA::VUYA(const YUV& yuv, uint8_t _A) :
	A(_A),
	YUV(yuv)
{}

CSTool::RGB24::RGB24(uint8_t _R, uint8_t _G, uint8_t _B) :
	R(_R),
	G(_G),
	B(_B)
{}

CSTool::ARGB::ARGB(uint8_t _A, uint8_t _R, uint8_t _G, uint8_t _B) :
	RGB24(_R, _G, _B),
	A(_A)
{}

CSTool::ARGB::ARGB(const RGB24& rgb, uint8_t _A) :
	A(_A),
	RGB24(rgb)
{}

CSTool::BGRA::BGRA(uint8_t _B, uint8_t _G, uint8_t _R, uint8_t _A) :
	RGB24(_R, _G, _B),
	A(_A)
{}

CSTool::BGRA::BGRA(const RGB24& rgb, uint8_t _A) :
	A(_A),
	RGB24(rgb)
{}


CSTool::YUV CSTool::RGB24toYUV(const RGB24& rgb)
{
	//[0, 255]
	uint8_t Y = 0.299 * rgb.R + 0.587 * rgb.G + 0.114 * rgb.B;
	uint8_t U = -0.169 * rgb.R - 0.331 * rgb.G + 0.5 * rgb.B + 128;
	uint8_t V = 0.5 * rgb.R - 0.419 * rgb.G - 0.081 * rgb.B + 128;
	return YUV(Y, U, V);
}

CSTool::RGB24 CSTool::YUVtoRGB24(const YUV& yuv)
{
	//[0, 255]
	uint8_t R = 1.0 * yuv.Y + 1.403 * (yuv.V - 128);
	uint8_t G = 1.0 * yuv.Y - 0.344 * (yuv.U - 128) - 0.714 * (yuv.V - 128);
	uint8_t B = 1.0 * yuv.Y + 1.773 * (yuv.U - 128);
	return RGB24(R, G, B);
}

CSTool::ColorSpace CSTool::StrToColorSpace(const char* str)
{
	if (str != nullptr)
	{
		if (strcmp(str, "AYUV") == 0)
			return AYUV_CS;
		if (strcmp(str, "VUYA") == 0)
			return VUYA_CS;
		if (strcmp(str, "ARGB") == 0)
			return ARGB_CS;
		if (strcmp(str, "BGRA") == 0)
			return BGRA_CS;
		if (strcmp(str, "RGB24") == 0)
			return RGB24_CS;
	}

	return Undefined_CS;
}

CSTool::Image::Image(const char* inputFileName,
					const char* outputFileName,
					unsigned int Height,
					unsigned int Width,
					const char* color_space) :
	
					input_file_name(nullptr),
					output_file_name(nullptr),
					height(Height),
					width(Width),
					input_CS(StrToColorSpace(color_space))
{
	if (inputFileName != nullptr)
	{
		input_file_name = new char[strlen(inputFileName) + 1]();
		strcpy(input_file_name, inputFileName);
	}

	if (outputFileName != nullptr)
	{
		output_file_name = new char[strlen(outputFileName) + 1]();
		strcpy(output_file_name, outputFileName);
	}
}

void CSTool::Image::setInputFileName(const char* fileName)
{
	delete[] input_file_name;
	input_file_name = new char[strlen(fileName) + 1]();
	strcpy(input_file_name, fileName);
}

const char* CSTool::Image::getInputFileName() const
{
	return input_file_name;
}

void CSTool::Image::setOutputFileName(const char* fileName)
{
	delete[] output_file_name;
	output_file_name = new char[strlen(fileName) + 1]();
	strcpy(output_file_name, fileName);
}

const char* CSTool::Image::getOutputFileName() const
{
	return output_file_name;
}

void CSTool::Image::convertColorSpaceTo(ColorSpace output_CS)
{
	std::fstream input_file(input_file_name, std::ios::in | std::ios::binary);
	std::fstream output_file(output_file_name, std::ios::out | std::ios::binary);

	if (output_CS == Undefined_CS)
	{
		std::cout << "Error! Output color space is undefined." << std::endl;
		system("pause");
		exit(-1);
	}

	if (!input_file.is_open())
	{
		std::cout << "Error! File not found." << std::endl;
		system("pause");
		input_file.close();
		output_file.close();
		exit(-1);
	}

	if (input_CS == RGB24_CS)
	{
		switch (output_CS)
		{
		case BGRA_CS:
			RGB24toBGRA(input_file, output_file);
			break;
		case ARGB_CS:
			RGB24toARGB(input_file, output_file);
			break;
		case VUYA_CS:
			RGB24toVUYA(input_file, output_file);
			break;
		case AYUV_CS:
			RGB24toAYUV(input_file, output_file);
		}
	}
	else if (input_CS == BGRA_CS)
	{
		switch (output_CS)
		{
		case ARGB_CS:
			BGRAtoARGB(input_file, output_file);
			break;
		case VUYA_CS:
			BGRAtoVUYA(input_file, output_file);
			break;
		case AYUV_CS:
			BGRAtoAYUV(input_file, output_file);
			break;
		case RGB24_CS:
			BGRAtoRGB24(input_file, output_file);
		}
	}
	else if (input_CS == ARGB_CS)
	{
		switch (output_CS)
		{
		case VUYA_CS:
			ARGBtoVUYA(input_file, output_file);
			break;
		case AYUV_CS:
			ARGBtoAYUV(input_file, output_file);
			break;
		case BGRA_CS:
			ARGBtoBGRA(input_file, output_file);
			break;
		case RGB24_CS:
			ARGBtoRGB24(input_file, output_file);
		}
	}
	else if (input_CS == VUYA_CS)
	{
		switch (output_CS)
		{
		case AYUV_CS:
			VUYAtoAYUV(input_file, output_file);
			break;
		case ARGB_CS:
			VUYAtoARGB(input_file, output_file);
			break;
		case BGRA_CS:
			VUYAtoARGB(input_file, output_file);
			break;
		case RGB24_CS:
			VUYAtoRGB24(input_file, output_file);
		}
	}
	else if (input_CS == AYUV_CS)
	{
		switch (output_CS)
		{
		case VUYA_CS:
			AYUVtoVUYA(input_file, output_file);
			break;
		case ARGB_CS:
			AYUVtoARGB(input_file, output_file);
			break;
		case BGRA_CS:
			AYUVtoBGRA(input_file, output_file);
			break;
		case RGB24_CS:
			AYUVtoRGB24(input_file, output_file);
		}
	}

	input_file.close();
	output_file.close();
}

CSTool::Image::~Image()
{
	if (input_file_name)
		delete[] input_file_name;
	if (output_file_name)
		delete[] output_file_name;
}

void CSTool::Image::RGB24toBGRA(std::fstream& in_file, std::fstream& out_file)
{
	RGB24 rgb;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> rgb.R >> rgb.G >> rgb.B;
			BGRA bgra(rgb, 255);
			out_file << bgra.B << bgra.G << bgra.R << bgra.A;
		}
	}
}

void CSTool::Image::RGB24toARGB(std::fstream& in_file, std::fstream& out_file)
{
	RGB24 rgb;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> rgb.R >> rgb.G >> rgb.B;
			ARGB bgra(rgb, 255);
			out_file << bgra.A << bgra.R << bgra.G << bgra.B;
		}
	}
}

void CSTool::Image::RGB24toVUYA(std::fstream& in_file, std::fstream& out_file)
{
	RGB24 rgb;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> rgb.R >> rgb.G >> rgb.B;
			VUYA vuya(RGB24toYUV(rgb), 255);
			out_file << vuya.V << vuya.U << vuya.Y << vuya.A;
		}
	}
}

void CSTool::Image::RGB24toAYUV(std::fstream& in_file, std::fstream& out_file)
{
	RGB24 rgb;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> rgb.R >> rgb.G >> rgb.B;
			AYUV ayuv(RGB24toYUV(rgb), 255);
			out_file << ayuv.A << ayuv.Y << ayuv.U << ayuv.V;
		}
	}
}

void CSTool::Image::BGRAtoARGB(std::fstream& in_file, std::fstream& out_file)
{
	BGRA bgra;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> bgra.B >> bgra.G >> bgra.R >> bgra.A;
			ARGB argb(bgra, bgra.A);
			out_file << argb.A << argb.R << argb.G << argb.B;
		}
	}
}

void CSTool::Image::BGRAtoVUYA(std::fstream& in_file, std::fstream& out_file)
{
	BGRA bgra;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> bgra.B >> bgra.G >> bgra.R >> bgra.A;
			VUYA vuya(RGB24toYUV(bgra), bgra.A);
			out_file << vuya.V << vuya.U << vuya.Y << vuya.A;
		}
	}
}

void CSTool::Image::BGRAtoAYUV(std::fstream& in_file, std::fstream& out_file)
{
	BGRA bgra;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> bgra.B >> bgra.G >> bgra.R >> bgra.A;
			AYUV ayuv(RGB24toYUV(bgra), bgra.A);
			out_file << ayuv.A << ayuv.Y << ayuv.U << ayuv.V;
		}
	}
}

void CSTool::Image::ARGBtoVUYA(std::fstream& in_file, std::fstream& out_file)
{
	ARGB argb;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> argb.A >> argb.R >> argb.G >> argb.B;
			VUYA vuya(RGB24toYUV(argb), argb.A);
			out_file << vuya.V << vuya.U << vuya.Y << vuya.A;
		}
	}
}

void CSTool::Image::ARGBtoAYUV(std::fstream& in_file, std::fstream& out_file)
{
	ARGB argb;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> argb.A >> argb.R >> argb.G >> argb.B;
			AYUV vuya(RGB24toYUV(argb), argb.A);
			out_file << vuya.V << vuya.U << vuya.Y << vuya.A;
		}
	}
}

void CSTool::Image::VUYAtoAYUV(std::fstream& in_file, std::fstream& out_file)
{
	VUYA vuya;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> vuya.V >> vuya.U >> vuya.Y >> vuya.A;
			AYUV ayuv(vuya, vuya.A);
			out_file << ayuv.A << ayuv.Y << ayuv.U << ayuv.V;
		}
	}
}

void CSTool::Image::AYUVtoVUYA(std::fstream& in_file, std::fstream& out_file)
{
	AYUV ayuv;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> ayuv.A >> ayuv.Y >> ayuv.U >> ayuv.V;
			VUYA vuya(ayuv, ayuv.A);
			out_file << vuya.V << vuya.U << vuya.Y << vuya.A;
		}
	}
}

void CSTool::Image::AYUVtoARGB(std::fstream& in_file, std::fstream& out_file)
{
	AYUV ayuv;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> ayuv.A >> ayuv.Y >> ayuv.U >> ayuv.V;
			ARGB argb(YUVtoRGB24(ayuv), ayuv.A);
			out_file << argb.A << argb.R << argb.G << argb.B;
		}
	}
}

void CSTool::Image::AYUVtoBGRA(std::fstream& in_file, std::fstream& out_file)
{
	AYUV ayuv;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> ayuv.A >> ayuv.Y >> ayuv.U >> ayuv.V;
			BGRA bgra(YUVtoRGB24(ayuv), ayuv.A);
			out_file << bgra.B << bgra.G << bgra.R << bgra.A;
		}
	}
}

void CSTool::Image::AYUVtoRGB24(std::fstream& in_file, std::fstream& out_file)
{
	RGB24 rgb;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> rgb.R >> rgb.G >> rgb.B;
			AYUV ayuv(RGB24toYUV(rgb), 255);
			out_file << ayuv.A << ayuv.Y << ayuv.U << ayuv.V;
		}
	}
}

void CSTool::Image::VUYAtoARGB(std::fstream& in_file, std::fstream& out_file)
{
	VUYA vuya;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> vuya.V >> vuya.U >> vuya.Y >> vuya.A;
			ARGB argb(YUVtoRGB24(vuya), vuya.A);
			out_file << argb.A << argb.R << argb.G << argb.B;
		}
	}
}

void CSTool::Image::VUYAtoBGRA(std::fstream& in_file, std::fstream& out_file)
{
	VUYA vuya;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> vuya.V >> vuya.U >> vuya.Y >> vuya.A;
			BGRA bgra(YUVtoRGB24(vuya), vuya.A);
			out_file << bgra.B << bgra.G << bgra.R << bgra.A;
		}
	}
}

void CSTool::Image::VUYAtoRGB24(std::fstream& in_file, std::fstream& out_file)
{
	VUYA vuya;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> vuya.V >> vuya.U >> vuya.Y >> vuya.A;
			RGB24 rgb(YUVtoRGB24(vuya));
			out_file << rgb.R << rgb.G << rgb.B;
		}
	}
}

void CSTool::Image::ARGBtoBGRA(std::fstream& in_file, std::fstream& out_file)
{
	ARGB argb;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> argb.A >> argb.R >> argb.G >> argb.B;
			BGRA bgra(argb, argb.A);
			out_file << bgra.B << bgra.G << bgra.R << bgra.A;
		}
	}
}

void CSTool::Image::ARGBtoRGB24(std::fstream& in_file, std::fstream& out_file)
{
	ARGB argb;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> argb.A >> argb.R >> argb.G >> argb.B;
			RGB24 rgb(argb);
			out_file << rgb.R << rgb.G << rgb.B;
		}
	}
}

void CSTool::Image::BGRAtoRGB24(std::fstream& in_file, std::fstream& out_file)
{
	BGRA bgra;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			in_file >> bgra.B >> bgra.G >> bgra.R >> bgra.A;
			RGB24 rgb(bgra);
			out_file << rgb.R << rgb.G << rgb.B;
		}
	}
}