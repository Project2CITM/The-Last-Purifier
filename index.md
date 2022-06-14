
Welcome to our project. This is a student project made by seven members of the UPC university on Barcelona, Terrassa. 
We have made a roguelite game with all the necessarily features like, procedural map, player upgrade system,etc.
In order to get to know more features make sure to check out the trailer!
We've made everything using open source software, and we haven't used any engine. Instead, we made the game using Visual Studio 2019 and the SDL2 library.

This is our first roguelite type of game so we expect you to enjoy it and give us some feedback.

Thank you in advance for any help you can provide.

***
## The Last Purifier Logo:

![gameLogo](https://raw.githubusercontent.com/Project2CITM/The-Last-Purifier/main/Game/Assets/Sprites/UI/logoGame.png)


***
TRAILER

***

## Black Pearl Games Logo:

![TeamLogo](https://raw.githubusercontent.com/Project2CITM/The-Last-Purifier/main/Game/Assets/Sprites/UI/Team_Logo.png)

***
## Our Team:

**Biel Rabasa**

* Github: [bielrabasa](https://github.com/bielrabasa)

**Zhida Chen**

* Github: [Xidashuaige](https://github.com/Xidashuaige)

**Robert Recordà**

* Github: [MrIllas](https://github.com/MrIllas) 

**Pol Rius**

* Github: [Loproxi](https://github.com/Loproxi)  

**Adrià Sellarés**

* Github: [AdriaSeSa](https://github.com/AdriaSeSa)  

**Roger Salas**

* Github: [Draquian](https://github.com/Draquian) 

**Gerard Josep Ramón**

* Github: [kramtron](https://github.com/kramtron) 

***

## Controls:

Move:
- 	Walk			→	Left Joystick
- 	Walk Up			→	W  /  Axis -Y
- 	Walk Down		→	S  /  Axis +Y
- 	Walk Left		→	A  /  Axis -X
- 	Walk Right		→	D  /  Axis +X
- 	Dash			→	Spacebar  /  B - O

Spells:
- 	Cast Spell		→	Right Click  /  A - X
- 	Change Left Spell 	→	Q  /  LB - L1
- 	Change Right Spell	→	E  /  RB - R1

Others:
- 	Pause			→	Escape  /  Start
- 	Attack			→	Left Click  /  X - Square
- 	Interact		→	F  /  Y - Triangle
-   Zoom Minimap → TAB / SHARE - SELECT 

Debug Keys:
- 	Show Hitbox		→	F1
- 	God Mode		→	F10
- 	Clear Room Enemies	→	F3
- 	Clear Current Dungeon (for next game)	→	F6
- 	Save Current Dungeon	→	F7

***
## The Last Purifier - Concept Discovery
<details>
<summary>▶ Features</summary>
  <br>
<pre>
● Full game Design

● Wiki
  ● Main Page
  ● GDD (Game Design Document)
  ● TDD (Tech Design Document)
  ● Production Plan
  ● GUI Design Document
  ● Art Bible
  ● Audio Bible
  ● QA Workflow
</pre>
</details>
<br>

## The Last Purifier - Vertical Slice
<details>
<summary>▶ Features </summary>
  <br>
<pre>
● Fullscreen & Windowed mode

● God Mode (F10) 
  ● Instakills  
  ● No damage 

● Keyboard & Gamepad control

● 2 playable characters
  ● Sage (range battle system)  
  ● Revenant (melee battle system)

● 3 types of enemies 
  ● Ghoul 
  ● Kaboom  
  ● Worm

● 6 NPCs with custom dialog

● 5 Spells
  ● Purifed Sword
  ● Soul Shield
  ● Purification Slash
  ● Foteiros
  ● Ekriski

● Spell deck

● 2 Skill Trees, one per class.

● Random generated map.

● Hub Map with different zones 

● Custom team logo (with fx)

● Custom game logo (with fx)

● Menu & Pause menu (keyboard & controller)
  ● Play
  ● Options
  ● Credits
  ● Load
  ● Save
  ● Exit
  ● GitHub link

● Audio
  ● Background music
  ● Audio effects
    ● Enemies
    ● Players
    ● Attacks
    ● Menu

● HUD
  ● Health bar & damage effects
  ● Spell cards + deck
  ● Pause menu
</pre>
</details>
<br>

## The Last Purifier - Alpha
<details>
<summary>▶ Features</summary>
  <br>
<pre>
● Map
  ● Minimap (dungeon & hub)
  ● 3 new puzzles (3 new rooms)
  ● Save & Continue from last dungeon
  ● Player converter

● Entities
  ● New slime enemy
  ● New breakable pot
  ● Buttons (random pressing order)
  ● Pathfinding

● Weapons
  ● New combat system
  ● Spear
  ● Staff
  ● Book

● Audio
  ● New fx
  ● New music for every scene
  ● Change Volume

● UI
  ● Win screen
  ● Game over screen
  ● Vsync activation
  ● Volume changing
  ● Ability trees
    ● Buy better stats
    ● Buy deck spaces
    ● Buy ability improvements

● Quests
</pre>
</details>
<br>

## The Last Purifier - Gold
<details>
<summary>▶ Features</summary>
  <br>
<pre>
  ● Webpage
    ● Intro
    ● Trailer
    ● Team members
    ● Feature list

  ● Gamepad
    ● Menu movement
    ● All mechanics

  ● Particle System
    ● Blood (player hit)
    ● Collecting souls
    ● Fire (marks the exit when winning the run)

  ● Objects
    ● Hp orb
    ● Pneuma Spell

  ● Entities
    ● Miniboss
    ● Boss
      ● AI
      ● Pathfinding
      ● 3 Attacks
      ● Transition

  ● UI
    ● Animated Main Menu
    ● Torch animation
    ● Animated Pause Menu
    ● Animated Option Menu
    ● Animated Card Inventory
    ● Animated + Updated HUD
    ● Weapon selector
    ● Health bar HUD


  ● Ability Tree
    ● Completed Tree
    ● All elements show description when hovering
    ● Switching trees with TAB

  ● Audio
    ● 15-20 new sfx
    ● Boss battle theme music

  ● General features
    ● Sage teleport effect
    ● Revenant dash effect
    ● New player sprite implementation
    ● Sage class balancing
    ● Spell stats update
    ● Healing purified sword

  ● Bugs
    ● Puzzle design errors
    ● Puzzle 2 barrel colliders not destroying
    ● Enemy knockback while deactivated
    ● Entity collider not destroying after death
    ● Enemy parents destroying before children
    ● Trigger class calling errors
    ● Projectile scaling errors, offsets & rotation
    ● Boss fight music corruption
    ● Max spell slots
    ● Saving system errors
    ● Trees crashing the game when not having any soul
    ● Hub roof sprite error when going indoors and outdoors
    ● Movement Blocked when exiting with instructor
    ● Fullscreen hover box rendering in wrong position
    ● Blocked spells spawning
</pre>
</details>
<br>

***

### A few images IN-GAME:

* Main Menu

![MainMenu](https://raw.githubusercontent.com/Project2CITM/The-Last-Purifier/gh-pages/IN-GAME-MAINMENU(7).png)

* Hub

![hub](https://raw.githubusercontent.com/Project2CITM/The-Last-Purifier/gh-pages/IN-GAME(1).png)

* Global Stats Tree and Ability Tree for each class

![GlobalStatsTree](https://raw.githubusercontent.com/Project2CITM/The-Last-Purifier/gh-pages/IN-GAME-GLOBALTREE(4).png)

* Revenant Ability Tree

![RevenantAbilityTree](https://raw.githubusercontent.com/Project2CITM/The-Last-Purifier/gh-pages/IN-GAME-REVENANTTREE(3).png)

* Sage Ability Tree

![SageAbilityTree](https://raw.githubusercontent.com/Project2CITM/The-Last-Purifier/gh-pages/IN-GAME-SAGETREE(2).png)

* In Dungeon

![InDungeon](https://raw.githubusercontent.com/Project2CITM/The-Last-Purifier/gh-pages/IN-GAME(5).png)

* Boss Transition

![BossTransition](https://raw.githubusercontent.com/Project2CITM/The-Last-Purifier/gh-pages/BOSSTRANSITIONGIF.gif)

* Boss Fight

![BossFight](https://raw.githubusercontent.com/Project2CITM/The-Last-Purifier/gh-pages/BOSSFIGHTGIF.gif)
