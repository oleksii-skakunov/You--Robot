#include "pch.h"
#include "Game.h"

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

void Game::Initialize( )
{
	
}

void Game::Cleanup( )
{
	m_BulletManager.DeleteAll();
}

void Game::Update( float elapsedSec )
{
	m_BulletManager.Update(elapsedSec);
	// Check keyboard state
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	if ( pStates[SDL_SCANCODE_W] )
	{
		m_Player.Move(Vector2f{ 0.f, 150.f }, elapsedSec);
	}
	if (pStates[SDL_SCANCODE_A])
	{
		m_Player.Move(Vector2f{ -150.f, 0.f }, elapsedSec);
	}
	if (pStates[SDL_SCANCODE_S])
	{
		m_Player.Move(Vector2f{ 0.f, -150.f }, elapsedSec);
	}
	if (pStates[SDL_SCANCODE_D])
	{
		m_Player.Move(Vector2f{ 150.f, 0.f }, elapsedSec);
	}

}

void Game::Draw( ) const
{
	ClearBackground( );
	m_Player.Draw();
	m_BulletManager.Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
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
		if (m_Player.Shoot(Vector2f{ float(e.x), float(e.y) }))
		{
			m_BulletManager.AddBullet(m_Player.GetBulletType(), m_Player.GetPos(), Vector2f{ float(e.x), float(e.y) }, 0);
		}
		
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
