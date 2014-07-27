#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

class RevBaseInstance;

class GameObject
{
public:
	GameObject(void);
	virtual ~GameObject(void);

	virtual bool Update( const float& deltaTime );
	
	void Die();
	void Activate();

	const CU::Vector3f& GetPosition();

	void SetPosition(const CU::Vector3f& position);
	void SetRotation(const CU::Matrix33f& rot );
	void SetTransformation(const CU::Matrix44f& transformation );
	void SetInstance(RevBaseInstance* instance);

private:
	RevBaseInstance* m_instance;
};

#endif