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
		//位置設定。
		void SetPosition(const Vector3& pos)
		{
			position = pos;
		}
		void SetPosition(float x, float y, float z)
		{
			this->SetPosition({ x, y, z });
		}
		//方向設定。
		void SetDirection(const Vector3& direction)
		{
			this->direction = direction;
		}
		void SetDirection(float x, float y, float z)
		{
			SetDirection({ x,y,z });
		}
		//方向を正規化。
		void NormalizeDirection()
		{
			direction.Normalize();
		}
		//カラー設定。
		void SetColor(const Vector3& color)
		{
			this->color = color;
		}
		void SetColor(float r, float g, float b)
		{
			SetColor({ r,g,b });
		}
		//影響率設定。
		void SetRange(float range)
		{
			this->range = range;
		}
		//角度設定。
		void SetAngle(float angle)
		{
			this->angle = angle;
		}

		//位置取得。
		const Vector3& GetPosition() const
		{
			return position;
		}
		//方向取得。
		const Vector3& GetDiretion() const
		{
			return direction;
		}
		//カラー取得。
		const Vector4& GetColor() const
		{
			return color;
		}
	};
}

