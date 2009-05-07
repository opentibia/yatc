#include "statusmsg.h"
#include "engine.h"

StatusMsg::StatusMsg()
{
    m_timeRemaining = 0;
    m_textColor = TEXTCOLOR_WHITE;
    m_messageText = "";
    m_xOffset = m_yOffset = 0;
    m_hAlign = ALIGN_LEFT;
    m_vAlign = ALIGN_TOP;
}
StatusMsg::StatusMsg(TextColor_t color, const std::string& message, double timeout, double xoffset, double yoffset, HAlignment halign, VAlignment valign)
{
    m_timeRemaining = timeout;
    m_textColor = color;
    m_messageText = message;
    m_xOffset = xoffset;
    m_yOffset = yoffset;
    m_hAlign = halign;
    m_vAlign = valign;
}

void StatusMsg::paintSelf(int windowX, int windowY, int windowW, int windowH)
{
    double x=0, y=0;

    if (m_timeRemaining <= 0)
        return;
    switch (m_hAlign)
    {
        case ALIGN_LEFT:
            x=0;
            break;
        case ALIGN_CENTER:
            x  = windowW / 2;
            x -= (g_engine->sizeText(m_messageText.c_str(), "gamefont" ) / 2);
            break;
        case ALIGN_RIGHT:
            x  = windowW;
            x -= (g_engine->sizeText(m_messageText.c_str(), "gamefont" ) );
            break;
    }
    x += m_xOffset;

    switch (m_vAlign)
    {
        case ALIGN_TOP:
            y=0;
            break;
        case ALIGN_MIDDLE:
            y  = windowH / 2;
            y -= 12 / 2;
            break;
        case ALIGN_BOTTOM:
            y  = windowH - 12;
            break;
    }
    y += m_yOffset;

    g_engine->drawText(m_messageText.c_str(),"gamefont",x,y,m_textColor);
}

void StatusMsg::updateSelf(double k)
{
    m_timeRemaining -= k;
}
