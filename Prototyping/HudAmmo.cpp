#include "pch.h"
#include "HudAmmo.h"

HudAmmo::HudAmmo(Texture* texture, Rectf pos, int currentAmmo, int maxAmmo) :
    HudObject(texture, pos),
    m_CurrentAmmo(currentAmmo),
    m_MaxAmmo(maxAmmo),
    m_StartAmmo(currentAmmo),
    m_TextTexture(nullptr)
{
    // Create initial text display
    std::string ammoText = "Ammo: " + std::to_string(m_CurrentAmmo) + " / " + std::to_string(m_MaxAmmo);
    m_TextTexture = new Texture(ammoText, "Font.ttf", 16, Color4f{ 1.f, 1.f, 1.f, 1.f });
}

void HudAmmo::Draw() const
{
    // Draw background
    HudObject::Draw();
    
    // Draw text
    if (m_TextTexture != nullptr)
    {
        // Position the text inside the ammo display
        m_TextTexture->Draw(Vector2f{ m_Pos.left + 5.f, m_Pos.bottom + 3.f });
    }
}

void HudAmmo::Update(int currentAmmo)
{
    // Update current ammo
    m_CurrentAmmo = currentAmmo;
    
    // Update display text
    delete m_TextTexture;
    std::string ammoText = "Ammo: " + std::to_string(m_CurrentAmmo) + " / " + std::to_string(m_MaxAmmo);
    m_TextTexture = new Texture(ammoText, "Font.ttf", 16, Color4f{ 1.f, 1.f, 1.f, 1.f });
}

int HudAmmo::GetCurrentAmmo() const
{
    return m_CurrentAmmo;
}

int HudAmmo::GetMaxAmmo() const
{
    return m_MaxAmmo;
}

void HudAmmo::SetCurrentAmmo(int ammo)
{
    m_CurrentAmmo = ammo;
}

void HudAmmo::SetMaxAmmo(int ammo)
{
    m_MaxAmmo = ammo;
}

void HudAmmo::Restart(int startAmmo)
{
    m_CurrentAmmo = startAmmo;
    m_StartAmmo = startAmmo;
    
    // Update display text
    delete m_TextTexture;
    std::string ammoText = "Ammo: " + std::to_string(m_CurrentAmmo) + " / " + std::to_string(m_MaxAmmo);
    m_TextTexture = new Texture(ammoText, "Font.ttf", 16, Color4f{ 1.f, 1.f, 1.f, 1.f });
} 