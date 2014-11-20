#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H

class BaseCharacter
{
public:
    enum Characters
    {
        PLAYER = 64
       ,ENEMY = 87
       ,DEFAULT_PLAYER_HEALTH = 100
       ,DEFAULT_ENEMY_HEALTH = 20
       ,DEFAULT_PLAYER_STRENGTH = 10
       ,DEFAULT_ENEMY_STRENGTH = 5
    };

    enum CharacterState
    {
        ENEMY_DEAD = 0x00000001
    };

    BaseCharacter( int, int, int, int, char );
    void Draw( void ) const;
    void Spawn( void );
    void XAxisMovement( void );
    void YAxisMovement( void );
    bool SelectAxisMovement( void );
    bool IsNextToPlayer( void );
    void UpdateHealth( int, int );
    void UpdateStrength( int, int );
    void RestoreHealth( int );
    void RestoreStrength( void );
    void Attack( void );
    int GetHealth( int );
    int GetStrength( void );
    void SpawnHealth( void );
    void SpawnStrength( void );
    void GetLoot( void );

    int m_iPosX, m_iPosY, m_iPrevPosX, m_iPrevPosY, m_iSign
        ,m_iHealth, m_iStrength, m_iPrevStrength, m_iNewHealth;
    char m_cEntity;
    bool m_bIsNextToPlayer, m_bIsAlive, m_bChestOpened, m_bPlayerOnChest;
    void (BaseCharacter::*pLoot[2])( void );
};

#endif // BASE_CHARACTER_H
