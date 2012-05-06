#include "List.h"
#include "Context.h"

namespace UI {

// XXX skin
static const float ALPHA_NORMAL = 0.0f;
static const float ALPHA_HOVER  = 0.4f;
static const float ALPHA_SELECT = 0.6f;

List::List(Context *context) : Container(context), m_selected(0)
{
	Context *c = GetContext();
	m_container = c->Background();
	m_container->SetInnerWidget(c->VBox());
	AddWidget(m_container);
}

vector2f List::PreferredSize() {
	return m_container->PreferredSize();
}

void List::Layout() {
	SetWidgetDimensions(m_container, 0, GetSize());
	m_container->Layout();
}

void List::RequestResize()
{
    if (GetContainer()) GetContainer()->RequestResize();
}

List *List::AddOption(const std::string &text)
{
	m_options.push_back(text);

	Context *c = GetContext();

	VBox *vbox = static_cast<VBox*>(m_container->GetInnerWidget());

	unsigned int index = m_optionBackgrounds.size();

	ColorBackground *background = c->ColorBackground(Color(0,0,0, m_selected == index ? ALPHA_SELECT : ALPHA_NORMAL));
	vbox->PackEnd(background->SetInnerWidget(c->Label(text)));

	background->onMouseOver.connect(sigc::bind(sigc::mem_fun(this, &List::HandleOptionMouseOver), index));
	background->onMouseOut.connect(sigc::bind(sigc::mem_fun(this, &List::HandleOptionMouseOut), index));
	background->onClick.connect(sigc::bind(sigc::mem_fun(this, &List::HandleOptionClick), index));

	m_optionBackgrounds.push_back(background);

	return this;
}

bool List::HandleOptionMouseOver(unsigned int index)
{
	m_optionBackgrounds[index]->SetColor(Color(0,0,0, ALPHA_HOVER));
	return false;
}

bool List::HandleOptionMouseOut(unsigned int index)
{
	m_optionBackgrounds[index]->SetColor(Color(0,0,0, m_selected == index ? ALPHA_SELECT : ALPHA_NORMAL));
	return false;
}

bool List::HandleOptionClick(unsigned int index)
{
	if (m_selected != index) {
		m_optionBackgrounds[m_selected]->SetColor(Color(0,0,0, ALPHA_NORMAL));
		m_selected = index;
		onOptionSelected.emit(index, m_options[index]);
	}

	return false;
}

}
