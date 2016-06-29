#include "Banner.h"


CBanner::CBanner(void)
{
	m_strLink = "";
	m_strImgThumb = "";
}


CBanner::~CBanner(void)
{
}

void CBanner::SetLink(string strLink)
{
	m_strLink = strLink;
}
void CBanner::SetImgThumb(string strImgThumb)
{
	m_strImgThumb = strImgThumb;
}

string CBanner::GetLink()
{
	return m_strLink;
}
string CBanner::GetImgThumb()
{
	return m_strImgThumb;
}