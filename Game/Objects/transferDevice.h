/*==============================================================================

	ì]ëóëïíu[transferDevice.h]
															 Author:äsÅ@ìNâF
															 Date  :2025/09/13
==============================================================================*/
#ifndef TRANSFERDEVICE_H
#define TRANSFERDEVICE_H

#include "Engine/Core/gameObject.h"


class TransferDevice : public GameObject
{
public:
	enum TransferType
	{
		Type_NONE = 0,
		Type_ToStage1,
		Type_ToStage2,
		Type_ToStage3,
		Type_ToResult,
		Type_MAX,
	};
private:
	std::string m_AnimationName;
	std::string m_AnimationNameNext;
	unsigned int m_Frame;
	class AnimationModel* m_Model;

	class QuadMesh* m_QuadMesh;
	ID3D11ShaderResourceView* m_Texture;
	bool m_QuadMeshEnable = false;

	ID3D11Buffer* m_TimeBuffer;
	ID3D11ShaderResourceView* m_TimeSRV;
	float m_Time;

	TransferType m_Type;

	bool m_CanUse = false;
public:
	void Init() override;   //èâä˙âª
	void UninitSelf() override; //èIóπ
	void UpdateSelf() override; //çXêV
	void DrawSelf() override;   //ï`âÊ
	void DrawInspector() override;

	void SetType(TransferType type) { m_Type = type; }
	const TransferType GetType()const { return m_Type; }

	void AnimationBlend(std::string AnimationName);

	void SetCanUse(bool use) { m_CanUse = use; }
	const bool GetCanUse()const { return m_CanUse; }

	void Serialize(Serializer& s) override;

	void Deserialize(Serializer& s) override;

	std::string GetObjectClassName() const override { return "TransferDevice"; }

};

#endif //TRANSFERDEVICE_H
