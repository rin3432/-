#pragma once

namespace nsK2EngineLow {
	struct PointLight
	{
	private:
		Vector3 position;
		float pad0;
		Vector3 color;
		float ptRange;	//影響範囲。

	public:
		//座標設定。
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
		//カラー設定。
		void SetColor(const Vector3& color)
		{
			this->color = color;
		}
		void SetColor(float r, float g, float b)
		{
			this->SetColor({ r, g, b });
		}
		//影響範囲設定。
		void SetRange(float range)
		{
			ptRange = range;
		}
		//座標取得。
		const Vector3& GetPosition() const
		{
			return position;
		}
		//カラー取得。
		const Vector4& GetColor() const
		{
			return color;
		}
	};
}


