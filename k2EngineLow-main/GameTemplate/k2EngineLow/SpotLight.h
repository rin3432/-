#pragma once

namespace nsK2EngineLow {
	struct SpotLight {
	private:
		Vector3 position;
		float pad0;
		Vector3 color;
		float range;
		Vector3 direction;
		float angle;

	public:
		//�ʒu�ݒ�B
		void SetPosition(const Vector3& pos)
		{
			position = pos;
		}
		void SetPosition(float x, float y, float z)
		{
			this->SetPosition({ x, y, z });
		}
		//�����ݒ�B
		void SetDirection(const Vector3& direction)
		{
			this->direction = direction;
		}
		void SetDirection(float x, float y, float z)
		{
			SetDirection({ x,y,z });
		}
		//�����𐳋K���B
		void NormalizeDirection()
		{
			direction.Normalize();
		}
		//�J���[�ݒ�B
		void SetColor(const Vector3& color)
		{
			this->color = color;
		}
		void SetColor(float r, float g, float b)
		{
			SetColor({ r,g,b });
		}
		//�e�����ݒ�B
		void SetRange(float range)
		{
			this->range = range;
		}
		//�p�x�ݒ�B
		void SetAngle(float angle)
		{
			this->angle = angle;
		}

		//�ʒu�擾�B
		const Vector3& GetPosition() const
		{
			return position;
		}
		//�����擾�B
		const Vector3& GetDiretion() const
		{
			return direction;
		}
		//�J���[�擾�B
		const Vector4& GetColor() const
		{
			return color;
		}
	};
}

