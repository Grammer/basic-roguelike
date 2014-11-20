#include <Windows.h>

#include "list.h"
#include "gui.h"
#include "base_character.h"
#include "world.h"
#include "engine.h"

Engine engine;

int main( int argc, char* argv[] )
{
    HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );
    CONSOLE_CURSOR_INFO structCursorInfo;

    GetConsoleCursorInfo( handle, &structCursorInfo );
    structCursorInfo.bVisible = false;
    SetConsoleCursorInfo( handle, &structCursorInfo );

    engine.Update();

    return 0;
}
