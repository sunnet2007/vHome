#include "Utility.h"
#include "Md5.h"
#include "StringFuns.h"

string CUtility::GetSign(string val)
{
	const unsigned char SECRET_KEY[16] = {0x9f, 0x17, 0xf2, 0x89, 0x02, 0x72, 0x48, 0x87, 0x30, 0x46, 0x15, 0x1f, 0x0d, 0x28, 0xbe, 0xf9};
	string key;
	key.assign((const char *)SECRET_KEY, 16);
	string sign = md5(val + key);

	return sign;
}

int CUtility::GetHeader(string head, string name, string & val)
{
	string::size_type p1, p2;
	if((p1 = head.find("\r\n\r\n")) == string::npos)
		return -1;

	if((p1 = head.rfind("\r\n"+name+":", p1)) == string::npos)
		return -1;

	p1 += 2 + name.size() + 1;
	while(head[p1] == ' ')
		p1++;

	if((p2 = head.find("\r\n", p1)) == string::npos)
		return -1;

	val = head.substr(p1, p2-p1);

	return 0;
}

/*
string CUtility::GetMime(const string & fn)
{
	string ext;
	if(fn.rfind(".") != string::npos)
	{
		ext = fn.substr(fn.rfind(".")+1);
	}

	StringLower(ext);

	if(ext.size() == 0)
		return "text/html";

	string mime_set = "|"
		"3gp video/3gpp|"
		"avi video/x-msvideo|"
		"asf video/x-ms-asf|"
		"asn application/astound|"
		"asp application/x-asap|"
		"asx video/x-ms-asf|"
		"au audio/basic|"
		"css text/css|"
		"cab application/vnd.ms-cab-compressed|"
		"dhtml text/html|"
		"doc application/msword|"
		"gif image/gif|"
		"gps application/x-gps|"
		"gtar application/x-gtar|"
		"gz application/x-gzip|"
		"hdf application/x-hdf|"
		"hdm text/x-hdml|"
		"hdml text/x-hdml|"
		"hts text/html|"
		"ifm image/gif|"
		"j2k image/j2k|"
		"jad text/vnd.sun.j2me.app-descriptor|"
		"jam application/x-jam|"
		"jar application/java-archive|"
		"jpe image/jpeg|"
		"jpeg image/jpeg|"
		"jpg image/jpeg|"
		"jpz image/jpeg|"
		"js application/x-javascript|"
		"mov video/quicktime|"
		"movie video/x-sgi-movie|"
		"mp2 audio/mpeg|"
		"mp3 audio/mpeg|"
		"mp4 video/mp4|"
		"mpc application/vnd.mpohun.certificate|"
		"mpe video/mpeg|"
		"mpeg video/mpeg|"
		"mpg video/mpeg|"
		"mpg4 video/mp4|"
		"mpga audio/mpeg|"
		"png image/png|"
		"pbm image/x-portable-bitmap|"
		"pcx image/x-pcx|"
		"pda image/x-pda|"
		"pac audio/x-pac|"
		"pae audio/x-epac|"
		"pdf application/pdf|"
		"ppt application/vnd.ms-powerpoint|"
		"nbmp image/nbmp|"
		"mil image/x-cals|"
		"mio audio/x-mio|"
		"mng video/x-mng|"
		"mod audio/x-mod|"
		"pict image/x-pict|"
		"nsnd audio/nsnd|"
		"pac audio/x-pac|"
		"pae audio/x-epac|"
		"pbm image/x-portable-bitmap|"
		"pcx image/x-pcx|"
		"pda image/x-pda|"
		"sgm text/x-sgml|"
		"sgml text/x-sgml|"
		"si6 image/si6|"
		"si7 image/vnd.stiwap.sis|"
		"si9 image/vnd.lgtwap.sis|"
		"pvx video/x-pv-pvx|"
		"qcp audio/vnd.qcelp|"
		"qt video/quicktime|"
		"qti image/x-quicktime|"
		"qtif image/x-quicktime|"
		"r3t text/vnd.rn-realtext3d|"
		"ra audio/x-pn-realaudio|"
		"ram audio/x-pn-realaudio|"
		"ras image/x-cmu-raster|"
		"rf image/vnd.rn-realflash|"
		"rgb image/x-rgb|"
		"rm application/vnd.rn-realmedia|"
		"rmf audio/x-rmf|"
		"rmm audio/x-pn-realaudio|"
		"rmvb application/vnd.rn-realmedia|"
		"rv video/vnd.rn-realvideo|"
		"s3m audio/x-mod|"
		"s3z audio/x-mod|"
		"svf image/vnd|"
		"svg image/svg-xml|"
		"svh image/svh|"
		"smz audio/x-smd|"
		"snd audio/basic|"
		"smd audio/x-smd|"
		"tif image/tiff|"
		"tiff image/tiff|"
		"z application/x-compress|"
		"zac application/x-zaurus-zac|"
		"zip application/zip|"
		"wav audio/x-wav|"
		"wax audio/x-ms-wax|"
		"wm video/x-ms-wm|"
		"wma audio/x-ms-wma|"
		"wmd application/x-ms-wmd|"
		"wmf application/x-msmetafile|"
		"wml text/vnd.wap.wml|"
		"wmlc application/vnd.wap.wmlc|"
		"wmls text/vnd.wap.wmlscript|"
		"wmlsc application/vnd.wap.wmlscriptc|"
		"wmlscript text/vnd.wap.wmlscript|"
		"wmv audio/x-ms-wmv|"
		"wmx video/x-ms-wmx|"
		"wmz application/x-ms-wmz|"
		"xml text/xml|"
		"xht application/xhtml+xml|"
		"xhtm application/xhtml+xml|"
		"xhtml application/xhtml+xml|"
		"wpng image/x-up-wpng|"
		"rar application/x-rar-compressed|"
		"htm text/html|"
		"html text/html|"
		"hts text/html|"
		"txt text/plain|"
		"log text/plain|"
		"ini text/plain|"
		"conf text/plain|"
		"swf application/x-shockwave-flash|"
		"ico image/x-icon|"
		"xls application/vnd.ms-excel|"
		"xsf text/xml|"
		"xsl text/xml|"
		"xslt text/xml|"
		"flv flv-application/octet-stream|";

	string::size_type p1 = mime_set.find("|" + ext+" ");
	if(p1 == string::npos)
		return "application/octet-stream";
	p1 += ext.size() + 2;

	string::size_type p2 = mime_set.find("|", p1);
	if(p2 == string::npos)
		return "application/octet-stream";

	string mime = mime_set.substr(p1, p2-p1);

	return mime;
}
*/

