#include "stdafx.h"
#include "Bullet.h"
#include "GameCamera.h"
#include "Player.h"
#include "Enemy.h"

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
    enemy = FindGO<Enemy>("enemy");

    m_velocity = Vector3::AxisZ;
   
    m_position = { 0.0f,-100.0f,0.0f };

    m_position += m_velocity * 50.0f;
    m_velocity *= 30.0f;
    m_rotation.SetRotationDegX(90.0f);

    InitModel();
    InitCollision();

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
            //const std::vector<Enemy*>& enemys = FindGOs<Enemy>("enemy");

            Move();
            Rotation();
            DeleteTime();
            HitJudgment();
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
    m_initPos = player->GetPos() + initPos;
    m_position = m_initPos;
    initAfter = true;
}

void Bullet::InitRot()
{
    Vector3 hitPos = gameCamera->GetHitPos();
    Vector3 hitRot = hitPos - m_initPos;
    m_velocity = hitRot;
    m_velocity.Normalize();
}

void Bullet::InitCollision()
{
    m_collision = NewGO<CollisionObject>(0);
    m_collision->CreateBox(m_position, Quaternion::Identity, Vector3(10.0f, 10.0f, 10.0f));
    m_collision->SetName("playerBullet");
    m_collision->SetIsEnableAutoDelete(false);
}

void Bullet::Move()
{
    m_position += m_velocity * 800.0f * g_gameTime->GetFrameDeltaTime();
    m_collision->SetPosition(m_position);
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

void Bullet::HitJudgment()
{
    const auto& collisions = g_collisionObjectManager->FindCollisionObjects("playerBullet");
    for (auto collision : collisions) {
        if (collision->IsHit(enemy->GetCol())) {
            enemy->DeleteEnemy();
            Init();
        }
    }

}

void Bullet::Render(RenderContext& rc)
{
    if (flag == true) {
        m_model.Draw(rc);
    }
}
