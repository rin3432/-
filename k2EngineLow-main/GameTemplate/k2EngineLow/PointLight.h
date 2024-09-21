#pragma once

namespace nsK2EngineLow {
	struct PointLight
	{
	private:
		Vector3 position;
		float pad0;
		Vector3 color;
		float ptRange;	//�e���͈́B

	public:
		//���W�ݒ�B
		void SetPosition(const Vector3& pos)
		{
			position = pos;
		}
		void SetPosition(float x, float y, float z)
		{
			this->SetPosition({ x, y, z });
		}

		/*void SetPosition(float x)
		{
			position.x = x;
		}
		void SetPosition(float y)
		{
			position.y = y;
		}
		void SetPosition(float z)
		{
			position.z = z;
		}*/
		//�J���[�ݒ�B
		void SetColor(const Vector3& color)
		{
			this->color = color;
		}
		void SetColor(float r, float g, float b)
		{
			this->SetColor({ r, g, b });
		}
		//�e���͈͐ݒ�B
		void SetRange(float range)
		{
			ptRange = range;
		}
		//���W�擾�B
		const Vector3& GetPosition() const
		{
			return position;
		}
		//�J���[�擾�B
		const Vector4& GetColor() const
		{
			return color;
		}
	};
}