string CUtility::GetMime(string ext) {
	string mt[][2] = { { "3gp", "video/3gpp" },
			{ "asf", "video/x-ms-asf" },
			{ "asn", "application/astound" },
			{ "asp", "application/x-asap" },
			{ "asx", "video/x-ms-asf" },
			{ "au",  "audio/basic" },
			{ "avi", "video/x-msvideo" },
			{ "amr", "audio/amr"},
			{ "cab", "application/vnd.ms-cab-compressed" },
			{ "conf", "text/plain" },
			{ "css", "text/css" },
			{ "dhtml", "text/html" },
			{ "doc", "application/msword" },
			{ "flv", "video/x-flv" },
			{ "gif", "image/gif" },
			{ "gps", "application/x-gps" },
			{ "gtar", "application/x-gtar" },
			{ "gz", "application/x-gzip" },
			{ "hdf", "application/x-hdf" },
			{ "hdml", "text/x-hdml" },
			{ "hdm", "text/x-hdml" },
			{ "html", "text/html" },
			{ "htm", "text/html" },
			{ "hts", "text/html" },
			{ "hts", "text/html" },
			{ "ico", "image/x-icon" },
			{ "ifm", "image/gif" },
			{ "ini", "text/plain" },
			{ "j2k", "image/j2k" },
			{ "jad", "text/vnd.sun.j2me.app-descriptor" },
			{ "jam", "application/x-jam" },
			{ "jar", "application/java-archive" },
			{ "jpeg", "image/jpeg" },
			{ "jpe", "image/jpeg" },
			{ "jpg", "image/jpeg" },
			{ "jpz", "image/jpeg" },
			{ "js", "application/x-javascript" },
			{ "log", "text/plain" },
			{ "mil", "image/x-cals" },
			{ "mio", "audio/x-mio" },
			{ "mng", "video/x-mng" },
			{ "mod", "audio/x-mod" },
			{ "movie", "video/x-sgi-movie" },
			{ "mov", "video/quicktime" },
			{ "mp2", "audio/mpeg" },
			{ "mp3", "audio/mpeg" },
			{ "mp4", "video/mp4" },
			{ "mpc", "application/vnd.mpohun.certificate" },
			{ "mpeg", "video/mpeg" },
			{ "mpe", "video/mpeg" },
			{ "mpg4", "video/mp4" },
			{ "mpga", "audio/mpeg" },
			{ "mpg", "video/mpeg" },
			{ "nbmp", "image/nbmp" },
			{ "nsnd", "audio/nsnd" },
			{ "pac", "audio/x-pac" },
			{ "pac", "audio/x-pac" },
			{ "pae", "audio/x-epac" },
			{ "pae", "audio/x-epac" },
			{ "pbm", "image/x-portable-bitmap" },
			{ "pbm", "image/x-portable-bitmap" },
			{ "pcx", "image/x-pcx" },
			{ "pcx", "image/x-pcx" },
			{ "pda", "image/x-pda" },
			{ "pda", "image/x-pda" },
			{ "pdf", "application/pdf" },
			{ "pict", "image/x-pict" },
			{ "png", "image/png" },
			{ "ppt", "application/vnd.ms-powerpoint" },
			{ "pvx", "video/x-pv-pvx" },
			{ "qcp", "audio/vnd.qcelp" },
			{ "qtif", "image/x-quicktime" },
			{ "qti", "image/x-quicktime" },
			{ "qt", "video/quicktime" },
			{ "r3t", "text/vnd.rn-realtext3d" },
			{ "ra", "audio/x-pn-realaudio" },
			{ "ram", "audio/x-pn-realaudio" },
			{ "rar", "application/x-rar-compressed" },
			{ "ras", "image/x-cmu-raster" },
			{ "rf", "image/vnd.rn-realflash" },
			{ "rgb", "image/x-rgb" },
			{ "rm", "application/vnd.rn-realmedia" },
			{ "rmf", "audio/x-rmf" },
			{ "rmm", "audio/x-pn-realaudio" },
			{ "rmvb", "application/vnd.rn-realmedia" },
			{ "rv", "video/vnd.rn-realvideo" },
			{ "s3m", "audio/x-mod" },
			{ "s3z", "audio/x-mod" },
			{ "sgml", "text/x-sgml" },
			{ "sgm", "text/x-sgml" },
			{ "si6", "image/si6" },
			{ "si7", "image/vnd.stiwap.sis" },
			{ "si9", "image/vnd.lgtwap.sis" },
			{ "smd", "audio/x-smd" },
			{ "smz", "audio/x-smd" },
			{ "snd", "audio/basic" },
			{ "svf", "image/vnd" },
			{ "svg", "image/svg-xml" },
			{ "svh", "image/svh" },
			{ "swf", "application/x-shockwave-flash" },
			{ "tiff", "image/tiff" },
			{ "tif", "image/tiff" },
			{ "txt", "text/plain" },
			{ "wav", "audio/x-wav" },
			{ "wax", "audio/x-ms-wax" },
			{ "wma", "audio/x-ms-wma" },
			{ "wmd", "application/x-ms-wmd" },
			{ "wmf", "application/x-msmetafile" },
			{ "wmlc", "application/vnd.wap.wmlc" },
			{ "wmlsc", "application/vnd.wap.wmlscriptc" },
			{ "wmlscript", "text/vnd.wap.wmlscript" },
			{ "wmls", "text/vnd.wap.wmlscript" },
			{ "wml", "text/vnd.wap.wml" },
			{ "wmv", "audio/x-ms-wmv" },
			{ "wm", "video/x-ms-wm" },
			{ "wmx", "video/x-ms-wmx" },
			{ "wmz", "application/x-ms-wmz" },
			{ "wpng", "image/x-up-wpng" },
			{ "xht", "application/xhtml+xml" },
			{ "xhtm", "application/xhtml+xml" },
			{ "xhtml", "application/xhtml+xml" },
			{ "xls", "application/vnd.ms-excel" },
			{ "xml", "text/xml" },
			{ "xsf", "text/xml" },
			{ "xsl", "text/xml" },
			{ "xslt", "text/xml" },
			{ "zac", "application/x-zaurus-zac" },
			{ "z", "application/x-compress" },
            {"docm","application/vnd.ms-word.document.macroEnabled.12"},
            {"docx","application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
            {"dotm","application/vnd.ms-word.template.macroEnabled.12"},
            {"dotx","application/vnd.openxmlformats-officedocument.wordprocessingml.template"},
            {"potm","application/vnd.ms-powerpoint.template.macroEnabled.12"},
            {"potx","application/vnd.openxmlformats-officedocument.presentationml.template"},
            {"ppam","application/vnd.ms-powerpoint.addin.macroEnabled.12"},
            {"ppsm","application/vnd.ms-powerpoint.slideshow.macroEnabled.12"},
            {"ppsx","application/vnd.openxmlformats-officedocument.presentationml.slideshow"},
            {"pptm","application/vnd.ms-powerpoint.presentation.macroEnabled.12"},
            {"pptx","application/vnd.openxmlformats-officedocument.presentationml.presentation"},
            {"xlam","application/vnd.ms-excel.addin.macroEnabled.12"},
            {"xlsb","application/vnd.ms-excel.sheet.binary.macroEnabled.12"},
            {"xlsm","application/vnd.ms-excel.sheet.macroEnabled.12"},
            {"xlsx","application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
            {"xltm","application/vnd.ms-excel.template.macroEnabled.12"},
            {"xltx","application/vnd.openxmlformats-officedocument.spreadsheetml.template"},
			{ "zip", "application/zip" } };
	
	if(ext.find("?") != string::npos) {
		ext = ext.substr(0, ext.find("?"));
	}
	if(ext.rfind(".") != string::npos) {
		ext = ext.substr(ext.rfind(".") + 1);
	}
	StringLower(ext);
	
	int i;
	for(i = 0; i < 145; i++) {
		if(mt[i][0] == ext) {
			return mt[i][1];
		}
	}
	
	return "application/octet-stream"; 
}
