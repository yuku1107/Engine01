/*==============================================================================

	uiï`âÊ[ui.h]
															 Author:äsÅ@ìNâF
															 Date  :2026/02/13
==============================================================================*/
#ifndef UI_H
#define UI_H

#include "Engine/Core/gameObject.h"
#include "Engine/Serialization/Serializer.h"

class UI : public GameObject
{
private:
	int m_Frame = 0;
	int m_SplitX = 1;
	int m_SplitY = 1;
	ID3D11ShaderResourceView* m_Texture;
	std::string m_TextureName;

protected:
	virtual std::string GetDefaultTexture() const { return "white"; }
	virtual std::string GetShaderName() const { return "unlitTextureAnimation"; }

public:
	void Init() override;   //èâä˙âª
	void Init(float x, float y, float Width, float Height, const std::string& name);
	void Init(float x, float y, float Width, float Height, const std::string& name, const char* FileName);
	void UninitSelf() override;//èIóπ
	void UpdateSelf() override; //çXêV
	void DrawSelf() override;   //ï`âÊ
	void DrawInspector() override;


	void SetFrame(int frame) { m_Frame = frame; }
	void SetSplit(int x, int y) { m_SplitX = x; m_SplitY = y; }
	int  GetFrame() const { return m_Frame; }

	std::string GetTextureName() { return m_TextureName; }
	void SetTextureName(const std::string& name);

	bool GetCollision(Vector3 pos);

	void Serialize(Serializer& s) override
	{
		GameObject::Serialize(s);

		s.WriteString("textureName", m_TextureName);
	}

	void Deserialize(Serializer& s) override
	{
		GameObject::Deserialize(s);

		m_TextureName = s.ReadString("textureName");
	}

	std::string GetObjectClassName() const override { return "UI"; }
};

class TitleUI :public UI
{
protected:
	virtual std::string GetDefaultTexture() const { return "title"; }
public:

	std::string GetObjectClassName() const override { return "TitleUI"; }
};

class ResultUI :public UI
{
protected:
	virtual std::string GetDefaultTexture() const { return "result"; }
public:

	std::string GetObjectClassName() const override { return "ResultUI"; }
};

class WhiteUI :public UI
{
protected:
	virtual std::string GetDefaultTexture() const { return "white"; }
public:

	std::string GetObjectClassName() const override { return "WhiteUI"; }
};

class LoadingUI :public UI
{
protected:
	virtual std::string GetDefaultTexture() const { return "loading"; }
public:

	std::string GetObjectClassName() const override { return "LoadingUI"; }
};

class SpinnerUI :public UI
{
protected:
	virtual std::string GetDefaultTexture() const { return "spinner"; }
public:
	void UpdateSelf() override;

	std::string GetObjectClassName() const override { return "SpinnerUI"; }
};

class ClearUI :public UI
{
protected:
	virtual std::string GetDefaultTexture() const { return "clear"; }
public:
	void UpdateSelf() override;

	std::string GetObjectClassName() const override { return "ClearUI"; }
};

class DigitUI : public UI
{
protected:
	std::string GetDefaultTexture() const override { return "number"; }
	std::string GetShaderName() const override { return "unlitTextureAnimation"; }

public:
	void InitDigit(float x, float y, float width, float height)
	{
		UI::Init(x, y, width, height, "number");

		SetSplit(5, 5);
		SetDigit(0);
	}
	void SetDigit(int digit);

	std::string GetObjectClassName() const override { return "DigitUI"; }
};

class MouseUI :public UI
{
protected:
	virtual std::string GetDefaultTexture() const { return "cursor"; }
public:
	void UpdateSelf() override;

	std::string GetObjectClassName() const override { return "MouseUI"; }
};

#endif //UI_H
