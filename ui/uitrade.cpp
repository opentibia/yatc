
#include "uitrade.h"
#include "../gm_gameworld.h"
#include "../net/protocolgame.h"
#include "../skin.h"

winTrade_t::winTrade_t()
{
	window.SetCaption("Trade");
	window.SetWidth(160);
	window.SetHeight(115);

	#if (GLICT_APIREV >= 76)
	window.AddTitlebarObject(&btnClose);
	btnClose.SetCaption("x");
	btnClose.SetWidth(12);
	btnClose.SetHeight(12);
	btnClose.SetPos(160 - 12, 0);
	btnClose.SetCustomData(this);
	btnClose.SetOnClick(tradeOnReject);

	window.AddTitlebarObject(&btnCollapse);
	btnCollapse.SetCaption("-");
	btnCollapse.SetWidth(12);
	btnCollapse.SetHeight(12);
	btnCollapse.SetPos(160 - 24, 0);
	btnCollapse.SetOnClick(onCollapse);

	window.AddTitlebarObject(&pnlIcon);
	pnlIcon.SetWidth(12);
	pnlIcon.SetHeight(12);
	pnlIcon.SetPos(2, 2);
	pnlIcon.SetOnPaint(iconOnPaint);
	#endif

	window.AddObject(&lblNameLeft);
	lblNameLeft.SetPos(3, 2);
	lblNameLeft.SetWidth(70);
	lblNameLeft.SetHeight(8);
	lblNameLeft.SetFont("aafont");
	lblNameLeft.SetBGActiveness(false);

	window.AddObject(&lblNameRight);
	lblNameRight.SetPos(82, 2);
	lblNameRight.SetWidth(70);
	lblNameRight.SetHeight(8);
	lblNameRight.SetFont("aafont");
	lblNameRight.SetBGActiveness(false);

	window.AddObject(&pnlSepTop);
	pnlSepTop.SetPos(0, 10);
	pnlSepTop.SetWidth(160);
	pnlSepTop.SetHeight(2);
	pnlSepTop.SetBGColor(.2,.2,.2,1.);

	window.AddObject(&pnlContainerLeft);
	pnlContainerLeft.SetBGActiveness(false);
	pnlContainerLeft.SetPos(0, 12);
	pnlContainerLeft.SetWidth(80);
	pnlContainerLeft.SetHeight(75);
	pnlContainerLeft.SetVirtualSize(80, 75);

	window.AddObject(&pnlContainerRight);
	pnlContainerRight.SetBGActiveness(false);
	pnlContainerRight.SetPos(80, 12);
	pnlContainerRight.SetWidth(80);
	pnlContainerRight.SetHeight(75);
	pnlContainerRight.SetVirtualSize(80, 75);

	// TODO (nfries88): add middle seperator to pnlContainer
	window.AddObject(&pnlSepMiddle);
	pnlSepMiddle.SetPos(80, 0);
	pnlSepMiddle.SetWidth(2);
	pnlSepMiddle.SetHeight(75);
	pnlSepMiddle.SetBGColor(.2,.2,.2,1.);

	window.AddObject(&pnlSepBottom);
	pnlSepBottom.SetPos(0, 88);
	pnlSepBottom.SetWidth(160);
	pnlSepBottom.SetHeight(2);
	pnlSepBottom.SetBGColor(.2,.2,.2,1.);

	window.AddObject(&lblWait);
	lblWait.SetFont("minifont");
	lblWait.SetPos(10, 92);
	lblWait.SetWidth(100);
	lblWait.SetHeight(20);
	lblWait.SetBGActiveness(false);

	window.AddObject(&btnAccept);
	btnAccept.SetFont("minifont",8);
	btnAccept.SetCaption("Accept");
	btnAccept.SetPos(67, 92);
	btnAccept.SetWidth(43);
	btnAccept.SetHeight(20);
	btnAccept.SetOnClick(tradeOnAccept);

	window.AddObject(&btnReject);
	btnReject.SetFont("minifont",8);
	btnReject.SetCaption("Reject");
	btnReject.SetPos(115, 92);
	btnReject.SetWidth(43);
	btnReject.SetHeight(20);
	btnReject.SetOnClick(tradeOnReject);

	m_rightSideSet = false;
	m_leftSideSet = false;
}

