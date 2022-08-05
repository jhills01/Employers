// NAME: Tom Hills
// NETID: jhills
// CS 494: Lab3 - jgraph
/* DESCRIPTION:
   This lab uses converted jgraph output to rewrite over a single JPG image to create gameplay for a space flight game where
   the user pilots a ship through a asteroid field. If the ship impacts an asteroid, ship dies and game ends. 
   The program opens a single JGR file and updates/flushes its contents based on user input in the game loop. Jgraph then takes
   that JGR file as input and outputs a PS file which ghostscript converts to the final JPG image (i.e., game frame). 
//*/
#include <cstdlib>
#include <iostream>
#include <sstream> 
#include <string>
#include <cstring>
#include <vector>
#include <cstdio>
#include <cmath>
#include <unistd.h>
#include <termios.h>
#include <time.h>

using namespace std;

//structs for all gameObjects
struct playerControlledObject
{  
    int xpos, ypos, xbound, ybound;
    int xVelocity, yVelocity;
};

struct asteroid
{
    int xpos, ypos, xbound, ybound;
    int xVelocity, yVelocity;
    int byteNum;
};

struct asteroids
{
    vector<asteroid> a;
    int numOfAsteroids;  
};

struct starBkg
{  
    int xpos, ypos, xpos2, ypos2;
    int xbound, ybound;
    int xVelocity, yVelocity;
};

//Renders final constructed JGR file
void renderScene()
{
    //NOTE: Actually, this might not be necessary
    // " &" must added to the end of each string command so that the system doesn't hang up 
    // system("jgraph -P jimage.jgr > jimage.ps &");
    // system("gs -q -sDEVICE=jpeg -dDEVICEXRESOLUTION=300 -dDEVICEYRESOLUTION=300 -dNOPAUSE -dBATCH -sOutputFile=jimage.jpg jimage.ps &");

    system("jgraph -P jimage.jgr > jimage.ps &");
    system("gs -q -sDEVICE=jpeg -dDEVICEXRESOLUTION=100 -dDEVICEYRESOLUTION=100 -dNOPAUSE -dBATCH -sOutputFile=jimage.jpg jimage.ps &");
}

//generates the scrolling star background images
void starBkgGenerator(FILE *file)
{
    int minimumStarSeparation = 10;

    fprintf(file, "newgraph                                                                                           \n"
                  "xaxis min 0 max 100 nodraw                                                                         \n"
                  "yaxis min 0 max 100 nodraw                                                                         \n"
                  "                                                                                                   \n");

    srand(0);
    for (int x = -300; x < 300; x += minimumStarSeparation)
    {
        for (int y = -300; y < 300; y += minimumStarSeparation)
        {
            if (rand()%10 == 1) 
            {
                fprintf(file, 
                            "newcurve pts %5d %5d color 1 1 1                                                   \n",
                            x, y);
            }
        }
    }
}

//used to build PS and JPG images files used in the game
//PS files are used as marks. JPG files are just for testing to make sure things look good.
void buildGameObjects()
{
    FILE *file = fopen("./gameObjects/jgr/star1.jgr", "w+");

    //DO NOT include -P flag with jgraph call to ensure postscript is encapsulated.
    //This will let us use "newcurve esp...." lines in jgr files
    system("jgraph ./gameObjects/jgr/ship.jgr > ./gameObjects/ps/ship.ps");
    system("gs -q -sDEVICE=jpeg -dDEVICEXRESOLUTION=100 -dDEVICEYRESOLUTION=100 -dNOPAUSE -dBATCH -sOutputFile=ship.jpg ./gameObjects/ps/ship.ps");

    system("jgraph ./gameObjects/jgr/space.jgr > ./gameObjects/ps/space.ps");
    system("gs -q -sDEVICE=jpeg -dDEVICEXRESOLUTION=100 -dDEVICEYRESOLUTION=100 -dNOPAUSE -dBATCH -sOutputFile=space.jpg ./gameObjects/ps/space.ps");

    starBkgGenerator(file);
    fclose(file);
    system("jgraph ./gameObjects/jgr/star1.jgr > ./gameObjects/ps/star1.ps");
    system("gs -q -sDEVICE=jpeg -dDEVICEXRESOLUTION=100 -dDEVICEYRESOLUTION=100 -dNOPAUSE -dBATCH -sOutputFile=star1.jpg ./gameObjects/ps/star1.ps");

    system("jgraph ./gameObjects/jgr/asteroid.jgr > ./gameObjects/ps/asteroid.ps");
    system("gs -q -sDEVICE=jpeg -dDEVICEXRESOLUTION=100 -dDEVICEYRESOLUTION=100 -dNOPAUSE -dBATCH -sOutputFile=asteroid.jpg ./gameObjects/ps/asteroid.ps");
}

