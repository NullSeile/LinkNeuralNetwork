#include "UIObject.h"

namespace ui {

	UIObject::UIObject(const std::string& id)
		: id(id)
	{
	}


	UIObject::~UIObject()
	{
	}

	void UIObject::Update(const sf::RenderWindow& window)
	{
		if (m_hasCustomUpdateFunction)
		{
			m_updateFunction(this);
		}
	}

	void UIObject::BeginPlay()
	{
		if (m_hasCustomBeginPlayFunction)
		{
			m_beginPlayFunction(this);
		}
	}

	void UIObject::SetUpdateFunction(const std::function<void(UIObject* self)>& function)
	{
		m_hasCustomUpdateFunction = true;
		m_updateFunction = function;
	}

	void UIObject::SetBeginPlayFunction(const std::function<void(UIObject* self)>& function)
	{
		m_hasCustomBeginPlayFunction = true;
		m_beginPlayFunction = function;
	}
}