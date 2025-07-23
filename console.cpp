//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <iomanip>
#include "console.h"
#include "engine.h"

#include "skin.h"
#include "clipboard.h"

#include "gm_gameworld.h"
#include "gamecontent/creature.h"

extern yatcClipboard g_clipboard;
extern int g_lastmousebutton;

ConsolePanel::ConsolePanel()
{
    AddObject(&pnlConsoleResizer);
    pnlConsoleResizer.SetSkin(&g_skin.btnn[0]);
    pnlConsoleResizer.SetHeight(5);
    pnlConsoleResizer.SetPos(0, 0);
    #if (GLICT_APIREV >= 98)
    pnlConsoleResizer.SetDraggable(true);
    pnlConsoleResizer.SetFocusable(true);
    #endif

	AddObject(&pnlConsoleEntryContainer);
    pnlConsoleEntryContainer.SetSkin(&g_skin.rsp);
    pnlConsoleEntryContainer.SetPos(0, 21);

    AddObject(&pnlConsoleButtonContainer);
    pnlConsoleButtonContainer.SetSkin(&g_skin.consoletabbg);
    pnlConsoleButtonContainer.SetPos(0,5);
    pnlConsoleButtonContainer.SetHeight(18);
    //pnlConsoleButtonContainer.SetBGActiveness(false);

    pnlConsoleEntryContainer.AddObject(&pnlConsoleEntryView);
    pnlConsoleEntryView.SetSkin(&g_skin.ptb);
    pnlConsoleEntryView.SetPos(5, 5);
    pnlConsoleEntryView.SetOnPaint(onPaintConsole);
    pnlConsoleEntryView.SetOnClick(onClickConsole);

    pnlConsoleEntryContainer.AddObject(&txtConsoleEntry);
	txtConsoleEntry.SetHeight(12);
	txtConsoleEntry.SetCaption("");

	pnlConsoleEntryContainer.AddObject(&btnSpeakLevel);
	btnSpeakLevel.SetWidth(16);
	btnSpeakLevel.SetHeight(16);
	switch(options.speaktype)
	{
		case SPEAK_WHISPER:
			btnSpeakLevel.SetSkin(&g_skin.graphicbtn[BUTTON_CONSOLE_WHISPER]);
			btnSpeakLevel.SetHighlightSkin(&g_skin.graphicbth[BUTTON_CONSOLE_WHISPER]);
			break;
		case SPEAK_SAY:
			btnSpeakLevel.SetSkin(&g_skin.graphicbtn[BUTTON_CONSOLE_SPEAK]);
			btnSpeakLevel.SetHighlightSkin(&g_skin.graphicbth[BUTTON_CONSOLE_SPEAK]);
			break;
		case SPEAK_YELL: default:
			btnSpeakLevel.SetSkin(&g_skin.graphicbtn[BUTTON_CONSOLE_YELL]);
			btnSpeakLevel.SetHighlightSkin(&g_skin.graphicbth[BUTTON_CONSOLE_YELL]);
			break;
	}
	btnSpeakLevel.SetCaption("");
	btnSpeakLevel.SetOnClick(btnSpeak_OnClick);

	pnlConsoleButtonContainer.AddObject(&btnIgnore);
	btnIgnore.SetWidth(16);
	btnIgnore.SetHeight(16);
	btnIgnore.SetSkin(&g_skin.graphicbtn[BUTTON_CONSOLE_IGNORELIST]);
	btnIgnore.SetHighlightSkin(&g_skin.graphicbth[BUTTON_CONSOLE_IGNORELIST]);
	btnIgnore.SetCaption("");
	//btnIgnore.SetOnClick(btnIgnoreOnClick);
	pnlConsoleButtonContainer.AddObject(&btnChannelList);
	btnChannelList.SetWidth(16);
	btnChannelList.SetHeight(16);
	btnChannelList.SetSkin(&g_skin.graphicbtn[BUTTON_CONSOLE_CHANNELS]);
	btnChannelList.SetHighlightSkin(&g_skin.graphicbth[BUTTON_CONSOLE_CHANNELS]);
	btnChannelList.SetCaption("");
	//btnChannelList.SetOnClick(btnChannelListOnClick);
	pnlConsoleButtonContainer.AddObject(&btnClose);
	btnClose.SetWidth(16);
	btnClose.SetHeight(16);
	btnClose.SetSkin(&g_skin.graphicbtn[BUTTON_CONSOLE_CLOSE]);
	btnClose.SetHighlightSkin(&g_skin.graphicbth[BUTTON_CONSOLE_CLOSE]);
	btnClose.SetCaption("");
	//btnClose.SetOnClick(btnChannelCloseOnClick);
	pnlConsoleButtonContainer.AddObject(&btnM);
	btnM.SetWidth(16);
	btnM.SetHeight(16);
	btnM.SetSkin(&g_skin.graphicbtn[BUTTON_CONSOLE_M]);
	btnM.SetHighlightSkin(&g_skin.graphicbth[BUTTON_CONSOLE_M]);
	btnM.SetCaption("");
	//btnM.SetOnClick(btnMOnClick);
}

