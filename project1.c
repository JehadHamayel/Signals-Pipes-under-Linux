// Jehad Hamayel 1200348
// Abdalkarim Eiss 1200015
// Musab Masalmah 1200078

#include "local.h"

int main()
{
    // Remove all FIFO files in the /tmp directory
    DIR *dir;
    struct dirent *entry;
    char dir_path[] = "/tmp/";

    if ((dir = opendir(dir_path)) != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            // Check if filename matches the pattern "/tmp/%dfifo%d"
            int second_number;
            if (sscanf(entry->d_name, "%*dfifo%d", &second_number) == 1)
            {
                // Construct full filepath
                char filepath[512];
                snprintf(filepath, sizeof(filepath), "%s%s", dir_path, entry->d_name);

                // Attempt to remove the file
                if (remove(filepath) != 0)
                {
                    perror("Error deleting file");
                }
            }
        }
        closedir(dir);
    }

    // Declare variables to store the values
    int round_number, round_time, game_time;

    // Open the file for reading
    FILE *file = fopen("settings.txt", "r");

    // Check if the file was opened successfully
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 1; // Exit with error
    }

    // Read the values from the file
    fscanf(file, "round_number: %d\n", &round_number);
    fscanf(file, "round_time: %d\n", &round_time);
    fscanf(file, "game_time: %d", &game_time);

    // Close the file
    fclose(file);

    // define variables
    char buffer[BUFSIZ];
    char message[BUFFER_SIZE];
    ssize_t num_bytes;
    pid_t leader_a, leader_b;

    // pipes channels
    int fd[NumOfChildren][2];
    pid_t pids[NumOfChildren];

    // define the time of the game and the time of the round
    int endTimeOfOneRound;
    int endTimeOfTheGame;

    // Create the children processes and the FIFOs between them
    for (int i = 0; i < NumOfChildren; i++)
    {
        // Create the pipe
        if (pipe(fd[i]) < 0)
        {
            // Handle error
            printf("Could not create pipe %d\n", i);
            exit(-1 * i);
        }

        // Create the child process
        pids[i] = fork();

        // Set the end time of the game
        endTimeOfTheGame = (game_time * 60) + time(0);
        //
        srand(time(0));

        // Check if the process was created successfully
        if (pids[i] < 0)
        {
            // Handle error
            printf("Could not fork child %d\n", i);
            exit(-10 * i);
        }

        // Check if the process is the child
        if (pids[i] == 0)
        {
            // define pointer to store the FIFOs
            char **FIFOs;

            // define the range of the energy of the players from 100
            int min_range = 60;
            int max_range = 100;

            // define the energy of the player
            int Enargey = (int)(min_range + (rand() % (max_range - min_range)));

            // define the delay of the player before sending the ball
            float delay = ((float)((100 - Enargey) + 20) / 20) + 1;

            // code of the leader A on the leader of the left teem in the GUI
            if (i == 0)
            { // 1st child --> leadera

                // define the signals that the leader A will catch
                signal(SIGUSR1, sigset_catcherFromPrevPToNextP);
                signal(SIGUSR2, sigset_catcherForAganstMovment);
                signal(SIGALRM, sigset_catcherAFromParant);

                // define the variables that will be used in the leader A to control the balls
                int teamBalls = 0;
                int myBallsToP1 = 0;
                int myBallsFromP5 = 0;

                // create the FIFOs for the leader A with the other players and parent
                FIFOs = creatFIFOs(i, fd, 5);

                // pause the leader A to wait for the signal from the parent to start the game
                pause();
                while (1)
                {
                    // define the flags that will be used to control the balls
                    handle = false;
                    handle2 = false;
                    handle3 = false;

                    // define the energy of the player randomly between 60 and 100
                    srand(time(0));
                    Enargey = (int)(min_range + (rand() % (max_range - min_range)));

                    // define the delay of the player before sending the ball based on the energy
                    delay = ((float)((100 - Enargey) + 20) / 20) + 1;

                    // initialize the number of balls with the team and the balls of the LeaderA
                    teamBalls = 0;
                    myBallsToP1 = 0;
                    myBallsFromP5 = 0;

                    // define the end time of the round
                    endTimeOfOneRound = (round_time * 60) + time(0);

                    // receive the ball from the parent
                    bool done = reseveBall(FIFOs[0]);
                    while (done != true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                    {
                        done = reseveBall(FIFOs[0]);
                    }

                    // hold the signals
                    sighold(SIGUSR1);
                    sighold(SIGUSR2);
                    sleep(0.001);
                    sigrelse(SIGUSR1);
                    sigrelse(SIGUSR2);

                    // increment the number of balls of the LeaderA if the ball is received
                    if (done == true)
                    {
                        myBallsToP1++;
                        teamBalls++;
                    }

                    pid_t from, to;

                    while (1)
                    {
                        // check if the leader of the right team sent the ball to the leader of the left team and the time of the round is not finished
                        if (handle2 == true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                        {
                            bool done = reseveBall(FIFOs[1]);
                            sighold(SIGUSR1);
                            sighold(SIGUSR2);
                            sleep(0.001);
                            sigrelse(SIGUSR1);
                            sigrelse(SIGUSR2);

                            // wait until the ball is received
                            while (done != true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                            {
                                done = reseveBall(FIFOs[1]);
                                sighold(SIGUSR1);
                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                                sigrelse(SIGUSR2);
                            }
                            // increment the number of balls of the LeaderA if the ball is received
                            if (done == true)
                            {
                                myBallsToP1++;
                                teamBalls++;
                            }
                        }

                        // check if the time of the round is finished
                        if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
                        {
                            break;
                        }

                        sighold(SIGUSR1);
                        sighold(SIGUSR2);
                        sleep(0.001);
                        sigrelse(SIGUSR1);
                        sigrelse(SIGUSR2);

                        // check if the team has balls and the LeaderA has balls and the LeaderA is not sending the ball to the LeaderB and the time of the round is not finished
                        if (teamBalls > 0 && myBallsFromP5 > 0 && handle == false && handle2 == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                        {
                            sscanf(FIFOs[1], "/tmp/%dfifo%d", &from, &to);
                            // send the ball from the LeaderA to the LeaderB
                            bool done = sendBall(from, to, delay, FIFOs[1], SIGUSR1);

                            sighold(SIGUSR1);
                            sighold(SIGUSR2);
                            sleep(0.001);
                            sigrelse(SIGUSR1);
                            sigrelse(SIGUSR2);

                            // wait until the ball is sent
                            while (done != true && handle == false && handle2 == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                            {

                                done = sendBall(from, to, delay, FIFOs[1], SIGUSR1);

                                sighold(SIGUSR1);
                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                                sigrelse(SIGUSR2);
                            }

                            // check if the ball is sent
                            if (done == true)
                            {
                                sscanf(FIFOs[4], "/tmp/%dfifo%d", &from, &to);
                                kill(to, SIGSEGV);
                                printf("Project : LeaderA to LeaderB\n");
                                sighold(SIGUSR1);
                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                                sigrelse(SIGUSR2);
                                // decrement the number of balls of the LeaderA and the team
                                myBallsFromP5--;
                                teamBalls--;

                                // decrease the energy of the player when he sends the ball
                                if (Enargey >= 25)
                                {
                                    Enargey = Enargey - (int)(1 + (rand() % (5 - 1)));
                                }
                            }
                        }

                        if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
                        {
                            break;
                        }
                        sighold(SIGUSR1);
                        sighold(SIGUSR2);
                        sleep(0.001);
                        sigrelse(SIGUSR1);
                        sigrelse(SIGUSR2);

                        // chick if the team has no balls and the LeaderA has no balls and the time of the round is not finished to receive the ball from the parent
                        if (teamBalls == 0 && handle2 == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                        {
                            sscanf(FIFOs[0], "/tmp/%dfifo%d", &from, &to);
                            // send the signal to the parent to send the ball to LeaderA
                            if (kill(from, SIGALRM) != 0)
                            {
                                perror("Error sending signal");
                            }

                            // receive the ball from the parent
                            bool done = reseveBall(FIFOs[0]);

                            sighold(SIGUSR1);
                            sighold(SIGUSR2);
                            sleep(0.001);
                            sigrelse(SIGUSR1);
                            sigrelse(SIGUSR2);

                            // wait until the ball is received
                            while (done != true && handle2 == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                            {
                                done = reseveBall(FIFOs[0]);
                                sighold(SIGUSR1);
                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                                sigrelse(SIGUSR2);
                            }
                            if (done == true)
                            {
                                myBallsToP1++;
                                teamBalls++;
                            }
                        }
                        if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
                        {
                            break;
                        }
                        sighold(SIGUSR1);
                        sighold(SIGUSR2);
                        sleep(0.001);
                        sigrelse(SIGUSR1);
                        sigrelse(SIGUSR2);

                        // check if the LeaderA has balls and the LeaderA received the ball from the parent and from Player 5 and the time of the round is not finished
                        if (myBallsToP1 > 0 && handle == false && handle2 == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                        {

                            sscanf(FIFOs[2], "/tmp/%dfifo%d", &from, &to);

                            bool done = sendBall(from, to, delay, FIFOs[2], SIGUSR1);

                            sighold(SIGUSR1);
                            sighold(SIGUSR2);
                            sleep(0.001);
                            sigrelse(SIGUSR1);
                            sigrelse(SIGUSR2);

                            // wait until the ball is sent
                            while (done != true && handle == false && handle2 == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                            {

                                done = sendBall(from, to, delay, FIFOs[2], SIGUSR1);

                                sighold(SIGUSR1);
                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                                sigrelse(SIGUSR2);
                            }
                            // check if the ball is sent
                            if (done == true)
                            {
                                sscanf(FIFOs[4], "/tmp/%dfifo%d", &from, &to);
                                kill(to, SIGILL);
                                printf("Project : LeaderA to P1\n");
                                sighold(SIGUSR1);
                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                                sigrelse(SIGUSR2);

                                // decrement the number of balls of the LeaderA
                                myBallsToP1--;

                                // decrease the energy of the player when he sends the ball
                                if (Enargey >= 25)
                                {
                                    Enargey = Enargey - (int)(1 + (rand() % (5 - 1)));
                                }
                            }
                        }
                        if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
                        {
                            break;
                        }
                        sighold(SIGUSR1);
                        sighold(SIGUSR2);
                        sleep(0.001);
                        sigrelse(SIGUSR1);
                        sigrelse(SIGUSR2);

                        // check if the LeaderA received the ball from the Player 5 and the time of the round is not finished
                        if (handle == true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                        {
                            bool done = reseveBall(FIFOs[3]);
                            sighold(SIGUSR1);
                            sighold(SIGUSR2);
                            sleep(0.001);
                            sigrelse(SIGUSR1);
                            sigrelse(SIGUSR2);

                            // wait until the ball is received
                            while (done != true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                            {
                                done = reseveBall(FIFOs[3]);
                                sighold(SIGUSR1);
                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                                sigrelse(SIGUSR2);
                            }

                            // increment the number of balls of the LeaderA
                            if (done == true)
                            {
                                myBallsFromP5++;
                            }
                        }
                        if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
                        {
                            break;
                        }
                        sighold(SIGUSR1);
                        sighold(SIGUSR2);
                        sleep(0.001);
                        sigrelse(SIGUSR1);
                        sigrelse(SIGUSR2);

                        delay = ((float)((100 - Enargey) + 20) / 20) + 1;
                    }

                    sleep(0.5);

                    signal(SIGALRM, SIG_IGN);

                    // end of the round after end the time of the round or the time of the game
                    printf("LeaderA (%d): End of the game\n", i);
                    bool breakLoop = false;
                    int id_fifo;

                    // wait until the players of the LeaderA end their last move
                    printf("LeaderA: wait my players ends there last move\n");

                    // open the FIFO to send the number of balls of the team
                    while (breakLoop == false)
                    {

                        id_fifo = open(FIFOs[0], O_RDWR);

                        if (id_fifo == -1)
                        {
                            perror("open");
                            exit(EXIT_FAILURE);
                        }

                        // Write data into the FIFO and then read the data from the FIFO to chick if the parent received the data
                        char message1[BUFFER_SIZE];
                        sprintf(message1, "%d", teamBalls);
                        if (write(id_fifo, message1, strlen(message1)) == -1)
                        {
                            perror("write");
                            exit(EXIT_FAILURE);
                        }

                        sleep(0.5);

                        // read data into the FIFO
                        memset(buffer, 0x0, BUFSIZ); /* clear first */
                        num_bytes = read(id_fifo, buffer, BUFSIZ);
                        if (num_bytes == -1)
                        {
                            perror("read");
                            exit(EXIT_FAILURE);
                        }

                        // check if the parent received the data
                        if (strcmp(message1, buffer) != 0)
                        {
                            breakLoop = true;
                        }
                        else
                        {
                            breakLoop = false;
                        }
                    }
                    signal(SIGUSR1, sigset_catcherFromPrevPToNextP);
                    signal(SIGUSR2, sigset_catcherForAganstMovment);
                    signal(SIGALRM, sigset_catcherAFromParant);

                    // wait for the signal from the parent to start the next round
                    pause();
                }
            }
            else if (i == 1)
            { // child 2 --> leaderb

                // define the signals that the leader B will catch
                signal(SIGUSR2, sigset_catcherFromPrevPToNextP);
                signal(SIGUSR1, sigset_catcherForAganstMovment);
                signal(SIGHUP, sigset_catcherBFromParant);

                // define the variables that will be used in the leader B to control the balls
                int teamBalls;
                int myBallsToP1;
                int myBallsFromP5;

                // create the FIFOs for the leader B with the other players and parent
                FIFOs = creatFIFOs(i, fd, 5);

                // pause the leader B to wait for the signal from the parent to start the game
                pause();
                while (1)
                {

                    // define the flags that will be used to control the balls
                    handle = false;
                    handle2 = false;
                    handle3 = false;
                    srand(time(0));

                    // define the energy of the player randomly between 60 and 100
                    Enargey = (int)(min_range + (rand() % (max_range - min_range)));

                    // define the delay of the player before sending the ball based on the energy
                    delay = ((float)((100 - Enargey) + 20) / 20) + 1;

                    // initialize the number of balls with the team and the balls of the LeaderB
                    teamBalls = 0;
                    myBallsToP1 = 0;
                    myBallsFromP5 = 0;

                    // define the end time of the round
                    endTimeOfOneRound = (round_time * 60) + time(0);
                    bool done = reseveBall(FIFOs[0]);

                    // receive the ball from the parent
                    while (done != true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                    {
                        done = reseveBall(FIFOs[0]);
                    }
                    sighold(SIGUSR1);
                    sighold(SIGUSR2);
                    sleep(0.001);
                    sigrelse(SIGUSR1);
                    sigrelse(SIGUSR2);

                    myBallsToP1++;
                    teamBalls++;

                    pid_t from, to;
                    while (1)
                    {
                        // check if the leader of the right team sent the ball to the leader of the left team and the time of the round is not finished
                        if (handle2 == true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                        {
                            bool done = reseveBall(FIFOs[1]);
                            sighold(SIGUSR1);
                            sighold(SIGUSR2);
                            sleep(0.001);
                            sigrelse(SIGUSR1);
                            sigrelse(SIGUSR2);
                            while (done != true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                            {
                                done = reseveBall(FIFOs[1]);
                                sighold(SIGUSR1);
                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                                sigrelse(SIGUSR2);
                            }
                            if (done == true)
                            {
                                myBallsToP1++;
                                teamBalls++;
                            }
                        }
                        if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
                        {
                            break;
                        }
                        sighold(SIGUSR1);
                        sighold(SIGUSR2);
                        sleep(0.001);
                        sigrelse(SIGUSR1);
                        sigrelse(SIGUSR2);

                        // check if the team has balls and the LeaderB has balls and the LeaderB is not sending the ball to the LeaderA and the time of the round is not finished
                        if (teamBalls > 0 && myBallsFromP5 > 0 && handle == false && handle2 == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                        {
                            sscanf(FIFOs[1], "/tmp/%dfifo%d", &to, &from);

                            bool done = sendBall(from, to, delay, FIFOs[1], SIGUSR2);

                            sighold(SIGUSR1);
                            sighold(SIGUSR2);
                            sleep(0.001);
                            sigrelse(SIGUSR1);
                            sigrelse(SIGUSR2);
                            while (done != true && handle == false && handle2 == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                            {
                                done = sendBall(from, to, delay, FIFOs[1], SIGUSR2);

                                sighold(SIGUSR1);
                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                                sigrelse(SIGUSR2);
                            }

                            if (done == true)
                            {
                                sscanf(FIFOs[4], "/tmp/%dfifo%d", &from, &to);
                                kill(to, SIGCONT);
                                printf("Project : LeaderB to LeaderA\n");
                                sighold(SIGUSR1);
                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                                sigrelse(SIGUSR2);

                                myBallsFromP5--;
                                teamBalls--;
                                if (Enargey >= 25)
                                {
                                    Enargey = Enargey - (int)(1 + (rand() % (5 - 1)));
                                }
                            }
                        }
                        if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
                        {
                            break;
                        }
                        sighold(SIGUSR1);
                        sighold(SIGUSR2);
                        sleep(0.001);
                        sigrelse(SIGUSR1);
                        sigrelse(SIGUSR2);

                        // check if the team has no balls and the LeaderB has no balls and the time of the round is not finished to receive the ball from the parent
                        if (teamBalls == 0 && handle2 == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                        {
                            sscanf(FIFOs[0], "/tmp/%dfifo%d", &from, &to);
                            if (kill(from, SIGHUP) != 0)
                            {
                                perror("Error sending signal");
                            }

                            bool done = reseveBall(FIFOs[0]);
                            sighold(SIGUSR1);
                            sighold(SIGUSR2);
                            sleep(0.001);
                            sigrelse(SIGUSR1);
                            sigrelse(SIGUSR2);
                            while (done != true && handle2 == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                            {
                                done = reseveBall(FIFOs[0]);
                                sighold(SIGUSR1);
                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                                sigrelse(SIGUSR2);
                            }
                            if (done == true)
                            {
                                myBallsToP1++;
                                teamBalls++;
                            }
                        }
                        if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
                        {
                            break;
                        }
                        sighold(SIGUSR1);
                        sighold(SIGUSR2);
                        sleep(0.001);
                        sigrelse(SIGUSR1);
                        sigrelse(SIGUSR2);

                        // check if the team has balls and the LeaderB has balls and the LeaderB is not sending the ball to the LeaderA and not send to Player 5 and the time of the round is not finished
                        if (myBallsToP1 > 0 && handle == false && handle2 == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                        {

                            sscanf(FIFOs[2], "/tmp/%dfifo%d", &from, &to);

                            bool done = sendBall(from, to, delay, FIFOs[2], SIGUSR2);

                            sighold(SIGUSR1);
                            sighold(SIGUSR2);
                            sleep(0.001);
                            sigrelse(SIGUSR1);
                            sigrelse(SIGUSR2);
                            while (done != true && handle == false && handle2 == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                            {

                                done = sendBall(from, to, delay, FIFOs[2], SIGUSR2);

                                sighold(SIGUSR1);
                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                                sigrelse(SIGUSR2);
                            }

                            if (done == true)
                            {
                                sscanf(FIFOs[4], "/tmp/%dfifo%d", &from, &to);
                                kill(to, SIGUSR2);
                                printf("Project : LeaderB to P1\n");
                                sighold(SIGUSR1);
                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                                sigrelse(SIGUSR2);
                                myBallsToP1--;
                                if (Enargey >= 25)
                                {
                                    Enargey = Enargey - (int)(1 + (rand() % (5 - 1)));
                                }
                            }
                        }
                        if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
                        {
                            break;
                        }
                        sighold(SIGUSR1);
                        sighold(SIGUSR2);
                        sleep(0.001);
                        sigrelse(SIGUSR1);
                        sigrelse(SIGUSR2);
                        // check if the LeaderB received the ball from the Player 5 and the time of the round is not finished
                        if (handle == true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                        {
                            bool done = reseveBall(FIFOs[3]);
                            sighold(SIGUSR1);
                            sighold(SIGUSR2);
                            sleep(0.001);
                            sigrelse(SIGUSR1);
                            sigrelse(SIGUSR2);
                            while (done != true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                            {
                                done = reseveBall(FIFOs[3]);
                                sighold(SIGUSR1);
                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                                sigrelse(SIGUSR2);
                            }
                            if (done == true)
                            {
                                myBallsFromP5++;
                            }
                        }
                        if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
                        {
                            break;
                        }
                        sighold(SIGUSR1);
                        sighold(SIGUSR2);
                        sleep(0.001);
                        sigrelse(SIGUSR1);
                        sigrelse(SIGUSR2);

                        delay = ((float)((100 - Enargey) + 20) / 20) + 1;
                    }

                    sleep(0.5);
                    // end of the round after end the time of the round or the time of the game or the parent send the signal to end the game
                    printf("LeaderB (%d): End of the game\n", i);

                    signal(SIGHUP, SIG_IGN);

                    // wait until the players of the LeaderB end their last move
                    printf("LeaderB: wait my players ends there last move\n");
                    bool breakLoop = false;
                    int id_fifo;

                    // open the FIFO to send the number of balls of the team and then read the data from the FIFO to chick if the parent received the data
                    while (breakLoop == false)
                    {

                        id_fifo = open(FIFOs[0], O_RDWR);

                        if (id_fifo == -1)
                        {
                            perror("open");
                            exit(EXIT_FAILURE);
                        }

                        // Write data into the FIFO
                        char message1[BUFFER_SIZE];
                        sprintf(message1, "%d", teamBalls);
                        if (write(id_fifo, message1, strlen(message1)) == -1)
                        {
                            perror("write");
                            exit(EXIT_FAILURE);
                        }

                        sleep(0.5);

                        // read data into the FIFO
                        memset(buffer, 0x0, BUFSIZ); /* clear first */
                        num_bytes = read(id_fifo, buffer, BUFSIZ);
                        if (num_bytes == -1)
                        {
                            perror("read");
                            exit(EXIT_FAILURE);
                        }
                        if (strcmp(message1, buffer) != 0)
                        {
                            breakLoop = true;
                        }
                        else
                        {
                            breakLoop = false;
                        }
                    }
                    signal(SIGUSR2, sigset_catcherFromPrevPToNextP);
                    signal(SIGUSR1, sigset_catcherForAganstMovment);
                    signal(SIGHUP, sigset_catcherBFromParant);
                    pause();
                }
            }
            else if (i > 1 && i <= 6)
            { // Team A
                signal(SIGUSR1, sigset_catcherFromPrevPToNextP);
                int myBalls = 0;
                FIFOs = creatFIFOs(i, fd, 3);

                // wait for the signal from the LeaderA
                pause();
                while (1)
                {
                    handle = false;
                    srand(time(0));
                    // define the energy of the player randomly between 60 and 100
                    Enargey = (int)(min_range + (rand() % (max_range - min_range)));
                    // define the delay of the player before sending the ball based on the energy
                    delay = ((float)((100 - Enargey) + 20) / 20) + 1;
                    myBalls = 0;
                    endTimeOfOneRound = (round_time * 60) + time(0);

                    pid_t from, to;
                    while (1)
                    {
                        // check if the LeaderA sent the ball to the player and the time of the round is not finished
                        if (handle == true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                        {
                            bool done = reseveBall(FIFOs[0]);
                            sighold(SIGUSR1);
                            sleep(0.001);
                            sigrelse(SIGUSR1);

                            if (done == true)
                            {
                                myBalls++;
                            }
                        }
                        if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
                        {
                            break;
                        }
                        sighold(SIGUSR1);
                        sleep(0.001);
                        sigrelse(SIGUSR1);
                        // check if the player has balls and the Leader A not sending the ball to me and the time of the round is not finished
                        if (myBalls > 0 && handle == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                        {

                            sscanf(FIFOs[1], "/tmp/%dfifo%d", &from, &to);

                            bool done = sendBall(from, to, delay, FIFOs[1], SIGUSR1);

                            sighold(SIGUSR1);
                            sleep(0.001);
                            sigrelse(SIGUSR1);

                            // wait until the ball is sent
                            while (done != true && handle == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                            {

                                done = sendBall(from, to, delay, FIFOs[1], SIGUSR1);

                                sighold(SIGUSR1);
                                sleep(0.001);
                                sigrelse(SIGUSR1);
                            }

                            if (done == true)
                            {
                                // send the signal to OpenGl to show the movement of the ball
                                if (i == 2)
                                {
                                    sscanf(FIFOs[2], "/tmp/%dfifo%d", &from, &to);
                                    kill(to, SIGTRAP);
                                    printf("Project : P1 to P2\n");
                                    sighold(SIGUSR1);
                                    sleep(0.001);
                                    sigrelse(SIGUSR1);
                                }
                                else if (i == 3)
                                {
                                    sscanf(FIFOs[2], "/tmp/%dfifo%d", &from, &to);
                                    kill(to, SIGABRT);
                                    printf("Project : P2 to P3\n");
                                    sighold(SIGUSR1);
                                    sleep(0.001);
                                    sigrelse(SIGUSR1);
                                }
                                else if (i == 4)
                                {
                                    sscanf(FIFOs[2], "/tmp/%dfifo%d", &from, &to);
                                    kill(to, SIGBUS);
                                    printf("Project : P3 to P4\n");
                                    sighold(SIGUSR1);
                                    sleep(0.001);
                                    sigrelse(SIGUSR1);
                                }
                                else if (i == 5)
                                {
                                    sscanf(FIFOs[2], "/tmp/%dfifo%d", &from, &to);
                                    kill(to, SIGFPE);
                                    printf("Project : P4 to P5\n");
                                    sighold(SIGUSR1);
                                    sleep(0.001);
                                    sigrelse(SIGUSR1);
                                }
                                else if (i == 6)
                                {
                                    sscanf(FIFOs[2], "/tmp/%dfifo%d", &from, &to);
                                    kill(to, SIGUSR1);
                                    printf("Project : P5 to LeaderA\n");
                                    sighold(SIGUSR1);
                                    sleep(0.001);
                                    sigrelse(SIGUSR1);
                                }

                                myBalls--;
                                if (Enargey >= 25)
                                {
                                    Enargey = Enargey - (int)(1 + (rand() % (5 - 1)));
                                }
                            }
                        }
                        if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
                        {
                            break;
                        }
                        sighold(SIGUSR1);
                        sleep(0.001);
                        sigrelse(SIGUSR1);
                        delay = ((float)((100 - Enargey) + 20) / 20) + 1;
                    }
                    // wait for the signal from the LeaderB or previouse player
                    pause();
                }
            }
            else if (i > 6 && i <= 11)
            { // Team B
                signal(SIGUSR2, sigset_catcherFromPrevPToNextP);
                int myBalls = 0;
                FIFOs = creatFIFOs(i, fd, 3);

                endTimeOfOneRound = (round_time * 60) + time(0);
                // wait for the signal from the LeaderB
                pause();
                while (1)
                {
                    handle = false;

                    srand(time(0));
                    Enargey = (int)(min_range + (rand() % (max_range - min_range)));

                    delay = ((float)((100 - Enargey) + 20) / 20) + 1;
                    myBalls = 0;
                    endTimeOfOneRound = (round_time * 60) + time(0);
                    pid_t from, to;
                    while (1)
                    {
                        // check if the Player resive the ball from the LeaderB and the time of the round is not finished
                        if (handle == true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                        {
                            bool done = reseveBall(FIFOs[0]);
                            sighold(SIGUSR2);
                            sleep(0.001);
                            sigrelse(SIGUSR2);
                            if (done == true)
                            {
                                myBalls++;
                            }
                        }
                        if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
                        {
                            break;
                        }
                        sighold(SIGUSR2);
                        sleep(0.001);
                        sigrelse(SIGUSR2);
                        // check if the player has balls and he want to send the ball to the next player and the time of the round is not finished
                        if (myBalls > 0 && handle == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                        {

                            sscanf(FIFOs[1], "/tmp/%dfifo%d", &from, &to);

                            bool done = sendBall(from, to, delay, FIFOs[1], SIGUSR2);

                            sighold(SIGUSR2);
                            sleep(0.001);
                            sigrelse(SIGUSR2);
                            while (done != true && handle == false && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                            {

                                done = sendBall(from, to, delay, FIFOs[1], SIGUSR2);

                                sighold(SIGUSR2);
                                sleep(0.001);
                                sigrelse(SIGUSR2);
                            }

                            if (done == true)
                            {
                                // send the signal to OpenGl to show the movement of the ball
                                if (i == 7)
                                {
                                    sscanf(FIFOs[2], "/tmp/%dfifo%d", &from, &to);
                                    kill(to, SIGPIPE);
                                    printf("Project :R P1 to P2\n");
                                    sighold(SIGUSR2);
                                    sleep(0.001);
                                    sigrelse(SIGUSR2);
                                }
                                else if (i == 8)
                                {
                                    sscanf(FIFOs[2], "/tmp/%dfifo%d", &from, &to);
                                    kill(to, SIGALRM);
                                    printf("Project :R P2 to P3\n");
                                    sighold(SIGUSR2);
                                    sleep(0.001);
                                    sigrelse(SIGUSR2);
                                }
                                else if (i == 9)
                                {
                                    sscanf(FIFOs[2], "/tmp/%dfifo%d", &from, &to);
                                    kill(to, SIGTERM);
                                    printf("Project :R P3 to P4\n");
                                    sighold(SIGUSR2);
                                    sleep(0.001);
                                    sigrelse(SIGUSR2);
                                }
                                else if (i == 10)
                                {
                                    sscanf(FIFOs[2], "/tmp/%dfifo%d", &from, &to);
                                    kill(to, SIGSTKFLT);
                                    printf("Project :R P4 to P5\n");
                                    sighold(SIGUSR2);
                                    sleep(0.001);
                                    sigrelse(SIGUSR2);
                                }
                                else if (i == 11)
                                {
                                    sscanf(FIFOs[2], "/tmp/%dfifo%d", &from, &to);
                                    kill(to, SIGCHLD);
                                    printf("Project :R P5 to LeaderB\n");
                                    sighold(SIGUSR2);
                                    sleep(0.001);
                                    sigrelse(SIGUSR2);
                                }

                                myBalls--;
                                if (Enargey >= 25)
                                {
                                    Enargey = Enargey - (int)(1 + (rand() % (5 - 1)));
                                }
                            }
                        }
                        if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
                        {
                            break;
                        }
                        sighold(SIGUSR2);
                        sleep(0.001);
                        sigrelse(SIGUSR2);
                        delay = ((float)((100 - Enargey) + 20) / 20) + 1;
                    }
                    // wait for the signal from the LeaderB or previouse player
                    pause();
                }
            }
            // close the FIFOs and free the memory
            if (i == 0 || i == 1)
            {
                for (int q = 0; q < 5; q++)
                {
                    free(FIFOs[q]);
                }
                free(FIFOs);
            }
            else
            {
                for (int q = 0; q < 3; q++)
                {
                    free(FIFOs[q]);
                }
                free(FIFOs);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    // this code for know the pid of the opengl process to communicate with it to show the game how it is going on

    // Open the file for writing
    file = fopen("ProjectPid.txt", "w");

    // Check if the file was opened successfully
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 1; // Exit with error
    }
    char str[50];
    sprintf(str, "%d", getpid());
    char buffer1[250] = "";
    strcat(buffer1, str);

    for (int i = 0; i < NumOfChildren; i++)
    {
        sprintf(str, ",%d", pids[i]);
        strcat(buffer1, str);
    }

    fprintf(file, "ProjectPID: %s\n", buffer1);

    // Close the file
    fclose(file);
    /******************/

    sleep(2);
    pid_t OpenGlPID;
    // Open the file for reading
    file = fopen("openglPID.txt", "r");

    // Check if the file was opened successfully
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 1; // Exit with error
    }

    // Read the values from the file
    fscanf(file, "OpenglPID: %d\n", &OpenGlPID);

    // Close the file
    fclose(file);

    // Print the values to verify they were read correctly

    /////////////////////////////////////////////////

    // while loop to send the pids of the close players for each player to communicate with them by create name of the Fifo between them
    for (int i = 0; i < NumOfChildren; i++)
    {
        close(fd[i][0]); // Close read end
        char messagePIDs[BUFFER_SIZE];
        if (i == 0)
        {
            sprintf(messagePIDs, "%d,%d,%d,%d,%d", getpid(), pids[1], pids[2], pids[6], OpenGlPID);
        }
        else if (i == 1)
        {
            sprintf(messagePIDs, "%d,%d,%d,%d,%d", getpid(), pids[0], pids[7], pids[11], OpenGlPID);
        }
        else if (i == 2)
        {
            sprintf(messagePIDs, "%d,%d,%d", pids[0], pids[3], OpenGlPID);
        }
        else if (i == 3)
        {
            sprintf(messagePIDs, "%d,%d,%d", pids[2], pids[4], OpenGlPID);
        }
        else if (i == 4)
        {
            sprintf(messagePIDs, "%d,%d,%d", pids[3], pids[5], OpenGlPID);
        }
        else if (i == 5)
        {
            sprintf(messagePIDs, "%d,%d,%d", pids[4], pids[6], OpenGlPID);
        }
        else if (i == 6)
        {
            sprintf(messagePIDs, "%d,%d,%d", pids[5], pids[0], OpenGlPID);
        }
        else if (i == 7)
        {
            sprintf(messagePIDs, "%d,%d,%d", pids[1], pids[8], OpenGlPID);
        }
        else if (i == 8)
        {
            sprintf(messagePIDs, "%d,%d,%d", pids[7], pids[9], OpenGlPID);
        }
        else if (i == 9)
        {
            sprintf(messagePIDs, "%d,%d,%d", pids[8], pids[10], OpenGlPID);
        }
        else if (i == 10)
        {
            sprintf(messagePIDs, "%d,%d,%d", pids[9], pids[11], OpenGlPID);
        }
        else if (i == 11)
        {
            sprintf(messagePIDs, "%d,%d,%d", pids[10], pids[1], OpenGlPID);
        }

        write(fd[i][1], messagePIDs, strlen(messagePIDs)); // Write to pipe
        close(fd[i][1]);                                   // Close write end
    }

    // signal for the parent to send the ball to the Leaders
    signal(SIGALRM, sigset_catcherFromLeaderA);
    signal(SIGHUP, sigset_catcherFromLeaderB);

    // define the variables that will be used to store the name of the FIFOs between the parent and the Leaders and the OpenGl
    char PareantFIFOWithLeaderA[BUFFER_SIZE]; // Allocate memory for FIFO name

    char PareantFIFOWithLeaderB[BUFFER_SIZE]; // Allocate memory for FIFO name

    char ProjectFIFOToOpenGl[BUFFER_SIZE]; // Allocate memory for FIFO name

    // create the FIFOs between the parent and the Leaders and the OpenGl
    sprintf(PareantFIFOWithLeaderA, "/tmp/%dfifo%d", getpid(), pids[0]);
    sprintf(PareantFIFOWithLeaderB, "/tmp/%dfifo%d", getpid(), pids[1]);
    sprintf(ProjectFIFOToOpenGl, "/tmp/%dfifo%d", getpid(), OpenGlPID);

    int delay = 0;

    sleep(1);
    // define the variables that will be used to store the score of the teams
    int scoreTeamA = 0;
    int scoreTeamB = 0;
    while (1)
    {
        handle = false;
        handle2 = false;

        endTimeOfOneRound = (round_time * 60) + time(0);
        // send the ball to the Leaders
        bool done = sendBall(getpid(), pids[0], delay, PareantFIFOWithLeaderA, SIGALRM);
        while (done != true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
        {
            done = sendBall(getpid(), pids[0], delay, PareantFIFOWithLeaderA, SIGALRM);
        }
        kill(OpenGlPID, SIGHUP);
        printf("Project: Parent sent ball to leaderA\n");

        done = sendBall(getpid(), pids[1], delay, PareantFIFOWithLeaderB, SIGHUP);
        while (done != true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
        {
            done = sendBall(getpid(), pids[1], delay, PareantFIFOWithLeaderB, SIGHUP);
        }
        kill(OpenGlPID, SIGQUIT);
        printf("Project: Parent sent ball to leaderB\n");

        while (1)
        {
            // check if the parent received signal from the LeaderA and the LeaderB and the time of the round is not finished
            if (handle == true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
            {
                done = sendBall(getpid(), pids[0], delay, PareantFIFOWithLeaderA, SIGALRM);
                while (done != true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                {
                    done = sendBall(getpid(), pids[0], delay, PareantFIFOWithLeaderA, SIGALRM);
                }
                if (done == true)
                {
                    kill(OpenGlPID, SIGHUP);
                    printf("Project: Parent sent ball to leaderA\n");
                    handle = false;
                }
            }
            if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
            {
                break;
            }
            if (handle2 == true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
            {
                done = sendBall(getpid(), pids[1], delay, PareantFIFOWithLeaderB, SIGHUP);
                while (done != true && time(0) < endTimeOfOneRound && time(0) < endTimeOfTheGame)
                {
                    done = sendBall(getpid(), pids[1], delay, PareantFIFOWithLeaderB, SIGHUP);
                }
                if (done == true)
                {
                    kill(OpenGlPID, SIGQUIT);
                    printf("Project: Parent sent ball to leaderB\n");
                    handle2 = false;
                }
            }
            if (time(0) > endTimeOfOneRound || time(0) > endTimeOfTheGame)
            {
                break;
            }
        }

        int scoreA = 0;
        int scoreB = 0;
        // wait until the Leaders and players end the round
        sleep(15);

        printf("Parent: End of the game\n");

        int id_fifo;
        // reseve the score of the LeaderA team and the LeaderB team to know who is the winner

        while (1)
        {
            id_fifo = open(PareantFIFOWithLeaderA, O_RDWR);
            if (id_fifo == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }

            // Read data from the FIFO
            memset(buffer, 0x0, BUFSIZ); /* clear first */
            num_bytes = read(id_fifo, buffer, BUFSIZ);
            if (num_bytes == -1)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }
            else if (num_bytes == 0)
            {
                perror("Resever process read nothing\n");
            }
            else
            {

                // Write data into the FIFO
                char message1[BUFFER_SIZE];
                sprintf(message1, "%d", getpid());
                if (write(id_fifo, message1, strlen(message1)) == -1)
                {
                    perror("write");
                    exit(EXIT_FAILURE);
                }

                // Close the FIFO
                if (close(id_fifo) == -1)
                {
                    perror("close");
                    exit(EXIT_FAILURE);
                }
                scoreA = atoi(buffer);

                break;
            }
        }
        sleep(0.5);
        while (1)
        {

            id_fifo = open(PareantFIFOWithLeaderB, O_RDWR);
            if (id_fifo == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }

            // Read data from the FIFO
            memset(buffer, 0x0, BUFSIZ); /* clear first */
            num_bytes = read(id_fifo, buffer, BUFSIZ);
            if (num_bytes == -1)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }
            else if (num_bytes == 0)
            {
                perror("Resever process read nothing\n");
            }
            else
            {

                // Write data into the FIFO
                char message1[BUFFER_SIZE];
                sprintf(message1, "%d", getpid());
                if (write(id_fifo, message1, strlen(message1)) == -1)
                {
                    perror("write");
                    exit(EXIT_FAILURE);
                }

                // Close the FIFO
                if (close(id_fifo) == -1)
                {
                    perror("close");
                    exit(EXIT_FAILURE);
                }
                scoreB = atoi(buffer);

                break;
            }
        }
        // check who is the winner in the round
        if (scoreA < scoreB)
        {
            scoreTeamA++;
        }
        else if (scoreA > scoreB)
        {
            scoreTeamB++;
        }
        printf("Team A: %d, Score%d\n", scoreA, scoreTeamA);
        printf("Team B: %d, Score%d\n", scoreB, scoreTeamB);
        // check if the team has round_number wins to be the winner
        if (scoreTeamA == round_number)
        {
            printf("Team A is the winner by Rounds\n");
            break;
        }
        else if (scoreTeamB == round_number)
        {
            printf("Team B is the winner by Rounds\n");
            break;
        }
        // send the signal to the OpenGl to show the score of the teams and restart the game for new round
        if (kill(OpenGlPID, SIGIO) == 0)
        {
            printf("Project: send restart\n");
        }
        else
        {
            perror("Error sending signal");
        }
        if (endTimeOfTheGame <= time(0))
        {
            if (scoreTeamA > scoreTeamB)
            {
                printf("Team A is the winner\n");
            }
            else if (scoreTeamA < scoreTeamB)
            {
                printf("Team B is the winner\n");
            }
            else
            {
                printf("No winner\n");
            }
            break;
        }
    }

    printf("Project: End of the game DONE\n");

    return 0;
}

// signal handlers
void sigset_catcherFromLeaderA(int n)
{

    handle = true;
}
void sigset_catcherFromLeaderB(int n)
{
    handle2 = true;
}
void sigset_catcherAFromParant(int n)
{
    handle3 = true;
}
void sigset_catcherBFromParant(int n)
{
    handle3 = true;
}
void sigset_catcherFromPrevPToNextP(int n)
{
    handle = true;
}
void sigset_catcherForAganstMovment(int n)
{
    handle2 = true;
}
// function to create the FIFOs between the parent and the Leaders and the OpenGl
char **creatFIFOs(int index, int fd[NumOfChildren][2], int numOfFIFOs)
{

    char buffer[BUFFER_SIZE];
    close(fd[index][1]); // close write
    memset(buffer, 0x0, BUFFER_SIZE);

    read(fd[index][0], buffer, BUFFER_SIZE); // Read from pipe

    close(fd[index][0]); // Close read end

    int i = 0;
    char *token;

    token = strtok(buffer, ",");
    int array[numOfFIFOs];
    while (token != NULL && i < numOfFIFOs)
    {
        array[i] = atoi(token); // Convert token to integer and store in array
        i++;
        // Get the next token
        token = strtok(NULL, ",");
    }

    char **FIFOs = malloc(numOfFIFOs * sizeof(char *));

    for (int i = 0; i < numOfFIFOs; i++)
    {
        FIFOs[i] = malloc(50 * sizeof(char)); // Allocate memory for string
    }
    if (index == 0 || index == 1)
    {
        sprintf(FIFOs[0], "/tmp/%dfifo%d", array[0], getpid());
        if (index == 0)
        {
            sprintf(FIFOs[1], "/tmp/%dfifo%d", getpid(), array[1]);
        }
        else if (index == 1)
        {
            sprintf(FIFOs[1], "/tmp/%dfifo%d", array[1], getpid());
        }
        sprintf(FIFOs[2], "/tmp/%dfifo%d", getpid(), array[2]);

        sprintf(FIFOs[3], "/tmp/%dfifo%d", array[3], getpid());
        sprintf(FIFOs[4], "/tmp/%dfifo%d", getpid(), array[4]);
    }
    else
    {
        sprintf(FIFOs[0], "/tmp/%dfifo%d", array[0], getpid());
        sprintf(FIFOs[1], "/tmp/%dfifo%d", getpid(), array[1]);
        sprintf(FIFOs[2], "/tmp/%dfifo%d", getpid(), array[2]);
    }

    for (int i = 0; i < numOfFIFOs; i++)
    {
        if (access(FIFOs[i], F_OK) == -1)
        {
            // Create named pipes with read and write permissions for all users
            mkfifo(FIFOs[i], S_IFIFO | 0666);
        }
    }

    return FIFOs;
}
// function to send the ball between the players and the Leaders
bool sendBall(int from, int to, float delay, char *FIFO, int signal)
{
    int id_fifo;
    char buffer[BUFSIZ];
    ssize_t num_bytes;

    if (kill(to, signal) != 0)
    {
        perror("Error sending signal");
    }

    id_fifo = open(FIFO, O_RDWR);

    if (id_fifo == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write data into the FIFO
    char message1[BUFFER_SIZE];
    sprintf(message1, "%f,%d", delay, getpid());
    if (write(id_fifo, message1, strlen(message1)) == -1)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    sleep(0.01);

    // read data into the FIFO
    memset(buffer, 0x0, BUFSIZ); /* clear first */
    num_bytes = read(id_fifo, buffer, BUFSIZ);
    if (num_bytes == -1)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
    if (strcmp(message1, buffer) != 0)
    {
        // send to openGL

        sleep(delay);

        return true;
    }

    return false;
}
// function to receive the ball between the players and the Leaders
bool reseveBall(char *FIFO)
{
    int id_fifo;
    char buffer[BUFSIZ];
    ssize_t num_bytes;
    id_fifo = open(FIFO, O_RDWR);
    if (id_fifo == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read data from the FIFO
    memset(buffer, 0x0, BUFSIZ); /* clear first */
    num_bytes = read(id_fifo, buffer, BUFSIZ);
    if (num_bytes == -1)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
    else if (num_bytes == 0)
    {
        perror("Resever process read nothing\n");
    }
    else
    {
        char *token;
        // Get the first token
        token = strtok(buffer, ",");
        float delay = atof(token);

        // Write data into the FIFO
        char message1[BUFFER_SIZE];
        sprintf(message1, "%f,%d", delay, getpid());
        if (write(id_fifo, message1, strlen(message1)) == -1)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }

        // Close the FIFO
        if (close(id_fifo) == -1)
        {
            perror("close");
            exit(EXIT_FAILURE);
        }
        // send to openGL
        sleep(delay);
        handle = false;
        handle2 = false;
        return true;
    }

    handle = false;
    handle2 = false;
    return false;
}
