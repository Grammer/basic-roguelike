#include <iostream>
#include <cmath>
#include <ctime>
#include <sstream>
#include <Windows.h>

#include "list.h"
#include "gui.h"
#include "base.h"
#include "gui.h"
#include "world.h"
#include "base_character.h"
#include "engine.h"

#define _CRT_SECURE_NO_WARNINGS

BaseCharacter::BaseCharacter( int x, int y, int x0, int y0, char c )
    : m_iPosX( x ), m_iPosY ( y ), m_iPrevPosX( x0 ), m_iPrevPosY( y0 ), m_cEntity( c )
{
    m_iPrevPosX = m_iPrevPosY = 1;
    m_bIsAlive = true;
    m_bChestOpened = m_bPlayerOnChest = false;

    pLoot[0] = &BaseCharacter::SpawnHealth;
    pLoot[1] = &BaseCharacter::SpawnStrength;
}

void BaseCharacter::Draw( void ) const
{
    COORD cCoord;
    HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

    cCoord.X = m_iPrevPosX;
    cCoord.Y = m_iPrevPosY;

    SetConsoleCursorPosition( hConsole, cCoord );
    std::cout << (char_t)World::TILE;

    cCoord.X = engine.pWorld->m_iExitPosX;
    cCoord.Y = engine.pWorld->m_iExitPosY;

    SetConsoleCursorPosition( hConsole, cCoord );
    std::cout << (char_t)World::EXIT;

    cCoord.X = m_iPosX;
    cCoord.Y = m_iPosY;

    SetConsoleCursorPosition( hConsole, cCoord );

    if ( m_cEntity == (char_t)ENEMY && ( GET_ENEMY->GetHealth( ENEMY ) < 1 ))
        std::cout << (char_t)World::GRAVE;
    else if ( m_cEntity == (char_t)World::CHEST && engine.pChest->m_bChestOpened && engine.pPlayer->m_bPlayerOnChest )
        std::cout << (char_t)PLAYER;
    else if ( m_cEntity == (char_t)World::CHEST && engine.pChest->m_bChestOpened )
        std::cout << (char_t)World::TILE;
    else
        std::cout << m_cEntity;
}

bool BaseCharacter::SelectAxisMovement( void )
{
    for ( BaseCharacter **i = engine.characters.Begin() + 2; i != engine.characters.End(); i++ )
    {
        BaseCharacter *enemy = *i;
        if ( abs( enemy->m_iPosX - engine.pPlayer->m_iPosX ) >= abs( enemy->m_iPosY - engine.pPlayer->m_iPosY ))
        {
            if ( enemy->m_iPosX - engine.pPlayer->m_iPosX > 1 )
                m_iSign = -1;
            else if ( enemy->m_iPosX - engine.pPlayer->m_iPosX < 0 )
                m_iSign = 1;
            return true;
        }
        else
        {
            if ( enemy->m_iPosY - engine.pPlayer->m_iPosY > 1 )
                m_iSign = -1;
            else if ( enemy->m_iPosY - engine.pPlayer->m_iPosY < 0 )
                m_iSign = 1;
            return false;
        }
    }
}

void BaseCharacter::XAxisMovement( void )
{
    if ( !IsNextToPlayer() )
    {
        if ( SelectAxisMovement() )
        {
            //for ( BaseCharacter **i = engine.characters.Begin() + 2; i != engine.characters.End(); i++ )
            //{
                //BaseCharacter *enemy = *i;
                if (((( GET_ENEMY->m_iPosX - (-1) * m_iSign ) != engine.pWorld->m_iExitPosX )
                    || ( GET_ENEMY->m_iPosY != engine.pWorld->m_iExitPosY ))
                    && engine.pWorld->CanWalk(( GET_ENEMY->m_iPosX - (-1) * m_iSign ), GET_ENEMY->m_iPosY ))
                {
                    GET_ENEMY->m_iPosX = GET_ENEMY->m_iPosX - (-1) * m_iSign;
                }
            //}
        }
    }
}

