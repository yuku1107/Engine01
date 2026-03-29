/*==============================================================================

	É{É^Éì[button.h]
															 Author:äsÅ@ìNâF
															 Date  :2025/12/11
==============================================================================*/
#ifndef BUTTON_H
#define BUTTON_H

#include "Engine/Objects/UIObjects/ui.h"

class Button : public UI
{
protected:
	std::string GetDefaultTexture() const override
{
		return m_NormalTexture.empty() ? "white" : m_NormalTexture;
	}
	std::string GetShaderName() const override { return "unlitTexture"; }
	std::string m_NormalTexture;
	std::string m_HoverTexture;

public:
	void Init() override;   //èâä˙âª
	void UpdateSelf() override; //çXêV
	void DrawInspector() override;

	bool IsHover();
	bool IsClicked();
	std::function<void()> OnClickEvent;

	void SetNormalTexture(const std::string& name) {m_NormalTexture = name;}
	void SetHoverTexture(const std::string& name) { m_HoverTexture = name; }

	const std::string& GetNormalTexture() const { return m_NormalTexture; }
	const std::string& GetHoverTexture()  const { return m_HoverTexture; }

	void Serialize(Serializer& s) override;

	void Deserialize(Serializer& s) override;

	std::string GetObjectClassName() const override { return "Button"; }
};

class StartButton : public Button
{
public:
	void Init() override
	{
		if (m_NormalTexture.empty())
			m_NormalTexture = "start_B";

		if (m_HoverTexture.empty())
			m_HoverTexture = "start_W";

		Button::Init();
	}

	void UpdateSelf() override
	{
		bool hover = IsHover();

		if (hover)
			SetTextureName(m_HoverTexture);
		else
			SetTextureName(m_NormalTexture);

		Button::UpdateSelf();
	}


	std::string GetObjectClassName() const override { return "StartButton"; }
};

class EndButton : public Button
{
public:
	void Init() override
	{
		if (m_NormalTexture.empty())
			SetNormalTexture("end_B");
		if (m_HoverTexture.empty())
			SetHoverTexture("end_W");

		Button::Init();
	}

	void UpdateSelf() override
	{
		bool hover = IsHover();

		if (hover)
			SetTextureName(m_HoverTexture);
		else
			SetTextureName(m_NormalTexture);

		Button::UpdateSelf();
	}


	std::string GetObjectClassName() const override { return "EndButton"; }
};

class ContinueButton : public Button
{
public:
	void Init() override
	{
		if (m_NormalTexture.empty())
			SetNormalTexture("continue_B");
		if (m_HoverTexture.empty())
			SetHoverTexture("continue_W");

		Button::Init();
	}

	void UpdateSelf() override
	{
		bool hover = IsHover();

		if (hover)
			SetTextureName(m_HoverTexture);
		else
			SetTextureName(m_NormalTexture);

		Button::UpdateSelf();
	}


	std::string GetObjectClassName() const override { return "ContinueButton"; }
};

class RetryButton : public Button
{
public:
	void Init() override
	{
		if (m_NormalTexture.empty())
			SetNormalTexture("retry_B");
		if (m_HoverTexture.empty())
			SetHoverTexture("retry_W");

		Button::Init();
	}

	void UpdateSelf() override
	{
		bool hover = IsHover();

		if (hover)
			SetTextureName(m_HoverTexture);
		else
			SetTextureName(m_NormalTexture);

		Button::UpdateSelf();
	}


	std::string GetObjectClassName() const override { return "RetryButton"; }
};


#endif //POLYGON2D_H
