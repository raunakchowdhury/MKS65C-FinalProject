# Coup
## by CRYM
Chowdhury, Raunak  
Ye, Michael  
(pronounced 'crime')

### Description
Our project will be an emulation of the card game **Coup**. In the game, players seek to eliminate other players using randomly chosen powers and bluffing their way to victory.

### Game
In a future where the government is run for profit, all but a privileged few live lives of poverty and desperation. The Resistance rises out of these oppressed masses in revolt and throws the government into chaos.

Many see hope for a brighter future for the first time in their lives. Others see opportunity for absolute power.

To take command, you must destroy the influence of your rivals and drive them into exile.

Only one can survive.

--> https://www.miniaturemarket.com/ibcgp-b0813.html

#### Contents:
- 15 Character Cards (three each of Duke, Assassin, Captain, Ambassador, Contessa)
- 6 Summary Cards
- 50 Coins
- Rules
Ages: 13+
Players: 2-6
Game Length: 15 minutes

![alt text](http://devetosgaming.com/wp-content/uploads/2017/07/Photo-Coup-01-1024x658.png)

### Libraries required
none

### Instructions to use project
1. Ensure you are in the root directory of the repo.
1. SSH into `*.stuy.edu`, where `*` is `homer`, `bart`, `clyde`, or `marge`. Run `make`.
1. Run `make rserver` in that same terminal.
1. In other terminals, run `make rclient args="*.stuy.edu"`.
1. Run `./server` or `./client` accordingly.
1. Follow the prompts on screen to play the game.

### Changelog
- 1/15/2019
  - Game operational after fixing bug by redesigning how clients handle server input (R+M, 5 hours)
  - Solved issues with clients on different ip addresses (R, 2 hours)
  - Allowed for exiting of players, final bug fixes (M, 3 hours)
- 1/14/2019
  - Solved server problem and tried to integrate sockets with game, but was unable to get reading and writing to clients working (R+M, 5 hours)
  - Replaced print and getinput to send to specific player (M, 2 hours)
- 1/13/2019
  - Ran into problems with old sockets; tried switching to pipes, but pipes didn't work either (R)
  - Fixed bugs, undoing actions when challenged correctly (M, 4 hours)
- 1/12/2019
  - Completed blocking, all player moves, game logic (M, 3 hours)
- 1/11/2019
  - Continued to work on server setup (R)
  - Added a second read/write that lets clients distinguish between responding to the server or not (R, 3 hours)
- 1/10/2019
  - Added template functions and included comments for quick substitution into game files (R, 2 hours)
  - Making the move "exchange", basic blocking as well (M, 4 hours)
- 1/09/2019
  - Set up the game using subservers (R, 5 hours)
  - Implimenting game logic, adding more moves (M, 4)
- 1/08/2019
  - Replaced pipe files with socket files (R, 0.1 hour)
  - Created basic moves, game can start and end (M, 2 hours)
- 1/06/2019
  - Added initial pipe files to repo (R, 0.1 hour)