void BaseCharacter::YAxisMovement( void )
{
    if ( !IsNextToPlayer() )
    {
        if ( !SelectAxisMovement() )
        {
            //for ( BaseCharacter **i = engine.characters.Begin() + 2; i != engine.characters.End(); i++ )
            //{
                //BaseCharacter *enemy = *i;
            if (((( GET_ENEMY->m_iPosY - (-1) * m_iSign ) != engine.pWorld->m_iExitPosY )
                    || ( GET_ENEMY->m_iPosX != engine.pWorld->m_iExitPosX ))
                    && engine.pWorld->CanWalk( GET_ENEMY->m_iPosX, ( GET_ENEMY->m_iPosY - (-1) * m_iSign )))
                {
                    GET_ENEMY->m_iPosY = GET_ENEMY->m_iPosY - (-1) * m_iSign;
                }
            //}
        }
    }
}

bool BaseCharacter::IsNextToPlayer( void )
{
    for ( BaseCharacter **i = engine.characters.Begin() + 2; i != engine.characters.End(); i++ )
    {
        BaseCharacter *enemy = *i;
        if (( abs( enemy->m_iPosX - engine.pPlayer->m_iPosX ) > 1 ) || ( abs( enemy->m_iPosY - engine.pPlayer->m_iPosY ) > 1 ))
            return false;
        else
            return true;
    }
}

void BaseCharacter::Attack( void )
{
    UpdateHealth( -GET_ENEMY->m_iStrength, -engine.pPlayer->m_iStrength );
    engine.gui->ShowStats();
}

void BaseCharacter::UpdateHealth( int iPlayerHealth, int iEnemyHealth )
{
    engine.pPlayer->m_iHealth += iPlayerHealth;
    GET_ENEMY->m_iHealth += iEnemyHealth;

    if (( iPlayerHealth > 0 ) && ( engine.pPlayer->m_iNewHealth < engine.pPlayer->m_iHealth ))
    {
        engine.pPlayer->m_iNewHealth = engine.pPlayer->m_iHealth;
    }
}

void BaseCharacter::UpdateStrength( int iPlayerStrength, int iEnemyStrength )
{
    engine.pPlayer->m_iStrength += iPlayerStrength;
    GET_ENEMY->m_iStrength += iEnemyStrength;
    GET_ENEMY->m_iPrevStrength = GET_ENEMY->m_iStrength;
}

void BaseCharacter::RestoreHealth( int iEntity )
{
    ( iEntity == PLAYER ? ( engine.pPlayer->m_iHealth = DEFAULT_PLAYER_HEALTH ) : ( GET_ENEMY->m_iHealth = DEFAULT_ENEMY_HEALTH ));
}

void BaseCharacter::RestoreStrength( void )
{
    engine.pPlayer->m_iStrength = DEFAULT_PLAYER_STRENGTH;
    GET_ENEMY->m_iStrength = DEFAULT_ENEMY_STRENGTH;
}

int BaseCharacter::GetHealth( int iEntity )
{
    return ( iEntity == PLAYER ? engine.pPlayer->m_iHealth : GET_ENEMY->m_iHealth );
}

int BaseCharacter::GetStrength( void )
{
    return GET_ENEMY->m_iPrevStrength;
}

void BaseCharacter::GetLoot( void )
{
    srand( static_cast<int32_t>( time( 0 )));

    (this->*pLoot[rand() % 2])();

    m_bChestOpened = true;

    engine.gui->ShowStats();
    engine.gui->MessageInterception( "You have looted " );
}

void BaseCharacter::SpawnHealth( void )
{
    srand( static_cast<int32_t>( time( 0 )));
    int i = 10 + rand() % 50;

    UpdateHealth( i, 0 );

    engine.gui->ShowMessage( i, World::POTION_OF_HEALTH );
}

void BaseCharacter::SpawnStrength( void )
{
    srand( static_cast<int32_t>( time( 0 )));
    int i = 1 + rand() % 5;

    UpdateStrength( 1 + rand() % 5, 0 );

    engine.gui->ShowMessage( i, World::POTION_OF_STRENGTH );
}
