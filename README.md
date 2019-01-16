# Coup
## Team Members — Michael Ye and Raunak Chowdhury

### Description
Our project will be an emulation of the card game Coup. In the game, players seek to eliminate other players using randomly chosen powers and bluffing their way to victory.

### Required Libraries
None

### Instructions to use project
1. Ensure you are in the root directory of the repo.
1. SSH into `*.stuy.edu`, where `*` is `homer`, `bart`, `clyde`, or `marge`. Run `make`.
1. Run `make rserver` in that same terminal.
1. In other terminals, run `make rclient args="*.stuy.edu"`.
1. Follow the prompts on screen to play the game.

### Instructions to use just the game, because server failed
1. Run 'gcc decoup.c'
1. Run './a.out'
1. will be included in makefile on later date

### Changelog
- 1/15/2019
  - Game operational after fixing bug by redesigning how clients handle server input (R+M, 5 hours)
- 1/14/2019
  - Solved server problem and tried to integrate sockets with game, but was unable to get reading and writing to clients working (R+M, 5 hours)
  - Integrated game with server, replaced print and getinput to send to specific player (M, 7 hours)
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
  - Created game skeleton, basic moves (M, 2 hours)
- 1/06/2019
  - Added initial pipe files to repo (R, 0.1 hour)