//Updates positions/bounds for player character (PC); i.e., the space ship
void updatePC(FILE *file, playerControlledObject *pc)
{
    rewind(file);
    fseek(file, 745, SEEK_SET);
    fprintf(file,"%5d %5d pts %5d %5d                           \n",
                    pc->xbound, pc->ybound, pc->xpos, pc->ypos);
    fflush(file);
}

//Updates positions/bounds for asteroids
void updateAsteroids(FILE *file, asteroids *a)
{
    int startByte = 849;
    rewind(file);
    
    for (int i = 0; i < a->numOfAsteroids; ++i)
    {
        fseek(file, startByte + i*100, SEEK_SET);
        fprintf(file,"%5d %5d pts %5d %5d                       \n",
                     a->a[i].xbound, a->a[i].ybound, a->a[i].xpos, a->a[i].ypos);
    }

    fflush(file);
}

//Updates positions/bounds for scrolling star background
void updateBkg(FILE *file, starBkg *sb)
{
    rewind(file);
    
    fseek(file, 545, SEEK_SET);
    fprintf(file,"%5d %5d pts %5d %5d                           \n",
                    sb->xbound, sb->ybound, sb->xpos, sb->ypos);
    
    fseek(file, 645, SEEK_SET);
    fprintf(file,"%5d %5d pts %5d %5d                           \n",
                    sb->xbound, sb->ybound, sb->xpos2, sb->ypos2);
    fflush(file);
}

//changes STDIN characteristics to allow non-blocking keypress capture
int getkey() 
{
    int character;
    struct termios orig_term_attr;
    struct termios new_term_attr;

    /* set the terminal to raw mode */
    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    /* read a character from the stdin stream without blocking */
    /*   returns EOF (-1) if no character is available */
    character = fgetc(stdin);

    //while (getchar() != EOF);

    /* restore the original terminal attributes */
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}

//REVIEW: IMPORTANT!
//  Copying/Pasting from within windows includes CARRIAGE RETURNS with newlines! JGRAPH program doesn't read lines with this character
//  at the end!!!! If useing NOTEPAD++, ctrl+f and replace "\r" with blank to get rid of this.

