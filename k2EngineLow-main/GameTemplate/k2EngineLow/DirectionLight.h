#pragma once

namespace nsK2EngineLow {
	struct DirectionalLight
	{
	private:
		Vector3 ligDirection;	//�����B
		float pad0;
		Vector3 color;		//�J���[�B
		float pad1;
		//int castShadow;		//�e�̃L���X�g�B

	public:
		//�����ݒ�B
		void SetDirection(const Vector3& direction) 
		{
			ligDirection = direction;
		}
		void SetDirection(float x,float y,float z)
		{
			this->SetDirection({ x,y,z });
		}
		//�����𐳋K���B
		void NormalizeDirection()
		{
			ligDirection.Normalize();
		}
		//�J���[�ݒ�B
		void SetColor(const Vector3& color)
		{
			this->color = color;
		}
		void SetColor(float r, float g, float b)
		{
			this->SetColor({ r,g,b });
		}

		//�����擾�B
		const Vector3& GetDiretion() const
		{
			return ligDirection;
		}
		//�J���[�擾�B
		const Vector4& GetColor() const
		{
			return color;
		}
	};

	

	
}


