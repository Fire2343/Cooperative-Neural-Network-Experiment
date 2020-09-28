# ProjectCo-Op

A neural net experiment, uses an evolutionary algorithm in order to complete a challenge. This challenge consists of 
this neural net belonging to two creatures inhabitting a 10x10 grid, with a predator creature that will try to kill the nearest 
neural net powered creature to it. The neural net powered creatures are smaller, in order to survive they must team up to kill
the predator creature. They can do this by both being within "touching distance" (their borders meet, either at the sides or at the corners)
of the predator creature. Included are the source files, plus a bunch of text files that store the last fittest neural net, past
fittest neural nets, and a movement file txt that stores the moves the last fittest neural net performed, and a move history that stores
the moves each generations fittest neural net performed. The latest move is read in order to display what the last fittest neural net
did, in an SFML graphical window, that refreshes every two seconds to make analyzing the moves easier.

Link that shows the result of running the program with the fittest neural net loaded:
https://www.youtube.com/watch?v=54NwIbQYdqg&feature=youtu.be

Requires SFML, Windows, and a 64 bits pc.


