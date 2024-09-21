#pragma once

namespace nsK2EngineLow {
	struct DirectionalLight
	{
	private:
		Vector3 ligDirection;	//方向。
		float pad0;
		Vector3 color;		//カラー。
		float pad1;
		//int castShadow;		//影のキャスト。

	public:
		//方向設定。
		void SetDirection(const Vector3& direction) 
		{
			ligDirection = direction;
		}
		void SetDirection(float x,float y,float z)
		{
			this->SetDirection({ x,y,z });
		}
		//方向を正規化。
		void NormalizeDirection()
		{
			ligDirection.Normalize();
		}
		//カラー設定。
		void SetColor(const Vector3& color)
		{
			this->color = color;
		}
		void SetColor(float r, float g, float b)
		{
			this->SetColor({ r,g,b });
		}

		//方向取得。
		const Vector3& GetDiretion() const
		{
			return ligDirection;
		}
		//カラー取得。
		const Vector4& GetColor() const
		{
			return color;
		}
	};

	

	
}


