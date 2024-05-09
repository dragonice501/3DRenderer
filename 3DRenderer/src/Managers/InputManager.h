#pragma once

class InputManager
{
public:
	static void Update(const float dt);

	static inline const bool KeyPressedA() { return mKeyPressedA; }
	static inline const bool KeyPressedB() { return mKeyPressedB; }
	static inline const bool KeyPressedD() { return mKeyPressedD; }
	static inline const bool KeyPressedK() { return mKeyPressedK; }
	static inline const bool KeyPressedL() { return mKeyPressedL; }
	static inline const bool KeyPressedO() { return mKeyPressedO; }
	static inline const bool KeyPressedS() { return mKeyPressedS; }
	static inline const bool KeyPressedW() { return mKeyPressedW; }
	static inline const bool KeyPressedEscape() { return mKeyPressedEscape; }
	static inline const bool KeyPressedSpace() { return mKeyPressedSpace; }
	static inline const bool KeyPressedOne() { return mKeyPressedOne; }
	static inline const bool KeyPressedTwo() { return mKeyPressedTwo; }
	static inline const bool KeyPressedThree() { return mKeyPressedThree; }
	static inline const bool KeyPressedFour() { return mKeyPressedFour; }
	static inline const bool KeyPressedFive() { return mKeyPressedFive; }
	static inline const bool KeyPressedSix() { return mKeyPressedSix; }

	static inline const bool KeyHeldA() { return mKeyHeldA; }
	static inline const bool KeyHeldD() { return mKeyHeldD; }
	static inline const bool KeyHeldS() { return mKeyHeldS; }
	static inline const bool KeyHeldW() { return mKeyHeldW; }
	
	static inline const float KeyHeldTimeA() { return mKeyHeldTimeA; }
	static inline const float KeyHeldTimeD() { return mKeyHeldTimeD; }
	static inline const float KeyHeldTimeS() { return mKeyHeldTimeS; }
	static inline const float KeyHeldTimeW() { return mKeyHeldTimeW; }

	static inline const bool KeyReleasedA() { return mKeyReleasedA; }
	static inline const bool KeyReleasedD() { return mKeyReleasedD; }
	static inline const bool KeyReleasedS() { return mKeyReleasedS; }
	static inline const bool KeyReleasedW() { return mKeyReleasedW; }

private:
	InputManager() {}
	~InputManager() {}

	static bool mKeyPressedA;
	static bool mKeyPressedB;
	static bool mKeyPressedD;
	static bool mKeyPressedK;
	static bool mKeyPressedL;
	static bool mKeyPressedO;
	static bool mKeyPressedS;
	static bool mKeyPressedW;
	static bool mKeyPressedEscape;
	static bool mKeyPressedSpace;
	static bool mKeyPressedOne;
	static bool mKeyPressedTwo;
	static bool mKeyPressedThree;
	static bool mKeyPressedFour;
	static bool mKeyPressedFive;
	static bool mKeyPressedSix;

	static bool mKeyHeldA;
	static bool mKeyHeldD;
	static bool mKeyHeldS;
	static bool mKeyHeldW;

	static float mKeyHeldTimeA;
	static float mKeyHeldTimeD;
	static float mKeyHeldTimeS;
	static float mKeyHeldTimeW;
	
	static bool mKeyReleasedA;
	static bool mKeyReleasedD;
	static bool mKeyReleasedS;
	static bool mKeyReleasedW;
};