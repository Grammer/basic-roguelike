#include <iostream>
#include <Windows.h>
#include <cstdarg>

#include "base.h"
#include "list.h"
#include "gui.h"
#include "base_character.h"
#include "world.h"
#include "engine.h"

Gui::Gui() : Text(), HUD()
{

}

Gui::~Gui()
{
    log.DeleteAll();
}

void Gui::ShowMessage( int iValue, int iItem )
{
    COORD screen;
    HANDLE console = GetStdHandle( STD_OUTPUT_HANDLE );

    screen.X = engine.pWorld->m_iWidth + 2;
    screen.Y = m_iTextScreenHeight + 2;

    for ( Message **i = log.Begin(); i != log.End(); i++ )
    {
        Message *message = *i;
        SetConsoleCursorPosition( console, screen );
        std::cout << message->text << iValue
            << ( iItem == World::POTION_OF_HEALTH ? " HP" : " STR" ) << std::endl;
    }
}

Text::Message::Message( const char* text ) : text( _strdup( text ))
{

}

Text::Message::~Message()
{
    free( text );
}

void Text::MessageInterception( const char* text, ... )
{
    va_list args;
    char msg[128], *line = msg, *eol;

    va_start( args, text );
    vsprintf_s( msg, text, args );
    va_end( args );

    do
    {
        eol = strchr( line, '\n' );
        if ( eol )
            *eol = 0;

        Message *message = new Message( line );
        log.Push( message );
    }
    while ( eol );
}

void Text::ShowStats( void )
{
    COORD screen;
    HANDLE console = GetStdHandle( STD_OUTPUT_HANDLE );

    screen.X = engine.pWorld->m_iWidth + 2;
    screen.Y = 0;
    SetConsoleCursorPosition( console, screen );
    std::cout << "Level #" << engine.pWorld->m_iCurLevel;

    for ( int i = 2; i < 6; i++ )
    {
        screen.Y = m_iTextScreenHeight = i;
        SetConsoleCursorPosition( console, screen );
        switch( i )
        {
        case 2 :
            std::cout << "Player\t    Enemy" << ( GET_ENEMY->GetHealth( BaseCharacter::ENEMY ) < 1 ? " (Dead)" : " " );
            break;
        case 4 :
            std::cout << "HP: " << engine.pPlayer->m_iHealth << "/" << engine.pPlayer->m_iNewHealth
                << ( engine.pPlayer->m_iHealth < 100 ? "    " : "   " ) << "HP: " << ( GET_ENEMY->GetHealth( BaseCharacter::ENEMY ) < 1 ? NULL : GET_ENEMY->m_iHealth )
                << "/" << BaseCharacter::DEFAULT_ENEMY_HEALTH << "   ";
            break;
        case 5 :
            std::cout << "STR: " << engine.pPlayer->m_iStrength << "\t    STR: " << GET_ENEMY->m_iStrength;
            break;
        default:
            break;
        }
    }
}

void HUD::RenderHUD( void )
{
    COORD screen;
    HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );

    screen.X = engine.pWorld->m_iWidth + 1;

    for ( int i = 0; i < MAX_HUD_HEIGTH; i++ )
    {
        screen.Y = i;
        SetConsoleCursorPosition( handle, screen );
        switch( i )
        {
        case 0 :
            std::cout << (char_t)TOP_LEFT_CORNER;
            for ( int j = 0; j < strlen( _DEBUG_STRING ) + 1; j++ )
                std::cout << (char_t)DOUBLE_HORIZ_WALL;
            std::cout << (char_t)TOP_RIGHT_CORNER;
            break;
        case 1:
            std::cout << (char_t)DOUBLE_VERT_WALL;

            break;
        }
        //std::cout << std::endl << "HP: 100/100\tHP: 20/20   ";

    }

}
