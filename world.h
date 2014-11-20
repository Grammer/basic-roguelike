#ifndef WORLD_H
#define WORLD_H

#define PLAYER_ALIVE engine.pPlayer->GetHealth( BaseCharacter::PLAYER ) > 0
#define GET_ENEMY engine.characters.Get( 2 )

struct Tile
{
    Tile() : m_bCanWalk( true ) { }

    bool m_bCanWalk;
};

class World
{
public:
    enum WorldConstuctor
    {
        TILE = 46 // .
        ,WALL = 35 // #
        ,EXIT = 62 // >
        ,GRAVE = 43 // +
        ,CHEST = 36 // $
    };

    enum Items
    {
        POTION_OF_HEALTH = 31 // !
        ,POTION_OF_STRENGTH = 42 // *
    };

    World( int, int );
    ~World();
    void Load( void );
    void LoadNextLevel( void );
    bool IsWall( int, int ) const;
    bool CanWalk( int, int ) const;
    void SpawnEnemy( void );
    void GenerateExit( void );
    void SpawnChest( void );

    int m_iWidth, m_iHeigth, m_iExitPosX, m_iExitPosY, m_iCurLevel, m_iPrevLevel, m_nEnemies;
    bool m_bOnNextLevel;

protected:
    Tile *pTile;

    void SetWall( int, int );
};

#endif // WORLD_H
