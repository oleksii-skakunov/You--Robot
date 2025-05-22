#include "pch.h"
#include "EscapePod.h"
#include <random>

EscapePod::EscapePod() :
    m_State(State::Inactive),
    m_Position(0.f, 0.f),
    m_Bounds(0.f, 0.f, 50.f, 50.f), // Default size
    m_InteractBounds(0.f, 0.f, 100.f, 100.f), // Larger interact area
    m_TargetTexture("Target.png"),
    m_InteractText("Press E to exfiltrate", "Font.ttf", 20, Color4f{1.f, 1.f, 1.f, 1.f}),
    m_AnimationTimer(0.f),
    m_AnimationDuration(0.f),
    m_IsExiting(false)
{
}

void EscapePod::Update(float elapsedSec, const Vector2f& playerPos, const std::vector<std::vector<Vector2f>>& levelVertices)
{
    switch (m_State)
    {
    case State::LandingTarget:
        m_AnimationTimer += elapsedSec;
        if (m_AnimationTimer >= 2.f) // 2 seconds for target flash
        {
            m_State = State::LandingAnimation;
            PlayAnimation(3.f); // 3 seconds for landing animation
        }
        break;

    case State::LandingAnimation:
        m_AnimationTimer += elapsedSec;
        if (m_AnimationTimer >= m_AnimationDuration)
        {
            m_State = State::Active;
            m_AnimationTimer = 0.f;
        }
        break;

    case State::ExitAnimation:
        m_AnimationTimer += elapsedSec;
        if (m_AnimationTimer >= m_AnimationDuration)
        {
            m_State = State::Exfil;
            m_AnimationTimer = 0.f;
            m_IsExiting = false;
        }
        break;
    }
}

void EscapePod::Draw(const Vector2f& playerPos) const
{
    if (m_State == State::Inactive) return;

    // Draw the escape pod

    // Draw target during landing target phase
    if (m_State == State::LandingTarget)
    {
        //float alpha = (sin(m_AnimationTimer * 10.f) + 1.f) / 2.f; // Flashing effect
        //utils::SetColor(Color4f{1.f, 1.f, 1.f, alpha});
        m_TargetTexture.Draw(Rectf(m_Bounds.left - 25.f, m_Bounds.bottom, m_TargetTexture.GetWidth(), m_TargetTexture.GetHeight()));
    }

    // Draw interact text if player is in range and pod is active
    if (m_State == State::Active && IsPlayerInRange(playerPos))
    {
        Vector2f textPos{
            m_Position.x - m_InteractText.GetWidth() / 2.f,
            m_Position.y - m_Bounds.height - 30.f
        };
        m_InteractText.Draw(textPos);
    }
}

void EscapePod::DebugDraw() const
{
    if (m_State == State::Inactive or m_State == State::LandingTarget) return;

    // Draw the escape pod bounds in green
    utils::SetColor(Color4f{0.f, 1.f, 0.f, 1.f});
    utils::FillRect(m_Bounds);

    // Draw the interaction bounds in semi-transparent green
    utils::SetColor(Color4f{0.f, 1.f, 0.f, 0.3f});
    utils::FillRect(m_InteractBounds);
}

void EscapePod::StartLanding(const Vector2f& targetPos, const std::vector<std::vector<Vector2f>>& obstacles)
{
    // Calculate spawn position above the target
    float viewportWidth = 800.f; // TODO: Get from viewport
    float viewportHeight = 600.f; // TODO: Get from viewport
    
    // Keep away from edges
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> posX(100.f, viewportWidth - 100.f);
    std::uniform_real_distribution<> posY(100.f, viewportHeight - 100.f);

    // Try up to 30 random positions
    for (int i = 0; i < 30; ++i)
    {
        Vector2f randomPos{float(posX(gen)), float(posY(gen))};
        if (TryLandAtPosition(randomPos, obstacles))
        {
            m_State = State::LandingTarget;
            m_AnimationTimer = 0.f;
            return;
        }
    }

    // If no valid position found, try the original target position as fallback
    if (TryLandAtPosition(targetPos, obstacles))
    {
        m_State = State::LandingTarget;
        m_AnimationTimer = 0.f;
        return;
    }

    // If still no valid position, try surrounding positions
    const float offset = 50.f;
    const Vector2f offsets[] = {
        Vector2f{0.f, offset},    // Above
        Vector2f{offset, 0.f},    // Right
        Vector2f{0.f, -offset},   // Below
        Vector2f{-offset, 0.f},   // Left
        Vector2f{offset, offset}, // Top-right
        Vector2f{offset, -offset},// Bottom-right
        Vector2f{-offset, -offset},// Bottom-left
        Vector2f{-offset, offset} // Top-left
    };

    for (const auto& offset : offsets)
    {
        if (TryLandAtPosition(targetPos + offset, obstacles))
        {
            m_State = State::LandingTarget;
            m_AnimationTimer = 0.f;
            return;
        }
    }
}

bool EscapePod::IsActive() const
{
    return m_State == State::Active;
}

bool EscapePod::IsInteractable() const
{
    return m_State == State::Active;
}

bool EscapePod::IsPlayerInRange(const Vector2f& playerPos) const
{
    return utils::IsPointInRect(playerPos, m_InteractBounds);
}

void EscapePod::StartExit()
{
    if (m_State == State::Active)
    {
        m_State = State::ExitAnimation;
        PlayAnimation(3.f); // 3 seconds for exit animation
        m_IsExiting = true;
    }
}

bool EscapePod::IsExiting() const
{
    return m_IsExiting;
}

bool EscapePod::TryLandAtPosition(const Vector2f& pos, const std::vector<std::vector<Vector2f>>& levelVertices)
{
    if (!IsValidPosition(pos, levelVertices)) return false;

    m_Position = pos;
    m_Bounds.left = pos.x - m_Bounds.width / 2;
    m_Bounds.bottom = pos.y - m_Bounds.height / 2;
    m_InteractBounds.left = pos.x - m_InteractBounds.width / 2;
    m_InteractBounds.bottom = pos.y - m_InteractBounds.height / 2;
    return true;
}

bool EscapePod::IsValidPosition(const Vector2f& pos, const std::vector<std::vector<Vector2f>>& levelVertices) const
{
    Rectf testBounds{
        pos.x - m_Bounds.width / 2,
        pos.y - m_Bounds.height / 2,
        m_Bounds.width,
        m_Bounds.height
    };

    // Check if any corner is inside an obstacle
    Vector2f bottomLeft{testBounds.left, testBounds.bottom};
    Vector2f bottomRight{testBounds.left + testBounds.width, testBounds.bottom};
    Vector2f topLeft{testBounds.left, testBounds.bottom + testBounds.height};
    Vector2f topRight{testBounds.left + testBounds.width, testBounds.bottom + testBounds.height};

    for (const auto& obstacle : levelVertices)
    {
        if (utils::IsPointInPolygon(bottomLeft, obstacle) ||
            utils::IsPointInPolygon(bottomRight, obstacle) ||
            utils::IsPointInPolygon(topLeft, obstacle) ||
            utils::IsPointInPolygon(topRight, obstacle))
        {
            return false;
        }
    }
    return true;
}

void EscapePod::PlayAnimation(float duration)
{
    m_AnimationTimer = 0.f;
    m_AnimationDuration = duration;
} 