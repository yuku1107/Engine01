/*==============================================================================

	ÉVÅ[Éìï`âÊ[scenePolygon.h]
															 Author:äsÅ@ìNâF
															 Date  :2025/09/11
==============================================================================*/
#ifndef SCENEPOLYGON_H
#define SCENEPOLYGON_H

#include "Engine/Core/gameObject.h"

enum class FadeState
{
	None,
	FadingIn,
	FadingOut,
	CompletedIn,
	CompletedOut
};

class ScenePolygon2D : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11ShaderResourceView* m_Texture;

	XMFLOAT4 m_Parameter = { 0.0f,1.0f,1.0f,1.0f };
	int m_FadeDirection = 0;
	float m_FadeSpeed = 1.0f; 

	FadeState m_FadeState = FadeState::None;

public:
	void Init() override;   //èâä˙âª
	void Init(int num);
	void Init(float x, float y, float Width, float Height, const std::string& TextureName);
	void Init(float x, float y, float Width, float Height, ID3D11ShaderResourceView* Texture);
	void UninitSelf() override;//èIóπ
	void UpdateSelf() override; //çXêV
	void DrawSelf() override;   //ï`âÊ

	void Deserialize(Serializer& s) override;

	void StartFadeIn()
	{
		m_FadeState = FadeState::FadingIn;
	}

	void StartFadeOut()
	{
		m_FadeState = FadeState::FadingOut;
	}
	bool IsFading() const
	{
		return m_FadeState == FadeState::FadingIn ||
			m_FadeState == FadeState::FadingOut;
	}

	bool IsFadeInComplete() const
	{
		return m_FadeState == FadeState::CompletedIn;
	}

	bool IsFadeOutComplete() const
	{
		return m_FadeState == FadeState::CompletedOut;
	}

	std::string GetObjectClassName() const override { return "ScenePolygon2D"; }
};

#endif //SCENEPOLYGON_H
