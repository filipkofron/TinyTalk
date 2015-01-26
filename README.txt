[----------------------------]
| Welcome to TinyTalk ReadMe |
[----------------------------]

[Author]
    Filip Kofron - Year 2014/2015
    school mail: kofrofil@fit.cvut.cz
    personal mail: filip.kofron.cz@gmail.com

[Current Sources]
    https://github.com/filipkofron/tinytalk

[Intro]
    This semestral work contains a simple VM capable of interpreting my
    simple language syntax called "TinyTalk". It is mainly based upon
    the smalltalk syntax, although it is rather modified to be more readable.

    The sample program consists of a very simple multi-threaded (without any synchronization yet)
    HTTP container running a basic SAT solver.


[Building]
Prep:
    a) To build the VM, you'll need a POSIX-compatible OS (since sockets and a few dependant
    parts are currently embedded inside).
    b) The actual tools needed are: CMake and a GNU C++ compatible compiler.
Actions:
    1) Download the source core into a folder $SOURCES
    2) Create a target folder $TARGET for the VM to be placed.
    3) cd $TARGET; cmake $SOURCES
    4) make -j$CPU_COUNT  # where $CPU_COUNT is your CPU count to speedup the compilation
    5) cp -r $SOURCES/ttlib $SOURCES/ttsrc ./


[Running]
    1) Go to the target folder $TARGET, where the compiled VM is
    2) Run ./TinyTalk
    3) A command-line "> " after a few tenths of log messages should appear.
    4) Each written command is gonna be executed only after issuing EOF. (ctrl+d  or  ^d)
    5) You can start messing with it now.


[Sample program]
Intro:
    A simple SAT solver using bruteforce to state, whether a formula is satisfiable running
    inside a simple Http container.
Starting:
    1) Make sure the port 8080 is free or simply edit the $TARGET/ttsrc/sample.tt - self explanatory
    2) In the command-line of a running VM issue the following command:
        Exec execFileName: "ttsrc/sample.tt"
        Press ctrl+d (^d) and the sample program will load and execute.
Using:
    3) Open your favourite browser capable of basic HTML/css at address: http://localhost:8080/
    4) A form will let you use the sample program.


[Code basics]
// Branching:
(1 equals: 1) not
    ifTrue: [ Out writeLine: "does not equal" ]
    ifFalse: [ Out writeLine: "equals" ];

// Loops:  (for example run  ttsrc/loops.tt)
| i |;
i = 100;
While
    isTrue: [ (i equals: 0) not ]
    do: [   Out writeLine: ("i: " append: (i toString));
            i = i minus: 1
    ];
// or
For from: 100 to: 1 do:
[ :i: |
    Out writeLine: ("i: " append: (i toString))
];

// Array creation
| array |; array = {1, 2, {"dog"}};
// or
array = Array newWithSize: 100;

// Object creation:
|obj|;
obj = Object alloc;
// or
obj = Object new; // sets parent to Object (has useful functions)
// or
obj = Object createWith:
{
    "name", nil,
    "getName", [ :getName |
        name
    ],
    "setName:", [ :setName: |
        name = setName
    ]
};
