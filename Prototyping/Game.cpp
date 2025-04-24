#include "pch.h"
#include "Game.h"
// all issues atm:
// bullets spawn from bottom left of the character and don't go directly to the cursor

// ideas
// you can earn defective points by breaking the laws yourself, but then you get a shit ton of debufs
Game::Game( const Window& window ) 
	:BaseGame{ window },
	m_Player{}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize()
{
	m_VerticiesLevel.push_back(std::vector < Vector2f>{
		Vector2f{5.f,5.f},
			Vector2f{GetViewPort().width - 5.f, 5.f},
			Vector2f(GetViewPort().width - 5.f, GetViewPort().height - 5.f),
			Vector2f(5.f, GetViewPort().height - 5.f)
	});
	std::vector<std::vector<Vector2f>> m_VerticiesTarget{};
	std::vector<std::vector<Vector2f>> m_VerticiesNonTarget{};
}

void Game::Cleanup( )
{
	m_BulletManager.DeleteAll();
}

void Game::Update( float elapsedSec )
{
	m_BulletManager.Update(elapsedSec, m_NPCManager);
	m_NPCManager.Update(elapsedSec);
	// Check keyboard state
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	if ( pStates[SDL_SCANCODE_W] )
	{
		m_Player.Update(Vector2f{ 0.f, 250.f }, elapsedSec);
	}
	if (pStates[SDL_SCANCODE_A])
	{
		m_Player.Update(Vector2f{ -250.f, 0.f }, elapsedSec);
	}
	if (pStates[SDL_SCANCODE_S])
	{
		m_Player.Update(Vector2f{ 0.f, -250.f }, elapsedSec);
	}
	if (pStates[SDL_SCANCODE_D])
	{
		m_Player.Update(Vector2f{ 250.f, 0.f }, elapsedSec);
	}

}

void Game::Draw( ) const
{
	ClearBackground( );
	m_Player.Draw();
	m_BulletManager.Draw();
	m_NPCManager.Draw();
	utils::SetColor(Color4f{ 1.f,1.f,1.f,1.f });
	for (size_t i = 0; i < m_VerticiesLevel.size(); i++)
	{
		for (size_t o = 0; o < m_VerticiesLevel[i].size(); o++)
		{
			utils::DrawLine(m_VerticiesLevel[i][o], m_VerticiesLevel[i][(o + 1) % m_VerticiesLevel[i].size()]);
		}
	}
	
	// debug stuff
	utils::SetColor(Color4f{ 1.f,0.f,0.f,1.f });
	for (size_t i = 0; i < m_VerticiesTarget.size(); i++)
	{
		for (size_t o = 0; o < m_VerticiesTarget[i].size(); o++)
		{
			utils::DrawLine(m_VerticiesTarget[i][o], m_VerticiesTarget[i][(o + 1)% m_VerticiesTarget[i].size()]);
		}
	}
	utils::SetColor(Color4f{ 0.f,0.f,1.f,1.f });
	for (size_t i = 0; i < m_VerticiesNonTarget.size(); i++)
	{
		for (size_t o = 0; o < m_VerticiesNonTarget[i].size(); o++)
		{
			utils::DrawLine(m_VerticiesNonTarget[i][o], m_VerticiesNonTarget[i][(o + 1) % m_VerticiesNonTarget[i].size()]);
		}
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	switch ( e.keysym.sym )
	{
	case SDLK_1:
		m_Player.DebugSetLaw();
		//std::cout << "Left arrow key released\n";
		break;
	//case SDLK_RIGHT:
		//std::cout << "`Right arrow key released\n";
		break;
	//case SDLK_1:
	//case SDLK_KP_1:
		//std::cout << "Key 1 released\n";
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	switch (e.button)
	{
	default:
		break;
	case SDL_BUTTON_LEFT:
		m_NPCManager.UpdateVector(m_VerticiesTarget, Team::Target);
		m_NPCManager.UpdateVector(m_VerticiesNonTarget, Team::NonTarget);
		if (m_Player.Shoot(Vector2f{ float(e.x), float(e.y) }, m_VerticiesTarget, m_VerticiesLevel, m_VerticiesNonTarget))
		{
			m_BulletManager.AddBullet(m_Player.GetBulletType(), m_Player.GetCenterPos(), Vector2f{ float(e.x), float(e.y) }, Team::Neutral);
		}
		
		break;
	case SDL_BUTTON_RIGHT:
		//for debug purposes
		m_NPCManager.AddNPC(2, Vector2f{ float(e.x), float(e.y) }, m_VerticiesTarget);
		break;
	case SDL_BUTTON_MIDDLE:
		m_NPCManager.UpdateVector(m_VerticiesTarget, Team::Target);
		m_NPCManager.UpdateVector(m_VerticiesNonTarget, Team::NonTarget);
		m_NPCManager.AddNPC(1, Vector2f{ float(e.x), float(e.y) }, m_VerticiesNonTarget);
		break;
	}
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