winTrade_t::~winTrade_t()
{
	//
}

void winTrade_t::onTradeUpdate(bool ack)
{
	Container* container;
	glictContainer* pnlContainer;
	PanelList* pnlList;
	uint32_t flag;
	if(ack){ //left side
		container = Containers::getInstance().getTradeContainerAck();
		lblNameLeft.SetCaption(container->getName());
		pnlContainer = &pnlContainerLeft;
		pnlList = &pnlItemsLeft;
		m_leftSideSet = true;
		flag = 1;
	}
	else{ //right side
		container = Containers::getInstance().getTradeContainer();
		lblNameRight.SetCaption(container->getName());
		pnlContainer = &pnlContainerRight;
		pnlList = &pnlItemsRight;
		m_rightSideSet = true;
		flag = 0;
	}

	float height = 4 + (36*ceil(container->getCapacity()/2.));
	pnlContainer->SetVirtualSize(80, height);
	pnlContainer->SetHeight(72);

	for(uint32_t i = 0; i != container->getCapacity(); ++i)
    {
        ItemPanel* panel = new ItemPanel(container, i, Position(0,0,0));
        panel->SetPos(5 + ((i % 2) * 36), 4 + (std::floor(i / 2) * 36));
        panel->SetOnClick(tradeItemOnClick);
        //panel->SetOnPaint(tradeItemOnPaint);
		panel->SetOnMouseUp(NULL);
		panel->SetOnMouseDown(NULL);
        panel->SetCustomData((void*)(i*2 | flag));

        pnlContainer->AddObject(panel);
        pnlList->push_back(panel);
    }

    //update buttons
	if(m_leftSideSet && !m_rightSideSet){
		btnReject.SetVisible(true);
		btnReject.SetCaption("Cancel");
		btnAccept.SetVisible(false);
		lblWait.SetCaption("Wait for a counter offer.");
		lblWait.SetVisible(true);
	}
	else if(m_rightSideSet){
		btnReject.SetVisible(true);
		btnReject.SetCaption("Reject");
		btnAccept.SetVisible(true);
		lblWait.SetVisible(false);
	}
}

void winTrade_t::onTradeCompleted()
{
	PanelList::iterator it = pnlItemsLeft.begin();
	for(;it != pnlItemsLeft.end(); ++it)
	{
		pnlContainerLeft.RemoveObject((*it));
		delete (*it);
	}
	pnlItemsLeft.clear();

	for(it = pnlItemsRight.begin(); it != pnlItemsRight.end(); ++it)
	{
		pnlContainerRight.RemoveObject((*it));
		delete (*it);
	}
	pnlItemsRight.clear();

	m_rightSideSet = false;
	m_leftSideSet = false;
}

void winTrade_t::tradeItemOnClick(glictPos* relmousepos, glictContainer* caller)
{
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
	void* data = caller->GetCustomData();
	bool flag = (long)data & 1;
	int slot = (long)data >> 1;
	gw->m_protocol->sendLookInTrade(flag, slot);
}

/*
void winTrade_t::tradeItemOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller)
{
	printf("Item trade paint\n");
	ItemPanel::itemPanelOnPaint(real, clipped, caller);
}
*/

void winTrade_t::tradeOnAccept(glictPos* relmousepos, glictContainer* caller)
{
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
	winTrade_t& window = gw->winTrade;
	window.btnAccept.SetVisible(false);
	window.lblWait.SetCaption("Please wait for your \npartner to accept.");
	window.lblWait.SetVisible(true);
	gw->m_protocol->sendAcceptTrade();
}

void winTrade_t::tradeOnReject(glictPos* relmousepos, glictContainer* caller)
{
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
	gw->m_protocol->sendRejectTrade();
}

void winTrade_t::onCollapse(glictPos* relmousepos, glictContainer* caller)
{
	//
}

void winTrade_t::iconOnPaint(glictRect* real, glictRect* clipped, glictContainer* caller)
{
	g_engine->getUISprite()->Blit((int)real->left, (int)real->top, 325, 60, 12, 12);
}
