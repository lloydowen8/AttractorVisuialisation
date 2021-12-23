# AttractorVisuialisation
A visualisation of mathematical attractors using C++ &amp; SFML 

This project was designed to be highly extendable with new attractors. The parent class (chaosAttractor) can be called with a series of lambda functions to represent any dynamic system. It has only been tested with sytems containing 3 dimentions (x,y,z). But can be extended to use more or less (untested) and is something I might look into doing in the future. 

Implemeting new attractors as sub-classes is more computationally efficent than the lambda functions. Sub-classes should implement the "calcStep" function.

<img src="./Screenshot.png">

## Dependencies
- SFML: Instructions on how to install SFML can be found <a src="https://www.sfml-dev.org/tutorials/2.5/start-vc.php">here</a>
- MinGW (Windows)

## Example Video
<a src="https://www.youtube.com/watch?v=TB8PrRNOKAM">https://www.youtube.com/watch?v=TB8PrRNOKAM</a>

## Controls 
- WASD: Move the attractor up, left, down, right 
- Up-Down arrows: Rotate the attractor on its X-axis
- Left-Right arrows: Rotate the attractor on its Z-axis
- Z: zoom in
- X: zoom out
- LShift: speed up
- LControl: speed down
- R: Increase red colour
- G: Increase green colour
- B: increase blue colour
- Backspace: reset colour

