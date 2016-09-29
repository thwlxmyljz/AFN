#include "Water.h"
#include "LogFileu.h"
#include "YQUtils.h"
#include "Lock.h"
#include "Log.h"
#include <sstream>

//------------------------------------------------------------------------------------
Water::Water()
	:m_name(""),
	m_heartTimer(0),
	m_getTimer(0)
{
}
Water::Water(string _name)
	:m_name(_name),
	m_heartTimer(0),
	m_getTimer(0)
{
}
Water::~Water()
{
}
BOOL Water::operator==(const Water& o)
{
	return (m_name == o.m_name);
}
std::string Water::printInfo()
{
	std::ostringstream os;
	os <<"no ele";
	return os.str();
}
BOOL Water::CheckTimeout()
{
	DWORD dwElapse = TYQUtils::TimeElapse(m_heartTimer);
	if ( dwElapse > 180000/*3分钟，3次heartbeart*/)
	{
		TYQUtils::TimeStart(m_heartTimer);
		ostringstream os;
		os << m_name << " timeout, from " << m_heartTimer << " + " << dwElapse;			
		YQLogInfo(os.str().c_str());
		return TRUE;
	}
	return FALSE;
}
