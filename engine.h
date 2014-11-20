#ifndef ENGINE_H
#define ENGINE_H

#define DEFAULT_PLAYER_SPAWN 1, 1
#define DEFAULT_ENEMY_SPAWN pWorld->m_iWidth - 4, pWorld->m_iHeigth - 4
#define ON_CHANGELEVEL ( pPlayer->m_iPosX == pWorld->m_iExitPosX && pPlayer->m_iPosY == pWorld->m_iExitPosY )

class Engine
{
public:
    List<BaseCharacter *> characters;
    BaseCharacter *pPlayer, *pChest;
    World *pWorld;
    Gui *gui;

    Engine();
    ~Engine();
    void Update( void );
    void Render( void );
    void SavePreviousPosition( void );
    void Restart( void );
};

extern Engine engine;

#endif // ENGINE_H
