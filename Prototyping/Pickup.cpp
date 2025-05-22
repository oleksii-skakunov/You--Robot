#include "pch.h"
#include "Pickup.h"

Pickup::Pickup(PickupType type, const Vector2f& position)
    : m_Type(type)
{
    // Size is 2/3 of player size (player is approximately 42.5 x 40)
    float width = 28.f;
    float height = 26.f;
    
    // Center the pickup at the given position
    m_Bounds = Rectf{
        position.x - width / 2,
        position.y - height / 2,
        width,
        height
    };
}

void Pickup::Draw() const
{
    // Draw pickup based on type
    switch (m_Type)
    {
        case PickupType::AMMO:
            // Orange for ammo
            utils::SetColor(Color4f{1.0f, 0.5f, 0.0f, 1.0f});
            break;
        case PickupType::HEALTH:
            // Pink for health
            utils::SetColor(Color4f{1.0f, 0.4f, 0.7f, 1.0f});
            break;
    }
    
    // Draw as a square
    utils::FillRect(m_Bounds);
}

bool Pickup::IsColliding(const Rectf& playerBounds) const
{
    // Check if the pickup collides with the player
    return utils::IsOverlapping(m_Bounds, playerBounds);
}
