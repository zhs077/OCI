
#include <cassert>
#include <string>
#include<vector>
#include<cstring>
using namespace std;


#ifndef ENCODE_CONV_H_
#define ENCODE_CONV_H_

namespace encodeConv
{

	class CodingConv
	{
	public:
		static string Unicode2Utf8(const wchar_t* pUnicode);
		static wstring Utf82Unicode(const char* pUtf8);
		static wstring s2ws(const string&s);
		static string ws2s(const wstring ws);
		static string ascii2Utf8(const char* pAscii);
		static string utf82Ascii(const char* pUtf8);
		static string urlEncode(const wchar_t* pUnicode);
		static wstring urlDecode(const wchar_t* pUnoode);
	private:

		static int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput,int outSize);
		static int my_utf8_to_unicode(vector<unsigned char>& strUnicode, unsigned char* utf8, int len);


	};


	

};


#endif
