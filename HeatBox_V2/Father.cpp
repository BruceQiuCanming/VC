#include "stdafx.h"
#include "Father.h"

Father::Father(void)
{
}

Father::~Father(void)
{
}

void Father::say(void)
{
	AfxMessageBox(_T("Father::say"));
}

void Father::mydo(void)
{
	say();
}