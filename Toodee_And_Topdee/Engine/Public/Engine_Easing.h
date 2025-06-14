#ifndef Engine_Easing_h__
#define Engine_Easing_h__

namespace Engine
{

	inline float Lerp(float start, float end, float value)
	{
		return start + (end - start) * value;
	}

	inline float EaseInOutBack(float x, const float c1 = 1.70158f)
	{
		const float c2 = c1 * 1.525f;

		return x < 0.5
			? (powf(2.f * x, 2.f) * ((c2 + 1.f) * 2.f * x - c2)) / 2.f
			: (powf(2.f * x - 2.f, 2.f) * ((c2 + 1.f) * (x * 2.f - 2.f) + c2) + 2.f) / 2.f;
	}

	inline float EaseOutCubic(float x)
	{
		return 1.f - powf(1.f - x, 3);
	}

	inline float EaseOutBounce(float x)
	{
		const float n1 = 7.5625f;
		const float d1 = 2.75f;

		if (x < 1.f / d1) {
			return n1 * x * x;
		}
		else if (x < 2.f / d1) {
			return n1 * (x -= 1.5f / d1) * x + 0.75f;
		}
		else if (x < 2.5f / d1) {
			return n1 * (x -= 2.25f / d1) * x + 0.9375f;
		}
		else {
			return n1 * (x -= 2.625f / d1) * x + 0.984375f;
		}
	}

	inline bool EaseVector3InOutBack(D3DXVECTOR3* pOut , D3DXVECTOR3 start, D3DXVECTOR3 end, float fElapsedTime, float fDuringTime, const float c1 = 1.70158f, float limitTime = 0.001f)
	{
		float ratio = min(fElapsedTime / fDuringTime, 1.f);
		float eased = EaseInOutBack(ratio,c1);

		D3DXVec3Lerp(pOut, &start, &end, eased);

		return (fDuringTime - fElapsedTime) < limitTime;

	}

	inline bool EaseFloatInOutBack(float* pOut, float start, float end, float fElapsedTime, float fDuringTime, float limitTime = 0.001f)
	{
		float ratio = min(fElapsedTime / fDuringTime, 1.f);
		float eased = EaseInOutBack(ratio);

		*pOut = Lerp(start, end, ratio);

		return (fDuringTime - fElapsedTime) < limitTime;

	}

	inline bool EaseFloatOutBounce(float* pOut, float start, float end, float fElapsedTime, float fDuringTime, float limitTime = 0.001f)
	{
		float ratio = min(fElapsedTime / fDuringTime, 1.f);
		float eased = EaseOutBounce(ratio);

		*pOut = Lerp(start, end, ratio);

		return (fDuringTime - fElapsedTime) < limitTime;

	}

	inline bool EaseVector3OutBounce(D3DXVECTOR3* pOut, D3DXVECTOR3 start, D3DXVECTOR3 end, float fElapsedTime, float fDuringTime, float limitTime = 0.001f)
	{
		float ratio = min(fElapsedTime / fDuringTime, 1.f);
		float eased = EaseOutBounce(ratio);

		D3DXVec3Lerp(pOut, &start, &end, eased);

		return (fDuringTime - fElapsedTime) < limitTime;
	}

	inline bool EaseVector3OutCubic(D3DXVECTOR3* pOut, D3DXVECTOR3 start, D3DXVECTOR3 end, float fElapsedTime, float fDuringTime, float limitTime = 0.001f)
	{
		float ratio = min(fElapsedTime / fDuringTime, 1.f);
		float eased = EaseOutCubic(ratio);

		D3DXVec3Lerp(pOut, &start, &end, eased);

		return (fDuringTime - fElapsedTime) < limitTime;

	}

}
#endif 