# JCM_SS: Janus Cosmological Modele Simple Simulator

JCM_SS is Janus Cosmological Model N-Body simulation.

For more information regarding Janus model please see:

- http://www.jp-petit.org/science/Le_Modele_Cosmologique_Janus.pdf,
- author website http://www.jp-petit.org
- and author youtube video channel https://www.youtube.com/user/JPPETITofficiel

## Introduction of M- space objects

JCM_SS  is a  simple N-Body  simulation  (without collision  management and  me
without .

As any N-Body  simulation, we manage "__M+__" positive mass  space objects. But
in JCM_SS we add in the model a  new type of space objects know as "__M-__" who
have a "negative mass".

Interaction between space objects follow those rules:

- M+ interact with M+ following Newton gravity,
- __M- interact with M- following Newton gravity,__
- __M+ interact with M- following a "reverse" Newton gravity (they repel).__

Newton force is `F=G * m * m' / r / r` where :

- `G` is gravity constant,
- `M` is the mass of first objects
- `m'` is the mass of the other object
- `r` is the distance between particules

The  newton function  is in  _1/r2_ which  lead for  very close  objects to  an
_"infinite force"_.  We add a  "smooth parameter"  which consider that  two "to
close objects" where `r < tooClose` join for new object with a mass of `m + m'`
(except opposite mass which never join).

At the opposite, too "far away" object have almost no interaction force is "too
close to  nothing". We  add another  "smooth" parameter to  `r >  tooFar` which
indicate to the system to not consider this interaction (we save some compute).

## How-to

Compilation is something like:

`make`

To run the simulaton, first create a `config_file.json` and launch the program:

`>./jcm_ss <config_file.json>`

## Configuration options

A config file is a simple json file:

```json
{
  "Save Dir": "tmp\/",
  "Save File": "file.pov",
  "Universe Radius": 1000,
  "Nbr Tic": 1000,
  "Too Close": 1000,
  "Too Far": 1000,
  "Object Size": 1000,
  "Initial Space Objects": [
    {"x": -2, "y": 0, "vx": -1, "vy":0, "mass": 5},
    {"x": 2, "y": 0, "vx": 1, "vy":0, "mass": -5},
  ]
}
```

Where:

- `Save Dir`: is the directory where result json will be saved
- `Save File`: is the suffix of  output file. Each tic, the program will create
  json  file name  `<Save Dir>/<tic  #>-<Save File>`  that will  be able  to be
  parsed by a renderer
- `Universe Radius`: is the radius of the Universe
- `Nbr Tic`: nbr of time the simulator will do the simulation
- `Too Close`: smoother see previous section
- `Too Far`:  smoother see previous section
- `Initial Space Objects`: are the initial condition of your simulation

You can generate your configuration  file using `initCond.rb <num objects>` and
then `./jcm_ss <num objects>.json`

## Output

The  output of  the simulation  will create  several file  in `Save  Dir`. File
prefix  is the  `<tic number>-`  and  suffis is  user define  by `<Save  File>`
parameter.

```json
{
  "Tic": 0,
  "Universe Radius": 1000,
  "Space Object": [
    {"x": -2.000000, "y": 0.000000, "Size": 1000, "Sign": "M-"},
    {"x": 2.000000, "y": 0.000000, "Size": 1000, "Sign": "M+"},
  ]
}
```

Where `Tic` is the number of tic  starting a 0. `Universe Radius` is the radius
of the universe  and each `Space Object`  is an array of object,  where `x` and
`y` are the positon of the object in the referential, `size` is the size of the
object (this is relative to its mass), and  `sign` indicate if it's an M+ or M-
particule.

## Technical Consideration

Considering the _simplicity_ of the algorithm  and what should be done, I tried
to write a very simple code.

The code  is in `c` because  the algorithm is  ultra complex (which is  not the
opposite of _simple_) and this should go fast.

I used a table  for objects because this go fast, I don't  have to add elements
and by  the way I can  do easy copy of  the table using `memcpy`  which is very
important, it's the fatest c library function...

The  input and  the output  are in  Json, so  feel free  to create  any initial
condition and to  write and to she a  renderer (I wont write it, I  do not like
graphical stuff).

## TODO

### Short term

- Test and control of the operation
- Changing output file to the format as inoput (to enbale "to be continued...")

### Long term

- Integration of funnier algo like "Particle Mesh"

## Miscellaneous

```
    ╚⊙ ⊙╝
  ╚═(███)═╝
 ╚═(███)═╝
╚═(███)═╝
 ╚═(███)═╝
  ╚═(███)═╝
   ╚═(███)═╝
```
