#include "Widget.h"

namespace ui {

	Widget::Widget()
	{

	}

	Widget::~Widget()
	{
	}

	// Other
	void Widget::Clear()
	{
		m_objects.clear();
	}

	void Widget::AddObject(UIObject* newObject)
	{
		m_objects.push_back(newObject);
	}

	UIObject * Widget::GetUIObject(const std::string& id)
	{
		for (UIObject* x : m_objects)
		{
			if (!x)
				continue;

			if (x->id == id)
			{
				return x;
			}
		}
		return nullptr;
	}

	void Widget::Draw(sf::RenderWindow& window)
	{
		for (UIObject* o : m_objects)
		{
			o->Draw(window);
		}
	}

	void Widget::BeginPlay()
	{
		for (UIObject* o : m_objects)
		{
			o->BeginPlay();
		}
	}

	void Widget::CheckInput(const sf::RenderWindow& window, const sf::Event& e)
	{
		for (UIObject* o : m_objects)
		{
			auto i = dynamic_cast<Interactuable*>(o);

			if (i)
			{
				i->CheckInput(window, e);
			}
		}
	}

	void Widget::Update(const sf::RenderWindow& window)
	{
		for (UIObject* o : m_objects)
		{
			o->Update(window);
		}
	}

}