ConsolePanel::~ConsolePanel()
{
}

bool ConsolePanel::CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue)
{
#if (GLICT_APIREV>=95)
    if (evt == GLICT_MOUSEMOVE) {
        if (draggedchild) {
            glictPos pos = *(glictPos*)wparam;
            SetHeight(GetHeight() - (pos.y - GetY()));
            SetPos(GetX(), glictGlobals.h - GetHeight());
            // TODO (nfries88): update game area size.
            g_game->doResize(glictGlobals.w, glictGlobals.h);
            return true;
        }
    }
    else if (evt == GLICT_MOUSEUP) {
        if (draggedchild){
            StopDraggingChild(*(glictPos*)wparam);
            return true;
        }
    }
#endif
    return glictPanel::CastEvent(evt,wparam,lparam,returnvalue);
}

void ConsolePanel::SetHeight(float h)
{
    // NOTE (nfries88): The purpose of this is to prevent a 0 height console and confusing people on how to restore it.
    //      and also prevent from shrinking the game area into oblivion.
    h = std::min(std::max(h, 200.f), glictGlobals.h - 352);
	glictPanel::SetHeight(h);

	pnlConsoleEntryContainer.SetHeight(GetHeight()-21);
	pnlConsoleEntryView.SetHeight(GetHeight()-45);
	txtConsoleEntry.SetPos(20,GetHeight()-38);
	btnSpeakLevel.SetPos(2, GetHeight()-39);

	options.consoleh = h;
}
void ConsolePanel::SetWidth(float w)
{
	glictPanel::SetWidth(w);

    pnlConsoleResizer.SetWidth(GetWidth());
	pnlConsoleEntryContainer.SetWidth(GetWidth());
    pnlConsoleButtonContainer.SetWidth(GetWidth());
    btnIgnore.SetPos(pnlConsoleButtonContainer.GetWidth()-16, 0);
    btnChannelList.SetPos(pnlConsoleButtonContainer.GetWidth()-32, 0);
    btnClose.SetPos(pnlConsoleButtonContainer.GetWidth()-48, 0);
    btnM.SetPos(pnlConsoleButtonContainer.GetWidth()-64, 0);
    pnlConsoleEntryView.SetWidth(GetWidth()-10);

    txtConsoleEntry.SetWidth(GetWidth()-20-7);
}

void ConsolePanel::SetActiveConsole(Console* console)
{
	pnlConsoleEntryView.SetCustomData(console);
    console->getAssignedButton()->SetSkin(&g_skin.consoletabactive);
    #if (GLICT_APIREV >= 85)
    console->getAssignedButton()->SetCaptionColor(0.7,0.7,0.7);
    #else
	#warning No support for setcaptioncolor before glict apirev 85
	#endif
	if(!console->getSpeakerName().length() && ((console->getAssignedButton()->GetCaption() == "Default") || (console->getAssignedButton()->GetCaption() == "Server Log")))
	{
		btnSpeakLevel.SetVisible(true);
		btnClose.SetVisible(false);
		btnM.SetVisible(false);
	}
	else
	{
		btnSpeakLevel.SetVisible(false);
		btnClose.SetVisible(true);
		btnM.SetVisible(true);
	}
}

