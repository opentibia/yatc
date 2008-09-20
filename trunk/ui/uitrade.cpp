
#include "uitrade.h"

winTrade_t::winTrade_t(Container* _initatior)
{
	initator = _initatior;

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
	//btnCollapse.SetOnClick(onCollapse);

	window.AddTitlebarObject(&pnlIcon);
	pnlIcon.SetWidth(12);
	pnlIcon.SetHeight(12);
	pnlIcon.SetPos(2, 2);
	//pnlIcon.SetCustomData(?);
	//pnlIcon.SetOnPaint(iconOnPaint);
	#endif
	//TODO (nfries88): finish constructor
}

winTrade_t::~winTrade_t()
{
	//TODO (nfries88): write destructor
}

void winTrade_t::onTradeAccepted(Container* _acceptor)
{
	acceptor = _acceptor;
	//TODO (nfries88): finish function
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
