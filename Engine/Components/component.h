/*==============================================================================

	コンポーネントクラス[component.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/05/14
==============================================================================*/
#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{

protected:
	class  GameObject* m_pOwner = nullptr;

public:
	//Component() = delete;
	Component(){}
	Component(GameObject* Object) { m_pOwner = Object; }
	virtual ~Component() {}

	void SetpOwner(GameObject* object) { m_pOwner = object; }
	GameObject* GetpOwner() const { return m_pOwner; }

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

	virtual bool IsAutoDraw() const { return true; }
};

#endif //COMPONENT_H