#include "SpaceStation.h"
#include "SpaceStationView.h"
#include "Pi.h"
#include "Player.h"
#include "WorldView.h"

class StationSubView: public Gui::Fixed {
public:
	StationSubView(SpaceStationView *parent): Gui::Fixed(Gui::Screen::GetWidth(), Gui::Screen::GetHeight()-64) {
		m_parent = parent;
	}
protected:
	SpaceStationView *m_parent;
};

/////////////////////////////////////////////////////////////////////////////

class StationFrontView: public StationSubView {
public:
	StationFrontView(SpaceStationView *parent);
private:
	void OnClickRequestLaunch()
	{
		Pi::player->SetDockedWith(0,0);
		Pi::SetView(Pi::worldView);
	}

	void OnClickGotoShipYard()
	{
		m_parent->GotoShipyard();
	}
};

StationFrontView::StationFrontView(SpaceStationView *parent): StationSubView(parent)
{
	SetTransparency(false);

	Gui::Label *l = new Gui::Label("Hello friend! Thankyou for docking with this space station!\n"
	"You may have noticed that the docking procedure was not entirely "
	"physically correct. This is a result of unimplemented physics in this "
	"region of the galaxy. We hope to have things back to normal within a "
	"few weeks, and in the mean time would like to offer our apologies for "
	"any loss of earnings, immersion or lunch.  "
	"Currently the usual space station services are not available, but we "
	"can offer you this promotional message from one of the station's sponsors:\n"
	"                       DIET STEAKETTE: IT'S BAD");

	Gui::Fixed *fbox = new Gui::Fixed(720, 400);
	fbox->Add(l, 0, 0);
	Add(fbox, 40, 100);
	fbox->ShowAll();

	Gui::SolidButton *b = new Gui::SolidButton();
	b->onClick.connect(sigc::mem_fun(this, &StationFrontView::OnClickRequestLaunch));
	Add(b, 40, 300);
	l = new Gui::Label("Request Launch");
	Add(l, 65, 300);

	b = new Gui::SolidButton();
	b->onClick.connect(sigc::mem_fun(this, &StationFrontView::OnClickGotoShipYard));
	Add(b, 40, 360);
	l = new Gui::Label("Shipyard");
	Add(l, 65, 360);

}

////////////////////////////////////////////////////////////////////

class StationShipyardView: public StationSubView {
public:
	StationShipyardView(SpaceStationView *parent);
private:
	virtual void ShowAll();
};

StationShipyardView::StationShipyardView(SpaceStationView *parent): StationSubView(parent)
{
	SetTransparency(false);
}

void StationShipyardView::ShowAll()
{
	DeleteAllChildren();

	SpaceStation *station = Pi::player->GetDockedWith();
	assert(station);
	SetTransparency(false);
	
	Gui::Fixed *fbox = new Gui::Fixed(500, 200);
	Add(fbox, 300, 100);

	Gui::VScrollBar *scroll = new Gui::VScrollBar();
	Gui::VScrollPortal *portal = new Gui::VScrollPortal(450,200);
	scroll->SetAdjustment(&portal->vscrollAdjust);
	//int GetEquipmentStock(Equip::Type t) const { return m_equipmentStock[t]; }

	int NUM_ITEMS = 0;
	const float YSEP = Gui::Screen::GetFontHeight() * 1.5;
	for (int i=1; i<Equip::TYPE_MAX; i++) {
		if (station->GetEquipmentStock(static_cast<Equip::Type>(i))) NUM_ITEMS++;
	}

	Gui::Fixed *innerbox = new Gui::Fixed(400, NUM_ITEMS*YSEP);
	for (int i=1, num=0; i<Equip::TYPE_MAX; i++) {
		int stock = station->GetEquipmentStock(static_cast<Equip::Type>(i));
		if (!stock) continue;
		Gui::Label *l = new Gui::Label(EquipType::types[i].name);
		innerbox->Add(l,0,num*YSEP);
		innerbox->Add(new Gui::SolidButton(), 275, num*YSEP);
		innerbox->Add(new Gui::SolidButton(), 300, num*YSEP);
		char buf[128];
		snprintf(buf, sizeof(buf), "$%d", station->GetEquipmentPrice(static_cast<Equip::Type>(i)));
		innerbox->Add(new Gui::Label(buf), 200, num*YSEP);
		snprintf(buf, sizeof(buf), "%dt", EquipType::types[i].mass);
		innerbox->Add(new Gui::Label(buf), 370, num*YSEP);
		num++;
	}
	innerbox->ShowAll();

	fbox->Add(new Gui::Label("Item"), 0, 0);
	fbox->Add(new Gui::Label("Price"), 200, 0);
	fbox->Add(new Gui::Label("Fit"), 275, 0);
	fbox->Add(new Gui::Label("Remove"), 300, 0);
	fbox->Add(new Gui::Label("Wt"), 370, 0);
	fbox->Add(portal, 0, YSEP);
	fbox->Add(scroll, 455, YSEP);
	portal->Add(innerbox);
	portal->ShowAll();
	fbox->ShowAll();

	Gui::Fixed::ShowAll();
}

/////////////////////////////////////////////////////////////////////

SpaceStationView::SpaceStationView(): View()
{
	m_frontview = new StationFrontView(this);
	m_shipyard = new StationShipyardView(this);
	m_subview = 0;
	SwitchView(m_frontview);

	Gui::Label *l = new Gui::Label("Comms Link");
	l->SetColor(1,.7,0);
	m_rightRegion2->Add(l, 10, 0);
}

void SpaceStationView::SwitchView(StationSubView *v)
{
	if (m_subview) {
		m_subview->HideAll();
		Remove(m_subview);
	}
	m_subview = v;
	Add(m_subview, 0, 0);
	m_subview->ShowAll();
}

void SpaceStationView::GotoShipyard()
{
	SwitchView(m_shipyard);
}

void SpaceStationView::OnSwitchTo()
{
	SwitchView(m_frontview);
}

void SpaceStationView::Draw3D()
{
}

void SpaceStationView::Update()
{
}
