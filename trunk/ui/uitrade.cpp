
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
	btnClose.SetOnClick(onClose);

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
	//pnlIcon.SetCustomData(?);
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

	window.AddObject(&pnlContainer);
	pnlContainer.SetBGActiveness(false);
	pnlContainer.SetPos(0, 12);
	pnlContainer.SetWidth(160);
	pnlContainer.SetHeight(75);
	pnlContainer.SetVirtualSize(160, 75);

	// TODO (nfries88): add middle seperator to pnlContainer
	pnlContainer.AddObject(&pnlSepMiddle);
	pnlSepMiddle.SetPos(78, 0);
	pnlSepMiddle.SetWidth(2);
	pnlSepMiddle.SetHeight(75);
	pnlSepMiddle.SetBGColor(.2,.2,.2,1.);

	window.AddObject(&pnlSepBottom);
	pnlSepBottom.SetPos(0, 88);
	pnlSepBottom.SetWidth(160);
	pnlSepBottom.SetHeight(2);
	pnlSepBottom.SetBGColor(.2,.2,.2,1.);

	window.AddObject(&lblWait);
	lblWait.SetFont("aafont");
	lblWait.SetCaption("Wait for a counter offer.");
	lblWait.SetPos(12, 92);
	lblWait.SetWidth(60);
	lblWait.SetHeight(20);
	lblWait.SetBGActiveness(false);
	lblWait.SetVisible(true);

	window.AddObject(&btnAccept);
	btnAccept.SetFont("minifont",8);
	btnAccept.SetCaption("Accept");
	btnAccept.SetPos(75, 92);
	btnAccept.SetWidth(43);
	btnAccept.SetHeight(20);
	btnAccept.SetVisible(false);

	window.AddObject(&btnReject);
	btnReject.SetFont("minifont",8);
	btnReject.SetCaption("Reject");
	btnReject.SetPos(122, 92);
	btnReject.SetWidth(43);
	btnReject.SetHeight(20);
	//TODO (nfries88): finish constructor

	initiator = NULL;
	acceptor = NULL;
}

winTrade_t::~winTrade_t()
{
	//TODO (nfries88): write destructor
}

void winTrade_t::onTradeStarted(Container* _initiatior)
{
	initiator = _initiatior;

	lblNameLeft.SetCaption(initiator->getName());
	float height = 4 + (36*ceil(initiator->getCapacity()/2.));
	pnlContainer.SetVirtualSize(160, height);
	pnlSepMiddle.SetHeight(height);

	for(uint32_t i = 0; i != initiator->getCapacity(); ++i)
    {
        glictPanel* panel = new glictPanel;
        panel->SetPos(5 + ((i % 2) * 36), 4 + (std::floor(i / 2) * 36));
        panel->SetWidth(32);
        panel->SetHeight(32);
        panel->SetBGColor(.1,.1,.1,1);
        panel->SetCaption("");
        panel->SetCustomData(this);
        panel->SetOnPaint(winTrade_t::tradeItemOnPaint);
        panel->SetOnClick(winTrade_t::tradeItemOnClick);

        panel->SetSkin(&g_skin.inv);

        pnlContainer.AddObject(panel);
        pnlItemsLeft.push_back(panel);
    }
}

void winTrade_t::onTradeAccepted(Container* _acceptor)
{
	acceptor = _acceptor;

	lblNameLeft.SetCaption(acceptor->getName());
	lblWait.SetVisible(false);
	if(!initiator || acceptor->getCapacity() > initiator->getCapacity()){
		float height = 4 + (36*ceil(initiator->getCapacity()/2.));
		pnlContainer.SetVirtualSize(160, height);
		pnlSepMiddle.SetHeight(height);
	}

	for(uint32_t i = 0; i != acceptor->getCapacity(); ++i)
    {
        glictPanel* panel = new glictPanel;
        panel->SetPos(5 + ((i % 2) * 36), 4 + (std::floor(i / 2) * 36));
        panel->SetWidth(32);
        panel->SetHeight(32);
        panel->SetBGColor(.1,.1,.1,1);
        panel->SetCaption("");
        panel->SetCustomData(this);
        panel->SetOnPaint(winTrade_t::tradeItemOnPaint);
        panel->SetOnClick(winTrade_t::tradeItemOnClick);

        panel->SetSkin(&g_skin.inv);

        pnlContainer.AddObject(panel);
        pnlItemsRight.push_back(panel);
    }
}

void winTrade_t::onTradeCompleted()
{
	initiator = NULL;
	acceptor = NULL;

	lblWait.SetVisible(true);
	btnAccept.SetVisible(false);

	PanelList::iterator it = pnlItemsLeft.begin();
	for(;it != pnlItemsLeft.end(); ++it)
	{
		pnlContainer.RemoveObject((*it));
		delete (*it);
	}
	pnlItemsLeft.clear();

	for(it = pnlItemsRight.begin(); it != pnlItemsRight.end(); ++it)
	{
		pnlContainer.RemoveObject((*it));
		delete (*it);
	}
	pnlItemsRight.clear();
}

void winTrade_t::tradeItemOnPaint(glictRect* real, glictRect* clipped, glictContainer* caller)
{
	//TODO (nfries88): write function
}
void winTrade_t::tradeItemOnClick(glictPos* relmousepos, glictContainer* caller)
{
	//TODO (nfries88): finish function
}

void winTrade_t::tradeOnAccept(glictPos* relmousepos, glictContainer* caller)
{
	//TODO (nfries88): finish function
}
void winTrade_t::tradeOnReject(glictPos* relmousepos, glictContainer* caller)
{
	//TODO (nfries88): write function
}

void winTrade_t::onClose(glictPos* relmousepos, glictContainer* caller)
{
	//TODO (nfries88): write function
}
void winTrade_t::onCollapse(glictPos* relmousepos, glictContainer* caller)
{
	//TODO (nfries88): write function
}
void winTrade_t::iconOnPaint(glictRect* real, glictRect* clipped, glictContainer* caller)
{
	//TODO (nfries88): write function
}
