# ProjectCo-Op

A neural net experiment, uses an evolutionary algorithm in order to complete a challenge. This challenge consists of 
the neural net being used by two "creatures" inhabitting a 10x10 grid, with a "dumb" predator creature that will simply try to kill the nearest 
creature also inhabiting the grid. The neural net powered creatures are smaller, and in order to survive they must team up to kill
the predator creature. They can do this by both being within "touching distance" (their borders meet, either at the sides or at the corners)
of the predator creature, at the same time. Included are the source files, plus a bunch of text files that store the last fittest neural net, past
fittest neural nets, and a movement file txt that stores the moves the last fittest neural net performed, and a move history that stores
the moves the fittest neural net in each generation performed. The moves performed by the latest generation are read in order to display what it
did, in an SFML graphical window, that refreshes every two seconds so as to provide time to analyze the moves.

Link that shows the result of running the program with the fittest neural net loaded:
https://www.youtube.com/watch?v=54NwIbQYdqg&feature=youtu.be

Requires SFML, Windows, and a 64 bits pc.