void ConsolePanel::MakeConsole(Console* console, const std::string& name)
{
	glictPanel* p = new glictPanel;
    console->setAssignedButton(p);
    p->SetCustomData(console);
    p->SetHeight(18);

    // NOTE (kilouco): Should shorten names which size exceed 66px.
    int real_size = (int)g_engine->sizeText(name.c_str(),"gamefont");
    int caption_size = real_size;
    std::string caption_name = name;

    while (caption_size > 66) {
        caption_name.erase(caption_name.end()-1);
        caption_size = (int)g_engine->sizeText(caption_name.c_str(),"gamefont");
    }

    if (real_size != caption_size)
        caption_name.insert(caption_name.end(),3,'.');

    p->SetCaption(caption_name.c_str());
    //p->SetCaption(name.c_str());
    p->SetWidth(96); //g_engine->sizeText(s.str().c_str(),"system"));
    p->SetBGColor(.2,.2,.2,1.);
    #if (GLICT_APIREV >= 85)
    p->SetTextOffset(int(96 / 2 - g_engine->sizeText(caption_name.c_str(),"gamefont") / 2), 4);
    p->SetCaptionColor(0.5,0.5,0.5);
    #else
	#warning No support for setcaptioncolor before glict apirev 85
	#endif
	// note (nfries88): Start at 20px offset to make appearance more like official client.
    int sum=20;
    for (std::vector<glictPanel*>::iterator it = pnlConsoleButtons.begin(); it != pnlConsoleButtons.end(); it++) {
        (*it)->SetPos(sum,5);
        sum += (int)(*it)->GetWidth();
    }
    p->SetPos(sum,5);
    p->SetOnClick(pnlConsoleButton_OnClick);
    p->SetSkin(&g_skin.consoletabpassive);
    p->SetFont("gamefont");
    AddObject(p);
    pnlConsoleButtons.push_back(p);
}

void ConsolePanel::DeleteConsole(Console* console)
{
	std::vector<glictPanel*>::iterator it = std::find(pnlConsoleButtons.begin(), pnlConsoleButtons.end(), console->getAssignedButton());
	if(it != pnlConsoleButtons.end()){
		pnlConsoleButtons.erase(it);
		console->getAssignedButton()->SetVisible(false);
		pnlConsoleButtonContainer.RemoveObject(console->getAssignedButton());
		console->clearEntries();
		// FIXME (nfries88): This is a memory leak, need to delete getAssignedButton at a later time.
		//delete console->getAssignedButton();
		delete console;
	}
}

void ConsolePanel::pnlConsoleButton_OnClick(glictPos* relmousepos, glictContainer* caller)
{
    Console* c = (Console*)caller->GetCustomData();
    GM_Gameworld* gw = (GM_Gameworld*)g_game;

    if (g_lastmousebutton == SDL_BUTTON_RIGHT){
        gw->performPopup(makeConsoleBtnPopup, NULL, (void*) c);
    }
    gw->setActiveConsole(c);
}

void ConsolePanel::onPaintConsole(glictRect* real, glictRect* clipped, glictContainer* callerclass)
{
	Console* console = (Console*)(callerclass->GetCustomData());
	if(console){
		console->paintConsole(real->left+4, real->top+2, real->right-4, real->bottom-2);
	}
}

void ConsolePanel::onMessageTo(Popup::Item *parent)
{
	Creature* c = Creatures::getInstance().getCreature((uint32_t)VOIDP2INT(parent->data));
	if(c != NULL){
		GM_Gameworld *gw = (GM_Gameworld*)g_game;
		gw->setActiveConsole(gw->findConsole(c->getName()));
	}
}

void ConsolePanel::onCopyMessage(Popup::Item *parent)
{
	ConsoleEntry* e = (ConsoleEntry*)parent->data;
	g_clipboard.setText(e->getFullText());
}

void ConsolePanel::onUnimplemented(Popup::Item *parent)
{
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
    gw->msgBox(gettext("This functionality is not yet finished"),"TODO");
}

