# Lift_Off_Laetus

## Notes
1.) In the main menu and pause menu, you will find a How To Play screen that describes controls!

2.) Quick notes on controls that were forgotten for the How To Play Screen: 
- Click left/right ctrl to open the pause menu.
- Click Tab / Right trigger to toggle characters

3.) If keys/buttons don't seem to work, try clicking the mouse in the game first, then try again!

## Credits
1.) Sound effects obtained from https://www.zapsplat.com/ and http://www.orangefreesounds.com/

2.) VFX obtained from Infinity Blade assets in the Unreal Marketplace.

## Map Layout Config  
The layout of where to place GridSpace objects (tiles) is described in /Config/grid.txt. The first three lines 
describe the number of rows on the grid, the number of columns in the grid, and then the starting location to start 
placing GridSpaces (i.e. the location to place the top left tile), respectively. Below that is a text representation 
of a two-dimensional grid of numbers representing the different spaces on the map. Each value in this grid is one
of the following integers::  

- 0 = Hole in the map, no GridSpace placed  
- 1 = Regular space a player can walk on, place a regular GridSpace  
- 2 = Space in which a player can harvest a slime orb from a slime tree  
- 3 = Space in which a player can harvest an energized ore from a rock  
- 4 = Space in which a player can harvest a shrub fruit from a shrub  

## Map Harvest Source Config
The placement of harvest sources, and what surrounding GridSpaces can harvest from them, is described in
/Config/grid_env.txt. Each row describes a single harvest source with the following three fields:  
- **Type**: 5 for a slime tree, 6 for rock, or 7 for a shrub.  
- **Tiles**: the row and column of the tiles to place this harvest source on. These are listed as a series 
of integers *(row1,column1,row2,column2,...)*. Should be 1, 2 or 4 tiles. In the case of more than one 
tile, the rows and columns of each tile are averaged together to place the harvest source in between all 
those tiles.  
- **Neighbors**: The row and column or each GridSpace that can harvest from it. These are listed as a series 
of integers *(row1,column1,row2,column2,...)*.
