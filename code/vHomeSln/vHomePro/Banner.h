#pragma once
#include <string>
using namespace std;

class CBanner
{
public:
	CBanner(void);
	~CBanner(void);

public:
	void SetLink(string strLink);
	void SetImgThumb(string strImgThumb);

	string GetLink();
	string GetImgThumb();

private:
	string m_strLink;// ��ת��ַ
	string m_strImgThumb; // ͼƬ��ַ
};

