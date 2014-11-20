#include <iostream>
#include <ctime>
#include <Windows.h>

#include "list.h"
#include "gui.h"
#include "base.h"
#include "base_character.h"
#include "world.h"
#include "engine.h"

World::World( int iWidth, int iHeigth ) : m_iWidth( iWidth ), m_iHeigth( iHeigth )
{
    pTile = new Tile[m_iWidth * m_iHeigth];
    for ( int x = 0, y = 0; x < m_iWidth, y < m_iHeigth; x++, y++ )
    {
        SetWall( x, 0 );
        SetWall( x, m_iWidth - 1 );
        SetWall( 0, y );
        SetWall( m_iHeigth - 1, y );
    }

    m_iCurLevel = 1; m_iPrevLevel = m_nEnemies = 0;
    m_bOnNextLevel = false;
}

World::~World()
{
    delete []pTile;
}

void World::SetWall( int iPosX, int iPosY )
{
    pTile[iPosX + iPosY * m_iWidth].m_bCanWalk = false;
}

bool World::IsWall( int iPosX, int iPosY ) const
{
    return !pTile[iPosX + iPosY * m_iWidth].m_bCanWalk;
}

bool World::CanWalk( int iPosX, int iPosY ) const
{
    if ( IsWall( iPosX, iPosY ))
        return false;

    for ( BaseCharacter **i = engine.characters.Begin();
        i != engine.characters.End(); i++ )
    {
        BaseCharacter *character = *i;
        if ( character->m_iPosX == iPosX && character->m_iPosY == iPosY )
            return false;
        if ( engine.pChest->m_bIsAlive )
        {
            if ( iPosX == engine.pChest->m_iPosX && iPosY == engine.pChest->m_iPosY )
                return true;
        }
    }

    return true;
}

void World::Load( void )
{
    for ( int x = 0; x < m_iWidth; x++ )
    {
        for ( int y = 0; y < m_iHeigth; y++ )
            std::cout << ( IsWall( x, y ) ? (char_t)WALL : (char_t)TILE );
        std::cout << std::endl;
    }

    GenerateExit();
}

void World::SpawnEnemy( void )
{
    srand( static_cast<int32_t>( time( 0 )));
    int x, y;

    do
    {
        x = 3 + rand() % ( m_iWidth - 4 );
        y = 3 + rand() % ( m_iHeigth - 4 );
    }
    while( !CanWalk( x, y ) || (( x == m_iExitPosX ) && ( y == m_iExitPosY ))
        || ( x == engine.pChest->m_iPosX && y == engine.pChest->m_iPosY));

    if ( engine.pWorld->m_bOnNextLevel )
        engine.characters.Pop();
    engine.characters.Push( new BaseCharacter( x, y, x, y, BaseCharacter::ENEMY ));

    GET_ENEMY->m_iPrevPosX = x; GET_ENEMY->m_iPrevPosY = y;

    GET_ENEMY->m_iHealth = BaseCharacter::DEFAULT_ENEMY_HEALTH;
    GET_ENEMY->m_iStrength = BaseCharacter::DEFAULT_ENEMY_STRENGTH;
}

void World::GenerateExit( void )
{
    srand( static_cast<int32_t>( time( 0 )));

    m_iExitPosX = m_iWidth - 4 + rand() % 2;
    m_iExitPosY = m_iWidth - 4 + rand() % 2;
}

void World::LoadNextLevel( void )
{
    system( "cls" );
    engine.pWorld->m_bOnNextLevel = true;

    engine.pPlayer->m_iPosX = engine.pPlayer->m_iPosY = 1;

    SpawnChest();
    SpawnEnemy();

    if ( PLAYER_ALIVE )
    {
        m_iPrevLevel = m_iCurLevel;
        m_iCurLevel++;
        GET_ENEMY->UpdateStrength( 0, 3 * m_iPrevLevel );
    }
    else
        engine.Restart();

    GET_ENEMY->RestoreHealth( BaseCharacter::ENEMY );

    Load();
    engine.gui->ShowStats();
}

void World::SpawnChest( void )
{
    srand( static_cast<int32_t>( time( 0 )));
    int x, y;

    do
    {
        x = 1 + rand() % ( m_iWidth - 2 );
        y = 1 + rand() % ( m_iHeigth - 2 );
    }
    while( !CanWalk( x, y ) /*|| ( x == 1 && y == 1 )*/ || ( x == m_iExitPosX && y == m_iExitPosY ));

    engine.pChest->m_iPosX = x; engine.pChest->m_iPosY = y;

    engine.pChest->m_bChestOpened = false;
    engine.pChest->m_bIsAlive = true;
}
