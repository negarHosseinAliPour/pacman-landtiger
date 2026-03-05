# Pac-Man for LandTiger Board

## Project Overview

This project implements a simplified version of the classic **Pac-Man** game on the **LandTiger LPC1768 development board** using **Keil µVision** and the **SW_Debug LandTiger emulator**.

The goal of the project is to demonstrate embedded programming concepts such as hardware interaction, real-time control, timers, interrupts, and graphical display management.

Pac-Man moves inside a labyrinth displayed on the LandTiger LCD screen. The player controls Pac-Man using the joystick and must collect all the pills before the countdown timer expires.

The game focuses on implementing core mechanics rather than graphical complexity.

---

## Game Features

The following features were implemented according to the project specifications:

* Labyrinth displayed on the LandTiger LCD
* **240 Standard Pills** distributed across the maze
* **6 Random Power Pills** generated at random positions and times
* **Score system**
* **Life system**
* **Countdown timer**
* **Teleport tunnels**
* **Pause and Resume functionality**
* **Victory and Game Over screens**

---

## Game Rules

### Pills

* Standard Pill → **+10 points**
* Power Pill → **+50 points**

Each pill disappears once Pac-Man passes over it.

### Lives

* The player starts with **1 life**.
* Every **1000 points** earned grants **one extra life**.

### Timer

The game starts with a **60-second countdown timer**.

* If all pills are eaten before time runs out → **Victory**
* If the timer reaches zero → **Game Over**

---

## Controls

### Joystick

The joystick controls Pac-Man's direction:

* **UP** → Move Up
* **DOWN** → Move Down
* **LEFT** → Move Left
* **RIGHT** → Move Right

Once a direction is chosen, Pac-Man continues moving in that direction until:

* The player changes direction
* Pac-Man hits a wall

### INT0 Button

The **INT0 external interrupt** is used to pause or resume the game.

* Press INT0 → Pause game
* Press INT0 again → Resume game

The game **starts in PAUSE mode**.

---

## Teleport Tunnels

Two teleport locations are placed in the center of the maze.

When Pac-Man reaches one side, he exits from the opposite side while maintaining the same movement direction.

---

## Game End Conditions

### Victory

If Pac-Man eats all pills in the labyrinth, the game stops and a **Victory screen** is displayed.

### Game Over

If the **countdown timer reaches zero** before all pills are collected, the game ends and a **Game Over screen** appears.

---

## Implementation Details

The project was implemented using **C programming** within **Keil µVision**.

Key implementation aspects include:

* LCD graphics rendering
* Joystick input handling
* Timer-based movement
* External interrupt handling
* Random generation of power pills
* Game state management (pause, victory, game over)

The game logic controls Pac-Man movement, pill consumption, score updates, and timing events.

---

## Development Environment

* **Board:** LandTiger LPC1768
* **IDE:** Keil µVision
* **Debug Target:** SW_Debug (LandTiger Emulator)
* **Language:** C

The emulator allows testing the game without requiring a physical LandTiger board.

---

## Repository Structure

```
PacMan-LandTiger
│
├── src/                # Source code
├── KeilProject/        # Keil µVision project files
├── docs/               # Assignment description
├── screenshots/        # Emulator screenshots
└── README.md
```

---

## How to Run the Project

1. Open **Keil µVision**
2. Load the project file:

```
pacman.uvprojx
```

3. Select the **SW_Debug target**
4. Build the project
5. Run the LandTiger emulator

The game will start in **PAUSE mode**. Press **INT0** to begin.

---

## Author

Embedded Systems Project
University Assignment
