# Fresh Roti On The Go

The game is a Top-down motorcycle driving twin-stick shooter.

I think mamaks are a core part of Malaysian culture, as we are all united by our great food. And roti canai is one of the core staples of mamaks. For me, the quality of a mamak is decided on the quality of their roti canai.

## Usage guide

- The whole game can be built with a single `make` command, the raylib source files are included.

- After that just run the executable `./game`

## Libraries used

I have chosen Raylib as my library of choice as it has simple and straightforward usage syntax, perfect for a game jam newbie like me to get to coding gameplay faster rather than dealing with tons of boilerplate.

## Where did I apply what I learnt

- In Game.cpp I used RAII principles for managing raylib instance (`InitWindow(), CloseWindow()`), loading and unloading assets.

- I used two std::vectors for storing the enemies and projectiles, allowing for easy traversal when I need to work on their data and draw then on the screen.

## What was cut

- When the game was initially planned it was supposed to have multiple levels and roti weapon types.

- Next time I would probably plan my time better, have clear goals and milestones for the essentials, set more realistic targets, leaving additional polish for when there is more time.

## Credits

Malaysia map texture - [wikipedia](https://en.wikipedia.org/wiki/Malaysia_at_the_2025_Asian_Winter_Games#/media/File:Flag_of_Malaysia.svg)

background tilemap & enemy - [Kenny](https://kenney.nl/assets)

Player character - [itch.io](https://tmd-studios.itch.io/cars)

sound effects - [rFXGen](https://raylibtech.itch.io/rfxgen)

sound effects - [BeepBox](https://www.beepbox.co/)

font - [itch.io](https://not-jam.itch.io/not-jam-font-pack)
