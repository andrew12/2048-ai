AI for the [2048 game](http://gabrielecirulli.github.io/2048/). This uses *expectimax optimization*, along with a highly-efficient bitboard representation to search upwards of 10 million moves per second on recent hardware. Heuristics used include bonuses for empty squares and bonuses for placing large values near edges and corners.  

## Building

Run `make`. Any relatively recent C++ compiler should be able to build the output.

## Running the command-line version

Run `bin/2048` if you want to see the AI by itself in action.
