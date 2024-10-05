#pragma once
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace nsK2EngineLow {
	class Light : Noncopyable{
	private:
		DirectionalLight dirLight;
		PointLight ptLight;
		SpotLight spLight;

		Vector3 eyePos;		//視点の位置。
		float specPow;
		Vector3 ambientLight;
		float pad1;

		Vector3 groundColor;
		float pad2;
		Vector3 skyColor;
		float pad3;
		Vector3 groundNormal;

	public:
		void SetEyePos(const Vector3& pos)
		{
			eyePos = pos;
		}
		void SetEyePos(float x, float y, float z)
		{
			SetEyePos({ x,y,z });
		}
		void SetAmbient(const Vector3& pos)
		{
			ambientLight = pos;
		}
		void SetAmbient(float x, float y, float z)
		{
			SetAmbient({ x,y,z });
		}
		//スペキュラの絞り設定。
		void SetSpecPow(float spec)
		{
			specPow = spec;
		}
		//地面色設定。
		void SetGroundColor(const Vector3& color)
		{
			groundColor = color;
		}
		void SetGroundColor(float x, float y, float z)
		{
			SetGroundColor({ x,y,z });
		}
		//天球色設定。
		void SetSkyColor(const Vector3& color)
		{
			skyColor = color;
		}
		void SetSkyColor(float x, float y, float z)
		{
			SetSkyColor({ x,y,z });
		}
		//地面の法線設定。
		void SetGroundNormal(const Vector3& normal)
		{
			groundNormal = normal;
		}
		void SetGroundNormal(float x, float y, float z)
		{
			SetGroundNormal({ x,y,z });
		}

		void SetLight(DirectionalLight& dirLig, PointLight& ptLig, SpotLight& spLig)
		{
			dirLight = dirLig;
			ptLight = ptLig;
			spLight = spLig;
		}
		/*void SetDirLight(DirectionalLight& dirLig)
		{
			dirLight = dirLig;
			
		}
		void SetPtLight(PointLight& ptLig)
		{
			ptLight = ptLig;
		}
		void SetSpLight(SpotLight& spLig)
		{
			spLight = spLig;
		}*/
	};
}

/*
void Set(const Vector3& )
{

}
void Set(float x, float y, float z)
{

}
*/