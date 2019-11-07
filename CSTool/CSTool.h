#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>

//All color space values between 0 and 255

namespace CSTool
{
	enum ColorSpace
	{
		AYUV_CS,
		VUYA_CS,
		ARGB_CS,
		BGRA_CS,
		RGB24_CS,
		Undefined_CS
	};

	struct YUV
	{
		uint8_t Y;
		uint8_t U;
		uint8_t V;

		YUV(uint8_t _Y = 0, uint8_t _U = 0, uint8_t _V = 0);
	};

	struct AYUV : public YUV
	{
		uint8_t A;

		AYUV(uint8_t _A = 0, uint8_t _Y = 0, uint8_t _U = 0, uint8_t _V = 0);

		AYUV(const YUV& yuv, uint8_t _A = 255);
	};

	struct VUYA : public YUV
	{
		uint8_t A;

		VUYA(uint8_t _V = 0, uint8_t _U = 0, uint8_t _Y = 0, uint8_t _A = 0);

		VUYA(const YUV& yuv, uint8_t _A = 255);
	};

	struct RGB24
	{
		uint8_t R;
		uint8_t G;
		uint8_t B;

		RGB24(uint8_t _R = 0, uint8_t _G = 0, uint8_t _B = 0);
	};

	struct ARGB : public RGB24
	{
		uint8_t A;

		ARGB(uint8_t _A = 0, uint8_t _R = 0, uint8_t _G = 0, uint8_t _B = 0);

		ARGB(const RGB24& rgb, uint8_t _A = 255);
	};

	struct BGRA : public RGB24
	{
		uint8_t A;

		BGRA(uint8_t _B = 0, uint8_t _G = 0, uint8_t _R = 0, uint8_t _A = 0);

		BGRA(const RGB24& rgb, uint8_t _A = 255);
	};

	YUV RGB24toYUV(const RGB24& rgb);

	RGB24 YUVtoRGB24(const YUV& yuv);

	ColorSpace StrToColorSpace(const char* str);

	class Image
	{
	public:

		unsigned int height;
		unsigned int width;
		ColorSpace input_CS;

		Image(const char* inputFileName = nullptr, 
			const char* outputFileName = nullptr, 
			unsigned int Height = 0, 
			unsigned int Width = 0, 
			const char* color_space = nullptr);

		void setInputFileName(const char* fileName);

		const char* getInputFileName() const;

		void setOutputFileName(const char* fileName);

		const char* getOutputFileName() const;

		void convertColorSpaceTo(ColorSpace output_CS);

		~Image();

	private:

		char* input_file_name;
		char* output_file_name;

		void RGB24toBGRA(std::fstream& in_file, std::fstream& out_file);

		void RGB24toARGB(std::fstream& in_file, std::fstream& out_file);

		void RGB24toVUYA(std::fstream& in_file, std::fstream& out_file);

		void RGB24toAYUV(std::fstream& in_file, std::fstream& out_file);

		void BGRAtoARGB(std::fstream& in_file, std::fstream& out_file);

		void BGRAtoVUYA(std::fstream& in_file, std::fstream& out_file);

		void BGRAtoAYUV(std::fstream& in_file, std::fstream& out_file);

		void ARGBtoVUYA(std::fstream& in_file, std::fstream& out_file);

		void ARGBtoAYUV(std::fstream& in_file, std::fstream& out_file);

		void VUYAtoAYUV(std::fstream& in_file, std::fstream& out_file);

		void AYUVtoVUYA(std::fstream& in_file, std::fstream& out_file);

		void AYUVtoARGB(std::fstream& in_file, std::fstream& out_file);

		void AYUVtoBGRA(std::fstream& in_file, std::fstream& out_file);

		void AYUVtoRGB24(std::fstream& in_file, std::fstream& out_file);

		void VUYAtoARGB(std::fstream& in_file, std::fstream& out_file);

		void VUYAtoBGRA(std::fstream& in_file, std::fstream& out_file);

		void VUYAtoRGB24(std::fstream& in_file, std::fstream& out_file);

		void ARGBtoBGRA(std::fstream& in_file, std::fstream& out_file);

		void ARGBtoRGB24(std::fstream& in_file, std::fstream& out_file);

		void BGRAtoRGB24(std::fstream& in_file, std::fstream& out_file);
	};
}