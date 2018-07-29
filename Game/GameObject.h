#pragma once
#include "Game.h"
#include "CXLib.h"
#include "Vec2.h"
#include "GameTimer.h"

// �񋓌^�̒�` ============================================================

// <�ʒu�֌W> ----------------------------------------------------------
typedef enum
{
	NONE = 0,
	CENTER_X = 1,				// X����
	LEFT = -2,					// ��
	RIGHT = 2,					// �E
	CENTER_Y = -1,				// Y����
	TOP = -3,					// ��
	BOTTOM = 3					// ��
} ObjectSide;

// <���̈ʒu> ----------------------------------------------------------
typedef enum
{
	EDGESIDE_CENTER = 0,		// ���̏�
	EDGESIDE_OUTER = -1,		// ���̊O��
	EDGESIDE_INNER = 1			// ���̓���
} ObjectEdgeSide;

// <���[���h�̂Ȃ���> ------------------------------------------------
typedef enum
{
	CONNECTION_NONE = 0,		// �q����Ȃ��A�����Ȃ���ԂɈړ�
	CONNECTION_BARRIER,			// �ǂ�����A�i�߂Ȃ�
	CONNECTION_LOOP				// ���Α�����o�Ă���
} ObjectConnection;

// <�I�u�W�F�N�g�̌`> --------------------------------------------------
typedef enum
{
	SHAPE_BOX,					// �����`
	SHAPE_CIRCLE,				// �~
} ObjectShape;

// �\���̂̐錾 ============================================================

// <�Q�[���I�u�W�F�N�g>
typedef struct
{
	Vec2 pos;					// <�ʒu>
	Vec2 vel;					// <���x>
	Vec2 size;					// <�傫��>
	ObjectShape shape;			// <�`>
	ATTR color;					// <�F>
	ObjectConnection sprite_connection;	// <�X�v���C�g�̂Ȃ���>
	BOOL alive;					// <�\�����>
	int state;					// <���>
	int type;					// <�^�C�v>
	GameTimer count;			// <�J�E���^>
} GameObject;

// �萔�̒�` ==============================================================

// <�e�N�X�`��>
#define TEXTURE_MISSING -1		// �e�N�X�`����������܂���
#define TEXTURE_NONE -2			// �e�N�X�`���Ȃ�

// �֐��̐錾 ==============================================================

// <<�I�u�W�F�N�g>> ----------------------------------------------------

// <�I�u�W�F�N�g�쐬>
GameObject GameObject_Create(Vec2 pos = Vec2_Create(), Vec2 vel = Vec2_Create(), Vec2 size = Vec2_Create());

// <�I�u�W�F�N�g�폜>
void GameObject_Dispose(GameObject* obj);

// <�I�u�W�F�N�g�m�F>
BOOL GameObject_IsAlive(const GameObject* obj);

// <�I�u�W�F�N�g���W�X�V>
void GameObject_UpdatePosition(GameObject* obj);

// <�I�u�W�F�N�gX�I�t�Z�b�g>
float GameObject_OffsetX(const GameObject* obj, ObjectSide side, float pos = 0.f, float margin = 0.f);

// <�I�u�W�F�N�gX�I�t�Z�b�g>
float GameObject_OffsetY(const GameObject* obj, ObjectSide side, float pos = 0.f, float margin = 0.f);

// <�I�u�W�F�N�gX�ʒu�Q�b�g>
float GameObject_GetX(const GameObject* obj, ObjectSide side, float margin = 0.f);

// <�I�u�W�F�N�gY�ʒu�Q�b�g>
float GameObject_GetY(const GameObject* obj, ObjectSide side, float margin = 0.f);

// <�I�u�W�F�N�g�����蔻��>
BOOL GameObject_IsHit(const GameObject* obj1, const GameObject* obj2);

// <�I�u�W�F�N�g�`��>
void GameObject_Render(const GameObject* obj, const Vec2* translate = &Vec2_Create());

// <<�t�B�[���h�I�u�W�F�N�g>> ------------------------------------------

// <�t�B�[���h�I�u�W�F�N�g�쐬>
GameObject GameObject_Field_Create(void);

// <�t�B�[���h�㉺�Փˏ���>
ObjectSide GameObject_Field_CollisionVertical(const GameObject* field, GameObject* obj, ObjectConnection connection, ObjectEdgeSide edge);

// <�t�B�[���h���E�Փˏ���>
ObjectSide GameObject_Field_CollisionHorizontal(const GameObject* field, GameObject* obj, ObjectConnection connection, ObjectEdgeSide edge);

// <�t�B�[���h�`��>
void GameObject_Field_Render(const GameObject* field);
