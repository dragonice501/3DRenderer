#include "InputManager.h"

#include <SDL.h>
#include <iostream>

bool InputManager::mKeyPressedW = false;
bool InputManager::mKeyHeldW = false;
bool InputManager::mKeyReleasedW = false;
float InputManager::mKeyHeldTimeW = 0.0f;

bool InputManager::mKeyPressedS = false;
bool InputManager::mKeyHeldS = false;
bool InputManager::mKeyReleasedS = false;
float InputManager::mKeyHeldTimeS = 0.0f;

bool InputManager::mKeyPressedA = false;
bool InputManager::mKeyHeldA = false;
bool InputManager::mKeyReleasedA = false;
float InputManager::mKeyHeldTimeA = 0.0f;

bool InputManager::mKeyPressedD = false;
bool InputManager::mKeyHeldD = false;
bool InputManager::mKeyReleasedD = false;
float InputManager::mKeyHeldTimeD = 0.0f;

bool InputManager::mKeyPressedL = false;
bool InputManager::mKeyPressedK = false;
bool InputManager::mKeyPressedO = false;

bool InputManager::mKeyPressedB = false;

bool InputManager::mKeyPressedEscape = false;
bool InputManager::mKeyPressedSpace = false;

bool InputManager::mKeyPressedOne = false;
bool InputManager::mKeyPressedTwo = false;
bool InputManager::mKeyPressedThree = false;
bool InputManager::mKeyPressedFour = false;
bool InputManager::mKeyPressedFive = false;
bool InputManager::mKeyPressedSix = false;

void InputManager::Update(const float dt)
{
    if (mKeyPressedW || mKeyHeldW)
    {
        mKeyPressedW = false;
        mKeyHeldW = true;
        mKeyHeldTimeW += dt;
    }
    else if (mKeyReleasedW)
    {
        mKeyReleasedW = false;
        mKeyHeldTimeW = 0.0f;
    }

    if (mKeyPressedS || mKeyHeldS)
    {
        mKeyPressedS = false;
        mKeyHeldS = true;
        mKeyHeldTimeS += dt;
    }
    else if (mKeyReleasedS)
    {
        mKeyReleasedS = false;
        mKeyHeldTimeS = 0.0f;
    }

    if (mKeyPressedA || mKeyHeldA)
    {
        mKeyPressedA = false;
        mKeyHeldA = true;
        mKeyHeldTimeA += dt;
    }
    else if (mKeyReleasedA)
    {
        mKeyReleasedA = false;
        mKeyHeldTimeA = 0.0f;
    }

    if (mKeyPressedD || mKeyHeldD)
    {
        mKeyPressedD = false;
        mKeyHeldD = true;
        mKeyHeldTimeD += dt;
    }
    else if (mKeyReleasedD)
    {
        mKeyReleasedD = false;
        mKeyHeldTimeD = 0.0f;
    }

    if (mKeyPressedL) mKeyPressedL = false;
    if (mKeyPressedK) mKeyPressedK = false;
    if (mKeyPressedO) mKeyPressedO = false;

    if (mKeyPressedB) mKeyPressedB = false;

    if (mKeyPressedEscape) mKeyPressedEscape = false;
    if (mKeyPressedSpace) mKeyPressedSpace = false;

    if (mKeyPressedOne) mKeyPressedOne = false;
    if (mKeyPressedTwo) mKeyPressedTwo = false;
    if (mKeyPressedThree) mKeyPressedThree = false;
    if (mKeyPressedFour) mKeyPressedFour = false;
    if (mKeyPressedFive) mKeyPressedFive = false;
    if (mKeyPressedSix) mKeyPressedSix = false;

    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
            case SDL_KEYDOWN:
            {
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
                {
                    mKeyPressedEscape = true;
                }

                if (sdlEvent.key.keysym.sym == SDLK_SPACE)
                {
                    mKeyPressedSpace = true;
                }

                if (sdlEvent.key.keysym.sym == SDLK_1)
                {
                    mKeyPressedOne = true;
                }

                if (sdlEvent.key.keysym.sym == SDLK_2)
                {
                    mKeyPressedTwo = true;
                }

                if (sdlEvent.key.keysym.sym == SDLK_3)
                {
                    mKeyPressedThree = true;
                }

                if (sdlEvent.key.keysym.sym == SDLK_4)
                {
                    mKeyPressedFour = true;
                }

                if (sdlEvent.key.keysym.sym == SDLK_5)
                {
                    mKeyPressedFive = true;
                }
                
                if (sdlEvent.key.keysym.sym == SDLK_6)
                {
                    mKeyPressedSix = true;
                }

                if (sdlEvent.key.keysym.sym == SDLK_w)
                {
                    if (!mKeyPressedW && !mKeyHeldW)
                    {
                        mKeyPressedW = true;
                        mKeyHeldTimeW = 0.0f;
                    }
                    break;
                }
                else if (sdlEvent.key.keysym.sym == SDLK_s)
                {
                    if (!mKeyPressedS && !mKeyHeldS)
                    {
                        mKeyPressedS = true;
                        mKeyHeldTimeS = 0.0f;
                    }
                    break;
                }
                else if (sdlEvent.key.keysym.sym == SDLK_a)
                {
                    if (!mKeyPressedA && !mKeyHeldA)
                    {
                        mKeyPressedA = true;
                        mKeyHeldTimeA = 0.0f;
                    }
                    break;
                }
                else if (sdlEvent.key.keysym.sym == SDLK_b)
                {
                    mKeyPressedB = true;
                }
                else if (sdlEvent.key.keysym.sym == SDLK_d)
                {
                    if (!mKeyPressedD && !mKeyHeldD)
                    {
                        mKeyPressedD = true;
                        mKeyHeldTimeD = 0.0f;
                    }
                    break;
                }

                if (sdlEvent.key.keysym.sym == SDLK_l)
                {
                    mKeyPressedL = true;
                    break;
                }
                else if (sdlEvent.key.keysym.sym == SDLK_k)
                {
                    mKeyPressedK = true;
                    break;
                }
                else if (sdlEvent.key.keysym.sym == SDLK_o)
                {
                    mKeyPressedO = true;
                    break;
                }
                
                break;
            }
            case SDL_KEYUP:
            {
                if (sdlEvent.key.keysym.sym == SDLK_w)
                {
                    mKeyReleasedW = true;
                    mKeyPressedW = false;
                    mKeyHeldW = false;
                    break;
                }
                else if (sdlEvent.key.keysym.sym == SDLK_s)
                {
                    mKeyReleasedS = true;
                    mKeyPressedS = false;
                    mKeyHeldS = false;
                    break;
                }
                else if (sdlEvent.key.keysym.sym == SDLK_a)
                {
                    mKeyReleasedA = true;
                    mKeyPressedA = false;
                    mKeyHeldA = false;
                    break;
                }
                else if (sdlEvent.key.keysym.sym == SDLK_d)
                {
                    mKeyReleasedD = true;
                    mKeyPressedD = false;
                    mKeyHeldD = false;
                    break;
                }
                break;
            }
        }
    }
}