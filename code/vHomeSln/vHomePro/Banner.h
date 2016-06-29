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
	string m_strLink;// Ìø×ªµØÖ·
	string m_strImgThumb; // Í¼Æ¬µØÖ·
};