void ConsolePanel::onCloseConsole(Popup::Item *parent)
{
	Console* c = (Console*)parent->data;
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
	gw->removeConsole(c);
}
void ConsolePanel::onShowM(Popup::Item *parent)
// TODO (kilouco): Show Server Messages in any console where this button is turned on.
{
/*
	Console* c = (Console*)parent->data;
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
*/
// commented out by ivucica to fix warning

}
void ConsolePanel::onSaveConsole(Popup::Item *parent)
{
	Console* c = (Console*)parent->data;
	c->dumpText();
	std::stringstream ss;
	ss << "Channel " << c->getAssignedButton()->GetCaption() << " appended to '" << c->getAssignedButton()->GetCaption() << ".txt'";
	ConsoleEntry ce(ss.str());
	c->insertEntry(ce);
}
void ConsolePanel::onClearConsole(Popup::Item *parent)
{
	Console* c = (Console*)parent->data;
	c->clearEntries();
}

void ConsolePanel::makeConsoleBtnPopup(Popup* popup, void* owner, void* arg)
{
	Console* c = (Console*)arg;

	std::stringstream s;

	if(c->getSpeakerName().length() || (c->getAssignedButton()->GetCaption() != "Default"))
	{
		s.str("");
		s << gettext("Close");
		popup->addItem(s.str(), onCloseConsole, c);

		s.str("");
		s << gettext("Show server messages");
		popup->addItem(s.str(), onShowM, c);

		popup->addItem("-",NULL,NULL);
	}

	s.str("");
	s << gettext("Save Window");
	popup->addItem(s.str(), onSaveConsole, c);

	s.str("");
	s << gettext("Clear Window");
	popup->addItem(s.str(), onClearConsole, c);
}

void ConsolePanel::makeConsolePopup(Popup* popup, void* owner, void* arg)
{
	ConsoleEntry* e = (ConsoleEntry*)arg;
	std::string speaker = e->getSpeaker();

	std::stringstream s;
	if(speaker.length() != 0)
	{
		Creature* c = Creatures::getInstance().lookup(speaker);
		if(c != NULL)
		{
			if(c->isPlayer()){
				s.str("");
				s << gettext("Message to") << " " << speaker;
				popup->addItem(s.str(), onMessageTo, (void*)c->getID());

				s.str("");
				s << gettext("Add to VIP list");
				popup->addItem(s.str(), onUnimplemented, (void*)c->getID());

				s.str("");
				s << gettext("Ignore") << " " << speaker;
				popup->addItem(s.str(), onUnimplemented, (void*)c->getID());


				popup->addItem("-",NULL,NULL);
			}

			s.str("");
			s << gettext("Copy Name");
			popup->addItem(s.str(), GM_Gameworld::onCopyName, (void*)c->getID());
		}
	}
	s.str("");
	s << gettext("Copy Message");
	popup->addItem(s.str(), onCopyMessage, e);
	popup->addItem("-",NULL,NULL);

	s.str("");
	s << gettext("Select all");
	popup->addItem(s.str(), onUnimplemented /*onSelectAll*/);
}

void ConsolePanel::onClickConsole(glictPos* relmousepos, glictContainer* callerclass)
{
	if(g_lastmousebutton != SDL_BUTTON_RIGHT) return;

	Console* console = (Console*)(callerclass->GetCustomData());
	if(console != NULL)
	{
		ConsoleEntry* e = console->getConsoleEntryAt(relmousepos->x, callerclass->GetHeight() - relmousepos->y);
		if(e != NULL)
			((GM_Gameworld*)g_game)->performPopup(makeConsolePopup, NULL, (void*) e);
	}
}

void ConsolePanel::btnSpeak_OnClick(glictPos* relmousepos, glictContainer* caller)
{
	glictButton* btnSpeak = (glictButton*)(caller);
	switch(options.speaktype)
	{
		case SPEAK_YELL:
			options.speaktype = SPEAK_WHISPER;
			btnSpeak->SetSkin(&g_skin.graphicbtn[BUTTON_CONSOLE_WHISPER]);
			btnSpeak->SetHighlightSkin(&g_skin.graphicbth[BUTTON_CONSOLE_WHISPER]);
			break;
		case SPEAK_WHISPER:
			options.speaktype = SPEAK_SAY;
			btnSpeak->SetSkin(&g_skin.graphicbtn[BUTTON_CONSOLE_SPEAK]);
			btnSpeak->SetHighlightSkin(&g_skin.graphicbth[BUTTON_CONSOLE_SPEAK]);
			break;
		case SPEAK_SAY: default:
			options.speaktype = SPEAK_YELL;
			btnSpeak->SetSkin(&g_skin.graphicbtn[BUTTON_CONSOLE_YELL]);
			btnSpeak->SetHighlightSkin(&g_skin.graphicbth[BUTTON_CONSOLE_YELL]);
			break;
	}
}

