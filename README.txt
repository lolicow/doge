1.folder structure:
since it's a bit messy I put this as the 1st thing to read. so the top level folder contains all libraries I used(or tried to use) and 
MyEngine is the folder to the actual solution.

in /MyEngine you can open the solution.

Every .c .cpp .h files ,as well as asset/gameobject/level/shader files are in /MyEngine/MyEngine

2.features:

2.1 Debug Drawing:
press x, draw body volume and vel vector

2.2 Object Architecture:
Component based.
I have a gameobject manager, a physics manager and a rendering manager, each has its own list of gameobject and deal with them in update
gameobject pointers have a reference counter to make sure they can be managed/deleted with no problem( try R to reload level)

2.3 Event:
Basic Event:
used in collision message sending and trigger(enhancement.h) sending text message(in game: eat a bone will trigger a text message)

2.4 Data Driven:
Serialization: gameobject using .txt files. level using .csv files(similar to .txt but easier to edit with excel)

2.5 Framerate Controller:
FRC.h

2.6 Input:
inpuntmanager.h
IsPressed is used on arrow keys left and right. IsTriggered is used on arrow key up, etc.
Is Released is not actually used but it works.

2.7 Graphics:
OpenGL
Translation and Scale is used everywhere in the game, Rotation is not used but is tested
Has animation(clearly)
Used Perspective Projection Matrix to make the sence with different layers
UI is made with another camara/shader

2.8 Physics:
Collision Detection:
Between Circles/Rectangles, however only rectangles are used in the game. If you try to change the 
body shape in player.txt  into circle 0.15, you will see collision detected however the dog will be flying
chaoticly because the physics reaction algorithm that I implement now only works well with rectangles

impulse based collision response (for rectangles)

3. The Game
Run it in Release Mode Please, the level has around 1k gameobject with collision, in debugmode it's very slow.
there's another level in the folder, much smaller, when I implement new functions I switch to that level file.

Just play through it.

Hint: it's not easy, it will probably take like 20 mins to finish(I've asked some people to test it). or press g to enable god mode, then it takes around 5 mins. press s to reset
to home and continue or r to restart in case of game logic bug.
Hint2: in player.txt under the line playerstate the numbers are initial jump,HP,attack, if you rly need it to be faster.
