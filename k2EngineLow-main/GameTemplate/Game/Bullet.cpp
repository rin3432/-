#include "stdafx.h"
#include "Bullet.h"
#include "GameCamera.h"
#include "Player.h"

namespace {
    const Vector3 initPos = { 0.0f,80.0f,0.0f };
}

Bullet::Bullet()
{

}

Bullet::~Bullet()
{

}

bool Bullet::Start()
{
    gameCamera = FindGO<GameCamera>("gameCamera");
    player = FindGO<Player>("player");

    m_velocity = Vector3::AxisZ;
   

    m_position += m_velocity * 50.0f;
    m_velocity *= 30.0f;
    m_rotation.SetRotationDegX(90.0f);

    InitModel();
    

    return true;
}

void Bullet::Update()
{
    if (flag == true) {
        if (initAfter == false) {
            InitPos();
            InitRot();
        }
        else{
            Move();
            Rotation();
            DeleteTime();
        }
       
    }

    m_model.UpdateWorldMatrix(
        m_position,
        m_rotation,
        g_vec3One
    );
}

void Bullet::InitModel()
{
    m_modelInitData.m_tkmFilePath = "Assets/modelData/Bullet.tkm";
    m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";
    //m_modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

    m_model.Init(m_modelInitData);
}

void Bullet::InitPos()
{
    m_position = player->GetPos() + initPos;
    initAfter = true;
}

void Bullet::InitRot()
{
    Vector3 hitPos = gameCamera->GetHitPos();
    Vector3 hitRot = hitPos - initPos;
    m_velocity = hitRot;
    m_velocity.Normalize();
}

void Bullet::Move()
{
    m_position += m_velocity * 800.0f * g_gameTime->GetFrameDeltaTime();
}

void Bullet::Rotation()
{
    
}

void Bullet::DeleteTime()
{
    if (deleteTime > 0.0f) {
        deleteTime -= g_gameTime->GetFrameDeltaTime();
    }
    else {
        Init();
    }
}

void Bullet::Render(RenderContext& rc)
{
    m_model.Draw(rc);
}
