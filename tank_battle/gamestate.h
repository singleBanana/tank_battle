/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 */ 
#ifndef _GAMESTATE_H
#define _GAMESTATE_H

class GameState
{
public:
	virtual ~GameState();

    // This is called periodically to draw the main menu, or the world, or the gameover screen
    // Please no logic in this code, only drawing
    // Render returns void. Can't stop the game from here, unlike HGE's renderfunc.
    virtual void Render() = 0;

    // This is called periodically to do the thinking (or nothing) and check keyboard etc.
    // Please no drawing in this code, only logic
	// true表示游戏结束
    virtual bool Think() = 0;
    
    // This is called when enters the state.
	virtual void OnEnterState();
};

#endif