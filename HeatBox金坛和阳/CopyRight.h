// CopyRight.h: interface for the CCopyRight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COPYRIGHT_H__A35C3869_4AF2_47E1_A868_7AC01510158F__INCLUDED_)
#define AFX_COPYRIGHT_H__A35C3869_4AF2_47E1_A868_7AC01510158F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define COPYWRITE_DAYS	1000
class CCopyRight  
{
public:
	void SaveBuildDate(int BoxNr,char *para);
	void SaveProductDate(int BoxNr,char *para);
	void SaveAuthor(int BoxNr,char *para);
	void SaveSN(int BoxNr,char *para);
	bool IsCopyRightOK(void);
	void ReadBuildDate(int BoxNr,CStringArray *SendCmdArray);
	void ReadProductDate(int BoxNr,CStringArray *SendCmdArray);
	void ReadAuthor(int BoxNr,CStringArray *SendCmdArray);
	TCHAR m_ProductDate[MAX_HEAT_BOX][100];
	TCHAR m_BuildDate[MAX_HEAT_BOX][100];
	TCHAR m_Author[MAX_HEAT_BOX][100];
	unsigned int m_SN[MAX_HEAT_BOX];
	bool IsCopyRightCheckTime(void);
	void SetCmdString(CString *cmdstr,int equip, COMMAND_ID cmdId, char para[4]);
	void ReadSN(int BoxNr,CStringArray *SendCmdArray);
	CCopyRight();
	virtual ~CCopyRight();

	int first_time;
	int last_time;
	int cur_time;
};

extern CCopyRight G_CopyRight;

#endif // !defined(AFX_COPYRIGHT_H__A35C3869_4AF2_47E1_A868_7AC01510158F__INCLUDED_)