int main()
{
    bool pause = true;
    int key, loopPause = 0;
    int thrust = 3;
    int startX = 48, startY = 63;
    int starX = 0, starY = 200;
    int starX2 = 0, starY2 = 800;
    int spaceX = 40, spaceY = 50, spaceXB = 150, spaceYB = 100;
    int astroSpeed = 8, astroStartY = 300, astroYSeparation = 125;
    playerControlledObject *pc = new playerControlledObject();
    starBkg *sb = new starBkg();
    asteroids *a = new asteroids();
    FILE *file;

    //STARTING STATS
    //ship
    pc->xpos = startX; pc->ypos = startY; pc->xbound = 30; pc->ybound = 30;
    pc->xVelocity = 0; pc->yVelocity = 0;
    
    //stars
    sb->xpos = starX; sb->ypos = starY, sb->xpos2 = starX2, sb->ypos2 = starY2; 
    sb->xbound = 100; sb->ybound = 100; 
    sb->yVelocity = 2;

    //asteroids
    srand(time(NULL));
    a->numOfAsteroids = 4;
    a->a.resize(a->numOfAsteroids);
    for (int i = 0; i < a->numOfAsteroids; ++i)
    {
        a->a[i].xbound = 100; a->a[i].ybound = 100; 
        a->a[i].xVelocity = 0; a->a[i].yVelocity = astroSpeed;

        a->a[i].ypos = astroStartY + (i * astroYSeparation);
        a->a[i].xpos = ( rand() % 201 ) - 50;
    }

    //build gameobjects
    //NOTE: Leave commented out unless new gameObject is added
    //buildGameObjects();

    //open jgr file and add marks
    //each line in jgr file is 100 bytes so fseek can be used to update only certain parts of the file   
    file = fopen("jimage.jgr", "w+");
    fprintf(file, "newgraph                                                                                           \n"
                  "xaxis min 0 max 100 nodraw                                                                         \n"
                  "yaxis min 0 max 100 nodraw                                                                         \n"
                  "                                                                                                   \n"
                  "newcurve eps gameObjects/ps/space.ps marksize %5d %5d pts %5d %5d                          \n"
                  "newcurve eps gameObjects/ps/star1.ps marksize %5d %5d pts %5d %5d                          \n"
                  "newcurve eps gameObjects/ps/star1.ps marksize %5d %5d pts %5d %5d                          \n"
                  "newcurve eps gameObjects/ps/ship.ps marksize %5d %5d pts %5d %5d                           \n",
                  spaceXB, spaceYB, spaceX, spaceY, 
                  sb->xbound, sb->ybound, sb->xpos, sb->ypos, 
                  sb->xbound, sb->ybound, sb->xpos2, sb->ypos2, 
                  pc->xbound, pc->ybound, pc->xpos, pc->ypos);

    for (int i = 0; i < a->numOfAsteroids; ++i)
    {
        fprintf(file, 
                  "newcurve eps gameObjects/ps/asteroid.ps marksize %5d %5d pts %5d %5d                       \n",
                  a->a[i].xbound, a->a[i].ybound, a->a[i].xpos, a->a[i].ypos);
    }

    fflush(file);

    //Print introduction and instructions
    printf("\nWelcome to Tom Hills classically inspired game: <--Don't Fly Into a F***ing Asteroid!!!-->\n"
           "\n"
           "    You are in control of a space ship which moves through space by using thrust. Thrusting\n"
           "    in any particular direction will increase your velocity in that direction. To reverse course,\n"
           "    you'll need to thrust in the opposite direction to stop, and then thrust again to move. Remember,\n"
           "    all units of thrust (1 unit == 1 keystroke) in any particular direction is cumulative. Your\n"
           "    sole objective is, you guessed it, don't fly into a f***ing asteroid or you blow up and die\n"
           "    a quick, but miserably lonely death. Enjoy!!\n"
           "\n"
           "        E: Thrust 1 unit UP\n"
           "        S: Thrust 1 unit LEFT\n"
           "        F: Thrust 1 unit RIGHT\n"
           "        D: Thrust 1 unit DOWN\n"
           "        X: Cancels all thrust and movement\n"
           "        V: Moves ship back to default position\n"
           "        Q: Pauses game\n"
           "        Z: Prints positions and velocities of all screen objects\n"
           "\n"
           "VERY IMPORTANT NOTE: You will need to provide your own way of viewing updates to \"jimage.jpg\"\n"
           "                     in order to play the game. \n");

    //game loop
    srand(time(NULL));
    renderScene();
    for (;;) 
    {
        //inital loop pause to allow player to read instructions
        if (loopPause == 0) 
        {
            printf("\nPress 'Q' when ready to play! Or ESCAPE to quit if you're scared...\n\n");
            while (pause) 
                {
                    key = getkey(); 
                    switch (key)
                    {
                        case 0x71:
                            pause = false;
                            break;
                        case 0x1B: //terminate game loop
                            printf("\nScared, huh?\n");
                            fclose(file);
                            delete pc;
                            delete sb;
                            delete a;
                            return 0;
                    }
                }
                loopPause = 1;
                pause = true;
        }

        //user input
        key = getkey();
        switch(key)
        {
            case 0x1B: //terminate game loop
                printf("\nScared, huh?\n");
                fclose(file);
                delete pc;
                delete sb;
                delete a;
                return 0;

            case 0x73: //MOVE LEFT
                pc->xVelocity -= thrust;
                break;

            case 0x66: //MOVE RIGHT
                pc->xVelocity += thrust;
                break;

            case 0x65: //MOVE UP
                pc->yVelocity += thrust;
                break;

            case 0x64: //MOVE DOWN
                pc->yVelocity -= thrust;
                break;

            case 0x76: //MOVE TO DEFAULT POSITION
                pc->xpos = startX; pc->ypos = startY;
                break;

            case 0x78: //STOP SHIP
                pc-> xVelocity = 0; pc->yVelocity = 0;
                break;
            case 0x7A: //PRINT PC STATS
                printf("\nSHIP -- x: %d, y: %d, xVelocity: %d, yVelocity: %d\n", 
                        pc->xpos, pc->ypos, pc->xVelocity, pc->yVelocity);
                printf("\nSPACE -- x: %d, y: %d, x2: %d, y2: %d\n", 
                        sb->xpos, sb->ypos, sb->xpos2, sb->ypos2);
                printf("\nASTEROID 0 -- x: %d, y: %d\n", 
                        a->a[0].xpos, a->a[0].ypos);
                for (int i = 1; i < a->numOfAsteroids; ++i) 
                {
                    printf("ASTEROID %d -- x: %d, y: %d\n", 
                                            i, a->a[i].xpos, a->a[i].ypos);
                }                    
                break;

            case 0x71: //PAUSE
                printf("\nPAUSE\n");
                while (pause) 
                {
                    key = getkey(); 
                    switch (key)
                    {
                        case 0x71:
                            pause = false;
                            break;
                        case 0x7A:
                            printf("\nSHIP -- x: %d, y: %d, xVelocity: %d, yVelocity: %d\n", 
                                    pc->xpos, pc->ypos, pc->xVelocity, pc->yVelocity);
                            printf("\nSPACE -- x: %d, y: %d, x2: %d, y2: %d\n", 
                                    sb->xpos, sb->ypos, sb->xpos2, sb->ypos2);
                            printf("\nASTEROID 0 -- x: %d, y: %d\n", 
                                    a->a[0].xpos, a->a[0].ypos);
                            for (int i = 1; i < a->numOfAsteroids; ++i) 
                            {
                                printf("ASTEROID %d -- x: %d, y: %d\n", 
                                                        i, a->a[i].xpos, a->a[i].ypos);
                            }                    
                            break;
                        case 0x1B: //terminate game loop
                            printf("\nScared, huh?\n");
                            fclose(file);
                            delete pc;
                            delete sb;
                            delete a;
                            return 0;
                    }
                }
                pause = true;
                break;
        }

        //star background
        sb->ypos -= sb->yVelocity;
        sb->ypos2 -= sb->yVelocity;
        if (sb->ypos < -550) {sb->xpos = starX; sb->ypos = 600;} 
        else if (sb->ypos2 < -550) {sb->xpos = starX; sb->ypos2 = 600;}

        //ship
        pc->xpos += pc->xVelocity;
        pc->ypos += pc->yVelocity;

        //asteroids
        for (int i = 0; i < a->numOfAsteroids; ++i) 
        {
            a->a[i].ypos -= a->a[i].yVelocity;
            
            if (a->a[i].ypos < -150) 
            {
                a->a[i].ypos = astroStartY + 50;
                a->a[i].xpos = ( rand() % 201 ) - 50;
            }

            //check for collisions
            //NOTE: These hit boxes are not accurate, but they get the point across    
            else if ( (abs(pc->xpos - a->a[i].xpos) < 45) && (abs(pc->ypos - a->a[i].ypos) < 30) ) 
            {
                printf("\n\n BOOM!!!!!!    U Be Ded Sucka!\n"); 
                exit(0); 
            }
        }

        //update all and render scene
        updateBkg(file, sb);
        updatePC(file, pc);
        updateAsteroids(file, a);
        system("jgraph -P jimage.jgr > jimage.ps");
        system("gs -q -sDEVICE=jpeg -dDEVICEXRESOLUTION=100 -dDEVICEYRESOLUTION=100 -dNOPAUSE -dBATCH -sOutputFile=jimage.jpg jimage.ps");
    }

    //Probably not needed. But just in case
    fclose(file);
    delete pc;
    delete sb;
    delete a;
    return 0;
}
