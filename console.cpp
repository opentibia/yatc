#include "console.h"
#include "engine.h"
int ConsoleEntry::paintEntry(float x, float y)
{
	// TODO (ivucica#3#) add word wrapping
	g_engine->drawText( (m_speaker + (m_speaker.size() ? ": " : "") + m_text).c_str(), "aafont", (int)x, (int)(y - glictFontNumberOfLines(m_text.c_str())*12), 0);
	return glictFontNumberOfLines(m_text.c_str())*12;
}

static int consolecount=0;

Console::Console()
{
    m_channelid = 0;
    m_consoleid = (consolecount++);
}
Console::Console(uint32_t channelid)
{
    m_channelid = channelid;
    m_consoleid = (consolecount++);
}
Console::Console(std::string speakername)
{
    m_speakername = speakername;
    m_channelid = 0;
    m_consoleid = (consolecount++);
}

Console::~Console()
{
}
void Console::paintConsole(float left, float top, float right, float bottom)
{
	float x, y;
	x = left; y = bottom;
	for (std::vector<ConsoleEntry>::reverse_iterator it=m_content.rbegin(); it!=m_content.rend(); it++) {
		y -= it->paintEntry(x, y);
		if (y < top)
			break;
	}

}
void Console::insertEntry(ConsoleEntry ce) {
	m_content.insert(m_content.end(), ce);
}
