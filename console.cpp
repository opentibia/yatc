#include "console.h"
#include "engine.h"
int ConsoleEntry::Paint(float x, float y)
{
	// TODO (ivucica#3#) add word wrapping
	g_engine->drawText( (m_speaker + (m_speaker.size() ? ": " : "") + m_text).c_str(), "aafont", (int)x, (int)(y - glictFontNumberOfLines(m_text.c_str())*12), 0);
	return glictFontNumberOfLines(m_text.c_str())*12;
}


Console::Console()
{

}
Console::~Console()
{
}
void Console::Paint(float left, float top, float right, float bottom)
{
	float x, y;
	x = left; y = bottom;
	for (std::vector<ConsoleEntry>::reverse_iterator it=m_content.rbegin(); it!=m_content.rend(); it++) {
		y -= it->Paint(x, y);
		if (y < top)
			break;
	}

}
void Console::Insert(ConsoleEntry ce) {
	m_content.insert(m_content.end(), ce);
}
