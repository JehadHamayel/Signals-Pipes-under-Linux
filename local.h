#ifndef PROJECT1_H
#define PROJECT1_H
// Include Files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <dirent.h>
#include <stdbool.h>
////OpenGl
#include <GL/glut.h>
#include <math.h>

/// Definitions and Functions Prototypes For project1.c (Backend)

#define BUFFER_SIZE 100
#define NumOfChildren 12
// 
int sighold(int sig);
int sigrelse(int sig);
// Function prototypes
char **creatFIFOs(int index, int fd[NumOfChildren][2], int numOfFIFOs); // Function prototype
void sigset_catcherFromLeaderA(int n);
void sigset_catcherFromLeaderB(int n);
void sigset_catcherFromPrevPToNextP(int n);
void sigset_catcherForAganstMovment(int n);
void sigset_catcherAFromParant(int n);
void sigset_catcherBFromParant(int n);
bool sendBall(int from, int to, float delay, char *FIFO, int signal);
bool reseveBall(char *FIFO);


bool handle = false;  // normal movment
bool handle2 = false; // leader movment
bool handle3 = false; // Parant movment

typedef void (*sighandler_t)(int);
sighandler_t sigset(int sig, sighandler_t disp);

/// OpenGl Definitions and Functions Prototypes (opengl.c)

#define NUM_PLAYERS_PER_TEAM 5
#define NUM_TEAMS 2

// Function prototypes for opengl
void sigset_catcherLeaderAToP1(int n);
void sigset_catcherLP1ToP2(int n);
void sigset_catcherLP2ToP3(int n);
void sigset_catcherLP3ToP4(int n);
void sigset_catcherLP4ToP5(int n);
void sigset_catcherLP5ToLeaderA(int n);
void sigset_catcherLeaderAToLeaderB(int n);
void sigset_catcherParentToLeaderB(int n);
void sigset_catcherLeaderBToP1(int n);
void sigset_catcherRP1ToP2(int n);
void sigset_catcherRP2ToP3(int n);
void sigset_catcherRP3ToP4(int n);
void sigset_catcherRP4ToP5(int n);
void sigset_catcherRP5ToLeaderB(int n);
void sigset_catcherLeaderBToLeaderA(int n);
void ParentToTLLeft();

#endif