int ConsoleEntry::paintEntry(float x, float y, float width /*= -1*/)
{
	// TODO (ivucica#3#) add word wrapping
	// TODO (kilouco): limit character input to 255.
	std::string fulltext = getFullText();

    // The code below may seem a little confusing, but it WORKS, HAHAHA!!!
	std::string message, old_line_message, new_line_message;
	message = new_line_message = old_line_message = fulltext;

	int max_width = (glictGlobals.w-172-4) - 20;
	int text_size = (int)g_engine->sizeText(message.c_str(),"aafont");
	int line_size = text_size;

	int linecount = 1;

	std::stringstream final_text;

	size_t pos;

    if (line_size > max_width) {
        while (line_size > max_width) {
            old_line_message.erase(old_line_message.end()-1);
            line_size = (int)g_engine->sizeText(old_line_message.c_str(),"aafont");

            if (line_size <= max_width) {
                pos = old_line_message.find_last_of(" ");

                if(pos == std::string::npos)
                    pos = old_line_message.length();

                old_line_message.resize(pos);

                new_line_message = new_line_message.substr(pos);

                if (linecount <= 1)
                    final_text << old_line_message << "\n";
                else
                    final_text << "  " << old_line_message << "\n";

                linecount++;

                old_line_message = new_line_message;
                line_size = (int)g_engine->sizeText(new_line_message.c_str(),"aafont");

                if (line_size <= max_width)
                    final_text << "   " << new_line_message;// << new_line_message;
            }
        }

        fulltext = final_text.str();
    }

	g_engine->drawText(fulltext.c_str(), "aafont", (int)x, (int)(y - glictFontNumberOfLines(fulltext.c_str())*12), m_color);
	return glictFontNumberOfLines(m_text.c_str())* (12 * linecount);
}

std::string ConsoleEntry::getFullText() const
{
	std::stringstream ss;
	if(options.timestamps) {
		struct tm* timeinfo = localtime((time_t*)(&m_timestamp));
		ss << setfill('0') << setw(2) << timeinfo->tm_hour << ":" << setfill('0') << setw(2) << timeinfo->tm_min << " ";
	}
	if(m_speaker.size()){
		ss << m_speaker;

		if(options.levels && m_level > 0){
			ss << " [" << m_level << "]";
		}
		ss << ": ";
	}

	ss << m_text;

	return ss.str();
}

int ConsoleEntry::getHeight()
{
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
    g_engine->setClipping(int(left),int(top),int(right-left),int(bottom-top));

	float x, y;
	x = left; y = bottom;

	for (std::vector<ConsoleEntry>::reverse_iterator it=m_content.rbegin(); it!=m_content.rend(); it++) {
		y -= it->paintEntry(x, y);
		if (y < top)
			break;
	}
	g_engine->resetClipping();

}
ConsoleEntry* Console::getConsoleEntryAt(float relx, float rely)
{
	float y = 0;

	for (std::vector<ConsoleEntry>::reverse_iterator it=m_content.rbegin(); it!=m_content.rend(); it++) {
		if (rely > y && rely < y + it->getHeight())
			return &(*it);
		y += it->getHeight();
	}
	return NULL;
}
void Console::insertEntry(ConsoleEntry ce) {
	m_content.insert(m_content.end(), ce);
}
void Console::clearEntries() {
	m_content.clear();
}
void Console::dumpText()
{
	std::stringstream ss;
	ss << getAssignedButton()->GetCaption() << ".txt";
	FILE* f = fopen(ss.str().c_str(), "wt+");
	if(!f) return;
	for (std::vector<ConsoleEntry>::iterator it=m_content.begin(); it!=m_content.end(); it++) {
		fprintf(f, "%s\n", (*it).getFullText().c_str());
	}
	fclose(f);
}
int Console::getHeight()
{
    // TODO (nfries88): use this to adjust the starting height for paintConsole and getConsoleEntryAt;
        //
    int height = 0;
    for (std::vector<ConsoleEntry>::reverse_iterator it=m_content.rbegin(); it!=m_content.rend(); it++) {
		height += (*it).getHeight();
	}
    return height;
}
