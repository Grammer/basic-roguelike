#include <iostream>
#include <conio.h>

#include "base.h"
#include "list.h"
#include "gui.h"
#include "base_character.h"
#include "world.h"
#include "engine.h"

Engine::Engine()
{
    pWorld = new World( 12, 12 );
    pWorld->Load();

    pPlayer = new BaseCharacter( DEFAULT_PLAYER_SPAWN, DEFAULT_PLAYER_SPAWN, BaseCharacter::PLAYER );
    characters.Push( pPlayer );
    pPlayer->m_iHealth = pPlayer->m_iNewHealth = BaseCharacter::DEFAULT_PLAYER_HEALTH;
    pPlayer->m_iStrength = BaseCharacter::DEFAULT_PLAYER_STRENGTH;

    pChest = new BaseCharacter( DEFAULT_PLAYER_SPAWN, DEFAULT_PLAYER_SPAWN, World::CHEST );
    characters.Push( pChest );
    pWorld->SpawnChest();

    pWorld->SpawnEnemy();

    SavePreviousPosition();

    gui = new Gui();
//	gui->RenderHUD();
    gui->ShowStats();
}

Engine::~Engine()
{
    characters.DeleteAll();

    delete gui;
    delete pWorld;
}

void Engine::Update( void )
{
    char_t cKey;

    Render();

    while( true )
    {
        cKey = _getch();
        switch( cKey )
        {
        case 72: // up
            if ( pWorld->CanWalk( pPlayer->m_iPosX, pPlayer->m_iPosY - 1 ))
                pPlayer->m_iPosY--;
            break;
        case 80: // down
            if ( pWorld->CanWalk( pPlayer->m_iPosX, pPlayer->m_iPosY + 1 ))
                pPlayer->m_iPosY++;
            break;
        case 75: // left
            if ( pWorld->CanWalk( pPlayer->m_iPosX - 1, pPlayer->m_iPosY ))
                pPlayer->m_iPosX--;
            break;
        case 77: // right
            if ( pWorld->CanWalk( pPlayer->m_iPosX + 1, pPlayer->m_iPosY ))
                pPlayer->m_iPosX++;
            break;
        default:
            SavePreviousPosition();
            continue;
        }
        if ( pPlayer->m_iPosX == pChest->m_iPosX && pPlayer->m_iPosY == pChest->m_iPosY )
        {
            if ( !pChest->m_bChestOpened )
            {
                pChest->GetLoot();
            }
            pPlayer->m_bPlayerOnChest = true;
        }
        else
            pPlayer->m_bPlayerOnChest = false;
        if ( GET_ENEMY->GetHealth( BaseCharacter::ENEMY ) > 0 )
        {
            for ( BaseCharacter **i = characters.Begin() + 2; i != characters.End(); i++ )
            {
                BaseCharacter *enemy = *i;
                enemy->XAxisMovement();
                enemy->YAxisMovement();

                if ( enemy->IsNextToPlayer() )
                    enemy->Attack();
            }
        }
        Render();
    }
}

void Engine::Render()
{
    if ( ON_CHANGELEVEL || pPlayer->GetHealth( BaseCharacter::PLAYER ) < 0 )
    {
        pWorld->LoadNextLevel();
    }

    for ( BaseCharacter **character = characters.Begin(); character != characters.End(); character++ )
    {
        (*character)->Draw();
    }
}

void Engine::SavePreviousPosition( void )
{
    for ( BaseCharacter **i = characters.Begin(); i != characters.End(); i++ )
    {
        BaseCharacter *character = *i;
        character->m_iPrevPosX = character->m_iPosX;
        character->m_iPrevPosY = character->m_iPosY;
    }
}

void Engine::Restart( void )
{
    system( "cls" );
    std::cout << "GAME OVER";
    std::cin.ignore();
    system( "cls" );

    pWorld->m_iCurLevel = 1; pWorld->m_iPrevLevel = 0;

    pPlayer->RestoreHealth( BaseCharacter::PLAYER );
    pPlayer->RestoreStrength();
    GET_ENEMY->RestoreStrength();
}
