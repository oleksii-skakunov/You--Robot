#pragma once
#include "Texture.h"
#include "utils.h"
#include <vector>

class EscapePod
{
public:
    enum class State
    {
        Inactive,
        LandingTarget,
        LandingAnimation,
        Active,
        ExitAnimation,
        Exfil
    };

    EscapePod();
    void Update(float elapsedSec, const Vector2f& playerPos, const std::vector<std::vector<Vector2f>>& levelVertices);
    void Draw(const Vector2f& playerPos) const;
    void DebugDraw() const;
    void StartLanding(const Vector2f& targetPos, const std::vector<std::vector<Vector2f>>& levelVertices);
    bool IsActive() const;
    bool IsInteractable() const;
    bool IsPlayerInRange(const Vector2f& playerPos) const;
    void StartExit();
    bool IsExiting() const;
    State GetState() const { return m_State; }
    void SetState(State newState) { m_State = newState; }

private:
    State m_State;
    Vector2f m_Position;
    Rectf m_Bounds;
    Rectf m_InteractBounds;
    Texture m_TargetTexture;
    Texture m_InteractText;
    float m_AnimationTimer;
    float m_AnimationDuration;
    bool m_IsExiting;

    bool TryLandAtPosition(const Vector2f& pos, const std::vector<std::vector<Vector2f>>& levelVertices);
    bool IsValidPosition(const Vector2f& pos, const std::vector<std::vector<Vector2f>>& levelVertices) const;
    void PlayAnimation(float duration);
}; 