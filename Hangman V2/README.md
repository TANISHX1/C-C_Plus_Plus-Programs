# Hangman V2

A terminal-based implementation of the classic Hangman word-guessing game in C with enhanced data structures and visual feedback.

## Overview
This is an improved version (V2) of Hangman that uses a **Binary Search Tree (BST)** for efficient word management and storage. Players attempt to guess a hidden word letter-by-letter within a maximum of 6 attempts.

## Contents

- **main.c** - Core game logic with interactive gameplay loop
- **header.h** - Header file with BST implementation, utility functions, and ANSI color styling
- **Debug.c** - Debug utility for testing data loading and random word selection
- **Debug** - Compiled debug executable
- **main** - Compiled game executable
- **sample.txt** - Word database containing word-hint pairs (format: `word:hint`)

## Key Features

### Game Mechanics
- **Interactive Gameplay** - Real-time letter guessing with visual feedback
- **Word Display** - Shows correctly guessed letters and blanks for unknown letters
- **Attempt Tracking** - Maximum 6 attempts with visual hangman ASCII art progression
- **Hint System** - Each word has a hint displayed at the start
- **Win/Lose Conditions** - Game ends when word is solved or attempts are exhausted

### Technical Features
- **Binary Search Tree (BST)** - Efficient storage and retrieval of words with O(log n) average lookup
- **Cross-Platform Compatibility** - Works on Windows and Unix-like systems (Linux/Mac)
- **ANSI Color Support** - Colored terminal output for better UX
- **Terminal Utilities** - Line clearing and cursor movement for clean display
- **Case-Insensitive Matching** - Accepts both uppercase and lowercase guesses

## Code Architecture

### Data Structure
- **hidden_word struct** - Node containing:
  - `node_id` - Unique identifier for each word
  - `word` - The actual word to guess
  - `hint` - Helpful hint for guessing
  - `left_node` / `right_node` - BST pointers

### Main Functions (header.h)
- `load_data()` - Reads words from sample.txt and builds BST
- `insert()` - Inserts new word into BST (recursive)
- `search()` - Finds word by node_id in BST
- `print()` - In-order traversal displaying all words
- `create_node()` - Creates new BST node
- `clean_n_lines_up()` - Terminal line clearing utility
- `time_freeze()` - Cross-platform sleep function
- `draw()` - Renders hangman ASCII art based on attempts

### Game Flow (main.c)
1. Load word database from sample.txt
2. Randomly select a word from the BST
3. Display hint to player
4. Loop until win/lose:
   - Display current word state and hangman
   - Accept letter guess
   - Validate against previous guesses
   - Update word display if correct
   - Increment attempts if incorrect
5. Display result message

## Compilation

### Compile Main Game
```bash
gcc -o main main.c header.h
```

### Compile Debug Utility
```bash
gcc -o Debug Debug.c header.h
```

## Execution

### Play the Game
```bash
./main
```

### Run Debug Mode (Test word loading and random selection)
```bash
./Debug
```

## Game Output Example
<video width="868" height="362" controls>
  <source src="data/output.mp4" type="video/mp4">
</video>


## Requirements
---
- GCC compiler (or compatible C compiler)
- POSIX-compliant terminal (for ANSI color codes and cursor control)
- sample.txt file in the same directory as the executable

## Technical Details

### Constants
- `MAX_WORD_LENGTH` - 10 characters max per word
- `MAX_WORD_HINT_LEN` - 80 characters max for hints
- `MAX_TRIES` - 6 attempts allowed
- `MAX_BUFFER_SIZE` - 256 bytes for file reading

### Performance
- **Time Complexity** - O(log n) average word lookup using BST
- **Space Complexity** - O(n) for storing all words
- **Initialization** - O(n log n) for building balanced BST

## Future Enhancements
- Implement AVL tree for balanced word distribution
- Add difficulty levels (easy, medium, hard)
- Include scoring system
- Persistent leaderboard
- More comprehensive word database

## Author Notes
---
This version improves upon the original Hangman by using proper data structures (BST) for word management, providing better code organization with header files, and enhancing user experience with colored output and terminal utilities.


__Tested and Developed by :- Tanish Shivhare (TANISHX1)__