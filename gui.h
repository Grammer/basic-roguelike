#ifndef GUI_H
#define GUI_H

#define _DEBUG_STRING "HP: 100/100\tHP: 20/20   "
#define HEALTH_DROP "You have looted "

class Text
{
public:
    struct Message
    {
        char *text;
        Message( const char* );
        ~Message();
    };
    List<Message *> log;

    void ShowStats( void );
    void MessageInterception( const char*, ... );

    int m_iTextScreenHeight;

};

class HUD
{
public:
    enum HUDConstructor
    {
        TOP_LEFT_CORNER = 201
        ,BOT_LEFT_CORNER = 188
        ,BOT_RIGHT_CORNER = 200
        ,TOP_RIGHT_CORNER = 187
        ,DOUBLE_VERT_WALL = 186
        ,SINGLE_VERT_WALL = 179
        ,DOUBLE_HORIZ_WALL = 205
        ,SINGLE_HORIZ_WALL = 196
        ,TOP_DOUBLE_SEPARATOR = 209
        ,TOP_SINGLE_SEPARATOR = 194
        //
        ,MAX_HUD_HEIGTH = 5
    };

    void RenderHUD( void );

};

class Gui : public Text, public HUD
{
public:
    Gui();
    ~Gui();

    void ShowMessage( int, int );

};


#endif	// GUI_H
