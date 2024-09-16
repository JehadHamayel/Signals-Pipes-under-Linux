#include "local.h"

// Initial position of the ball
float ball_position[20][2] = {
	{-0.1, 0.7}, {0.1, 0.7}, {-0.5, 0.4}, {-0.6, 0.3}, {-0.6, 0.1}, {-0.5, 0.1}, {-0.4, 0.1}, {-0.4, 0.3}, {-0.5, 0.4}, {0.5, 0.4}, {0.6, 0.3}, {0.6, 0.1}, {0.5, 0.1}, {0.4, 0.1}, {0.4, 0.3}, {0.5, 0.4}};

float direction = 0;		  // Direction variable
int ctrl[20] = {0};			  // Control array
int draw_ball_flag[20] = {0}; // Flag to determine if a ball should be drawn
int balls_count[12] = {0};	  // Array to count balls
int signals[16] = {0};		  // Signals array
int total_balls[2];			  // Total balls array
int score[2];				  // Score array
int flag_to_remove[14];		  // Array to indicate flags to remove
int round_count = 1;		  // Round count variable

// function to draw a square player
void drawPlayer(float x, float y)
{
	glColor3f(0.0, 0.0, 0.0); // Set color to black
	glBegin(GL_QUADS);
	glVertex2f(x - 0.05, y - 0.05);
	glVertex2f(x + 0.05, y - 0.05);
	glVertex2f(x + 0.05, y + 0.05);
	glVertex2f(x - 0.05, y + 0.05);
	glEnd();
}

// function to draw a filed
void drawField()
{
	glColor3f(0.0, 1.0, 0.0); // Set color to green
	glBegin(GL_QUADS);
	glVertex2f(-0.9, -0.9);
	glVertex2f(0.9, -0.9);
	glVertex2f(0.9, 0.9);
	glVertex2f(-0.9, 0.9);
	glEnd();
}

// function to draw a ball
void drawBall(float x, float y)
{
	glColor3f(1.0, 0.0, 0.0); // Set color to red for the ball
	glPointSize(10.0);		  // Set point size larger for visibility
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

// function to draw a text
void drawText(float x, float y, char *text)
{
	// Set text color (white)
	glColor3f(1.0, 1.0, 1.0);

	// Set text position (behind the player)
	float text_x = x - 0.1; // Adjust this value based on your requirement
	float text_y = y - 0.1; // Adjust this value based on your requirement

	// Set text position
	glRasterPos2f(text_x, text_y);

	// Render text
	int len = strlen(text);
	for (int i = 0; i < len; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
}

// function to draw a float counter
void drawCounter(float x, float y, int count)
{
	// Convert integer count to string
	char text[20];
	sprintf(text, "%d", count);

	// Set text color (red)
	glColor3f(1.0, 0.0, 0.0); // Red color

	// Set text position
	float text_x = x - 0.1; // Adjust this value based on your requirement
	float text_y = y - 0.1; // Adjust this value based on your requirement
	glRasterPos2f(text_x, text_y);

	// Render text
	int len = strlen(text);
	for (int i = 0; i < len; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
}

// display function that we draw the game from it
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//********************* Draw the football field and the players *****************************
	drawField();
	drawPlayer(0, 0.7);

	// left Team
	drawPlayer(-0.5, 0.5);

	drawPlayer(-0.7, 0.3);
	drawPlayer(-0.7, 0.1);
	drawPlayer(-0.5, 0);
	drawPlayer(-0.3, 0.1);
	drawPlayer(-0.3, 0.3);

	// Right Team
	drawPlayer(0.5, 0.5);

	drawPlayer(0.7, 0.3);
	drawPlayer(0.7, 0.1);
	drawPlayer(0.5, 0);
	drawPlayer(0.3, 0.1);
	drawPlayer(0.3, 0.3);

	//******************* Print the names of the players ******************
	drawText(0.06, 0.9, "Parent");

	drawText(-0.5, 0.68, "Team Leader L");
	drawText(-0.62, 0.37, "P1");
	drawText(-0.62, 0.17, "P2");
	drawText(-0.42, 0.07, "P3");
	drawText(-0.22, 0.17, "P4");
	drawText(-0.22, 0.37, "P5");

	drawText(0.5, 0.68, "Team Leader R");
	drawText(0.78, 0.37, "P1");
	drawText(0.78, 0.17, "P2");
	drawText(0.58, 0.07, "P3");
	drawText(0.38, 0.17, "P4");
	drawText(0.38, 0.37, "P5");

	//******************* Print the number of ball that each player have ***************************
	drawCounter(-0.33, 0.57, balls_count[0]);
	drawCounter(-0.68, 0.37, balls_count[1]);
	drawCounter(-0.68, 0.17, balls_count[2]);
	drawCounter(-0.33, 0.07, balls_count[3]);
	drawCounter(-0.13, 0.17, balls_count[4]);
	drawCounter(-0.13, 0.37, balls_count[5]);

	drawCounter(0.67, 0.57, balls_count[6]);
	drawCounter(0.86, 0.37, balls_count[7]);
	drawCounter(0.86, 0.17, balls_count[8]);
	drawCounter(0.66, 0.07, balls_count[9]);
	drawCounter(0.32, 0.17, balls_count[10]);
	drawCounter(0.32, 0.37, balls_count[11]);

	drawText(-0.7, 0.8, "#Round: ");
	drawCounter(-0.57, 0.8, round_count);

	// check if the ball counts of the player is > 0 to draw the ball behind the player
	if (balls_count[0] > 0)
	{
		drawBall(-0.5, 0.4);
	}
	if (balls_count[1] > 0)
	{
		drawBall(-0.6, 0.3);
	}
	if (balls_count[2] > 0)
	{
		drawBall(-0.6, 0.1);
	}
	if (balls_count[3] > 0)
	{
		drawBall(-0.5, 0.1);
	}
	if (balls_count[4] > 0)
	{
		drawBall(-0.4, 0.1);
	}
	if (balls_count[5] > 0)
	{
		drawBall(-0.4, 0.3);
	}
	if (balls_count[6] > 0)
	{
		drawBall(0.5, 0.4);
	}
	if (balls_count[7] > 0)
	{
		drawBall(0.6, 0.3);
	}
	if (balls_count[8] > 0)
	{
		drawBall(0.6, 0.1);
	}
	if (balls_count[9] > 0)
	{
		drawBall(0.5, 0.1);
	}
	if (balls_count[10] > 0)
	{
		drawBall(0.4, 0.1);
	}
	if (balls_count[11] > 0)
	{
		drawBall(0.4, 0.3);
	}

	// draw the text of the score and the total balls
	drawText(-0.8, -0.1, "Total Ball:");
	drawText(-0.8, -0.3, "Score:");

	drawCounter(-0.6, -0.1, total_balls[0]);
	drawCounter(-0.6, -0.3, score[0]);

	drawText(0.3, -0.1, "Total Ball:");
	drawText(0.3, -0.3, "Score:");

	drawCounter(0.5, -0.1, total_balls[1]);
	drawCounter(0.5, -0.3, score[1]);

	// check if the ball is ready to be draw and then draw it
	if (draw_ball_flag[0] == 1)
	{
		drawBall(ball_position[0][0], ball_position[0][1]);
	}
	if (draw_ball_flag[1] == 1)
	{
		drawBall(ball_position[1][0], ball_position[1][1]);
	}
	if (draw_ball_flag[2])
	{
		drawBall(ball_position[2][0], ball_position[2][1]);
	}
	if (draw_ball_flag[3])
	{
		drawBall(ball_position[3][0], ball_position[3][1]);
	}
	if (draw_ball_flag[4])
	{
		drawBall(ball_position[4][0], ball_position[4][1]);
	}
	if (draw_ball_flag[5])
	{
		drawBall(ball_position[5][0], ball_position[5][1]);
	}
	if (draw_ball_flag[6])
	{
		drawBall(ball_position[6][0], ball_position[6][1]);
	}
	if (draw_ball_flag[7])
	{
		drawBall(ball_position[7][0], ball_position[7][1]);
	}
	if (draw_ball_flag[8])
	{
		drawBall(ball_position[8][0], ball_position[8][1]);
	}
	if (draw_ball_flag[9])
	{
		drawBall(ball_position[9][0], ball_position[9][1]);
	}
	if (draw_ball_flag[10])
	{
		drawBall(ball_position[10][0], ball_position[10][1]);
	}
	if (draw_ball_flag[11])
	{
		drawBall(ball_position[11][0], ball_position[11][1]);
	}
	if (draw_ball_flag[12])
	{
		drawBall(ball_position[12][0], ball_position[12][1]);
	}
	if (draw_ball_flag[13])
	{
		drawBall(ball_position[13][0], ball_position[13][1]);
	}
	if (draw_ball_flag[14])
	{
		drawBall(ball_position[14][0], ball_position[14][1]);
	}
	if (draw_ball_flag[15])
	{
		drawBall(ball_position[15][0], ball_position[15][1]);
	}
	glFlush();
}

// from line 398 - 792 : this functions to draw the movment of the ball from player to another one and
//			this make the ball move step by step by change the position and draw it by the loop

void ParentToTLLeft()
{
	draw_ball_flag[0] = 1;
	switch (ctrl[0])
	{
	case 0:
		if (fabs(ball_position[0][1] - 0.7) < 0.0001)
		{
			total_balls[0]++;
		}
		ball_position[0][1] -= 0.05;
		if (ball_position[0][1] <= 0.4)
		{
			ctrl[0] = 1;
		}
		break;
	case 1:
		ball_position[0][0] -= 0.05;
		if (ball_position[0][0] <= -0.5)
		{
			draw_ball_flag[0] = 0;
			signals[0] = 0;
			flag_to_remove[0] = 0;
			ctrl[0] = 0;
			balls_count[0]++;
			ball_position[0][0] = -0.1;
			ball_position[0][1] = 0.7;
		}
		break;
	}

	// Draw the ball

	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void ParentToTLRight()
{
	draw_ball_flag[1] = 1;
	switch (ctrl[1])
	{
	case 0:
		if (fabs(ball_position[1][1] - 0.7) < 0.0001)
		{
			total_balls[1]++;
		}
		ball_position[1][1] -= 0.05;
		if (ball_position[1][1] <= 0.4)
		{
			ctrl[1] = 1;
		}
		break;
	case 1:
		ball_position[1][0] += 0.05;
		if (ball_position[1][0] >= 0.5)
		{
			draw_ball_flag[1] = 0;
			signals[1] = 0;
			ctrl[1] = 0;
			balls_count[6]++;
			ball_position[1][0] = 0.1;
			ball_position[1][1] = 0.7;
		}
		break;
	}
	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void TLLeftToP1L()
{
	draw_ball_flag[2] = 1;
	switch (ctrl[2])
	{
	case 0:
		if (ball_position[2][0] == -0.5)
		{
			balls_count[0]--;
		}
		ball_position[2][0] -= 0.03;
		if (ball_position[2][0] <= -0.6)
		{
			ctrl[2] = 1;
		}
		break;
	case 1:
		ball_position[2][1] -= 0.03;
		if (ball_position[2][1] <= 0.3)
		{
			draw_ball_flag[2] = 0;
			signals[2] = 0;
			ctrl[2] = 0;
			balls_count[1]++;
			ball_position[2][0] = -0.5;
			ball_position[2][1] = 0.4;
		}
		break;
	}
	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void P1ToP2L()
{
	if (fabs(ball_position[3][1] - 0.3) < 0.0001)
	{
		balls_count[1]--;
	}
	draw_ball_flag[3] = 1;
	ball_position[3][1] -= 0.03;
	if (ball_position[3][1] <= 0.1)
	{
		draw_ball_flag[3] = 0;
		signals[3] = 0;
		balls_count[2]++;
		ball_position[3][0] = -0.6;
		ball_position[3][1] = 0.3;
	}

	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void P2ToP3L()
{
	draw_ball_flag[4] = 1;
	if (fabs(ball_position[4][0] + 0.6) < 0.0001)
	{
		balls_count[2]--;
	}
	ball_position[4][0] += 0.03;
	if (ball_position[4][0] >= -0.5)
	{
		draw_ball_flag[4] = 0;
		signals[4] = 0;
		balls_count[3]++;
		ball_position[4][0] = -0.6;
		ball_position[4][1] = 0.1;
	}

	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void P3ToP4L()
{
	draw_ball_flag[5] = 1;
	if (fabs(ball_position[5][0] + 0.5) < 0.0001)
	{
		balls_count[3]--;
	}
	ball_position[5][0] += 0.03;
	if (ball_position[5][0] >= -0.4)
	{
		draw_ball_flag[5] = 0;
		signals[5] = 0;
		balls_count[4]++;
		ball_position[5][0] = -0.5;
		ball_position[5][1] = 0.1;
	}

	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void P4ToP5L()
{
	draw_ball_flag[6] = 1;
	if (fabs(ball_position[6][1] - 0.1) < 0.0001)
	{
		balls_count[4]--;
	}
	ball_position[6][1] += 0.03;
	if (ball_position[6][1] >= 0.3)
	{
		draw_ball_flag[6] = 0;
		signals[6] = 0;
		balls_count[5]++;
		ball_position[6][0] = -0.4;
		ball_position[6][1] = 0.1;
	}

	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void P5ToTLLeftL()
{
	draw_ball_flag[7] = 1;
	switch (ctrl[3])
	{
	case 0:
		if (fabs(ball_position[7][1] - 0.3) < 0.0001)
		{
			balls_count[5]--;
		}
		ball_position[7][1] += 0.03;
		if (ball_position[7][1] >= 0.4)
		{
			ctrl[3] = 1;
		}
		break;
	case 1:
		ball_position[7][0] -= 0.03;
		if (ball_position[7][0] <= -0.5)
		{
			draw_ball_flag[7] = 0;
			signals[7] = 0;
			ctrl[3] = 0;
			balls_count[0]++;
			ball_position[7][0] = -0.4;
			ball_position[7][1] = 0.3;
		}
		break;
	}
	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void TLLeftToTLRightL()
{
	draw_ball_flag[8] = 1;
	switch (ctrl[4])
	{
	case 0:
		if (fabs(ball_position[8][0] + 0.5) < 0.0001)
		{
			total_balls[0]--;
			balls_count[0]--;
			total_balls[1]++;
		}
		ball_position[8][0] += 0.12;
		if (ball_position[8][0] >= -0.4)
		{
			ctrl[4] = 1;
		}
		break;
	case 1:
		ball_position[8][1] += 0.12;
		if (ball_position[8][1] >= 0.5)
		{
			ctrl[4] = 2;
		}
		break;
	case 2:
		ball_position[8][0] += 0.12;
		if (ball_position[8][0] >= 0.4)
		{
			ctrl[4] = 3;
		}
		break;
	case 3:
		ball_position[8][1] -= 0.12;
		if (ball_position[8][1] <= 0.4)
		{
			ctrl[4] = 4;
		}
		break;
	case 4:
		ball_position[8][0] += 0.12;
		if (ball_position[8][0] >= 0.54)
		{
			draw_ball_flag[8] = 0;
			signals[8] = 0;
			ctrl[4] = 0;
			balls_count[6]++;
			ball_position[8][0] = -0.5;
			ball_position[8][1] = 0.4;
		}
		break;
	}
	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void TLRightToP1R()
{
	draw_ball_flag[9] = 1;
	switch (ctrl[5])
	{
	case 0:
		if (fabs(ball_position[9][0] - 0.5) < 0.0001)
		{
			balls_count[6]--;
		}
		ball_position[9][0] += 0.03;
		if (ball_position[9][0] >= 0.6)
		{
			ctrl[5] = 1;
		}
		break;
	case 1:
		ball_position[9][1] -= 0.03;
		if (ball_position[9][1] <= 0.3)
		{
			draw_ball_flag[9] = 0;
			signals[9] = 0;
			ctrl[5] = 0;
			balls_count[7]++;
			ball_position[9][0] = 0.5;
			ball_position[9][1] = 0.4;
		}
		break;
	}
	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void P1ToP2R()
{
	draw_ball_flag[10] = 1;
	if (fabs(ball_position[10][1] - 0.3) < 0.0001)
	{
		balls_count[7]--;
	}
	ball_position[10][1] -= 0.03;
	if (ball_position[10][1] <= 0.1)
	{
		draw_ball_flag[10] = 0;
		signals[10] = 0;
		balls_count[8]++;
		ball_position[10][0] = 0.6;
		ball_position[10][1] = 0.3;
	}

	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void P2ToP3R()
{
	draw_ball_flag[11] = 1;
	if (fabs(ball_position[11][0] - 0.6) < 0.0001)
	{
		balls_count[8]--;
	}
	ball_position[11][0] -= 0.03;
	if (ball_position[11][0] <= 0.5)
	{
		draw_ball_flag[11] = 0;
		signals[11] = 0;
		balls_count[9]++;
		ball_position[11][0] = 0.6;
		ball_position[11][1] = 0.1;
	}

	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void P3ToP4R()
{
	draw_ball_flag[12] = 1;
	if (fabs(ball_position[12][0] - 0.5) < 0.0001)
	{
		balls_count[9]--;
	}
	ball_position[12][0] -= 0.03;
	if (ball_position[12][0] <= 0.4)
	{
		draw_ball_flag[12] = 0;
		signals[12] = 0;
		balls_count[10]++;
		ball_position[12][0] = 0.5;
		ball_position[12][1] = 0.1;
	}

	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void P4ToP5R()
{
	draw_ball_flag[13] = 1;
	if (fabs(ball_position[13][1] - 0.1) < 0.0001)
	{
		balls_count[10]--;
	}
	ball_position[13][1] += 0.03;
	if (ball_position[13][1] >= 0.3)
	{
		draw_ball_flag[13] = 0;
		signals[13] = 0;
		balls_count[11]++;
		ball_position[13][0] = 0.4;
		ball_position[13][1] = 0.1;
	}

	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void P5ToTLRight()
{
	draw_ball_flag[14] = 1;
	switch (ctrl[6])
	{
	case 0:
		if (fabs(ball_position[14][1] - 0.3) < 0.0001)
		{
			balls_count[11]--;
		}
		ball_position[14][1] += 0.03;
		if (ball_position[14][1] >= 0.4)
		{
			ctrl[6] = 1;
		}
		break;
	case 1:
		ball_position[14][0] += 0.03;
		if (ball_position[14][0] >= 0.5)
		{
			draw_ball_flag[14] = 0;
			signals[14] = 0;
			ctrl[6] = 0;
			balls_count[6]++;
			ball_position[14][0] = 0.4;
			ball_position[14][1] = 0.3;
		}
		break;
	}
	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

void TLRightToTLLeft()
{
	draw_ball_flag[15] = 1;
	switch (ctrl[7])
	{
	case 0:
		if (fabs(ball_position[15][0] - 0.5) < 0.0001)
		{
			balls_count[6]--;
			total_balls[1]--;

			total_balls[0]++;
		}
		ball_position[15][0] -= 0.12;
		if (ball_position[15][0] <= 0.4)
		{
			ctrl[7] = 1;
		}
		break;
	case 1:
		ball_position[15][1] += 0.12;
		if (ball_position[15][1] >= 0.5)
		{
			ctrl[7] = 2;
		}
		break;
	case 2:
		ball_position[15][0] -= 0.12;
		if (ball_position[15][0] <= -0.4)
		{
			ctrl[7] = 3;
		}
		break;
	case 3:
		ball_position[15][1] -= 0.12;
		if (ball_position[15][1] <= 0.4)
		{
			ctrl[7] = 4;
		}
		break;
	case 4:
		ball_position[15][0] -= 0.12;
		if (ball_position[15][0] <= -0.5)
		{
			draw_ball_flag[15] = 0;
			signals[15] = 0;
			ctrl[7] = 0;
			balls_count[0]++;
			ball_position[15][0] = 0.5;
			ball_position[15][1] = 0.4;
		}
		break;
	}
	// Draw the ball
	glutPostRedisplay(); // Request redrawing
	usleep(21);			 // Adjust the speed of ball movement (21 microseconds = 0.05 seconds)
}

// This function is to recive the signals from the main function to detect which draw momvment the must be aplly
void updateBallPosition()
{

	// Alternately call ParentToTLLeft and ParentToTLRight
	if (signals[0])
	{
		ParentToTLLeft();
	}
	if (signals[1])
	{
		ParentToTLRight();
	}
	if (signals[2])
	{
		TLLeftToP1L();
	}
	if (signals[3])
	{
		P1ToP2L();
	}
	if (signals[4])
	{
		P2ToP3L();
	}
	if (signals[5])
	{
		P3ToP4L();
	}
	if (signals[6])
	{
		P4ToP5L();
	}
	if (signals[7])
	{
		P5ToTLLeftL();
	}
	if (signals[8])
	{
		TLLeftToTLRightL();
	}
	if (signals[9])
	{
		TLRightToP1R();
	}
	if (signals[10])
	{
		P1ToP2R();
	}
	if (signals[11])
	{
		P2ToP3R();
	}
	if (signals[12])
	{
		P3ToP4R();
	}
	if (signals[13])
	{
		P4ToP5R();
	}
	if (signals[14])
	{
		P5ToTLRight();
	}
	if (signals[15])
	{
		TLRightToTLLeft();
	}

	// Request redrawing
	glutPostRedisplay();
}

// draw background and Set it color to white
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	gluOrtho2D(-1, 1, -1, 1);
}

typedef void (*sighandler_t)(int);
sighandler_t sigset(int sig, sighandler_t disp);
void sigset_catcherParentToLeaderA(int n);
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
void sigset_catcherRestart(int n);
bool restart = false;

// this function is to recive the restart signal from the backend code and if the signal arraive the values on the screen will be changed
void sigset_catcherRestart(int n)
{
	printf("Restart\n");
	if (total_balls[0] < total_balls[1])
	{
		score[0]++;
	}
	else if (total_balls[0] > total_balls[1])
	{
		score[1]++;
	}
	// Restart the game
	signal(SIGHUP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGILL, SIG_IGN);
	signal(SIGTRAP, SIG_IGN);
	signal(SIGABRT, SIG_IGN);
	signal(SIGBUS, SIG_IGN);
	signal(SIGFPE, SIG_IGN);
	signal(SIGUSR1, SIG_IGN);
	signal(SIGSEGV, SIG_IGN);
	signal(SIGUSR2, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGALRM, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGSTKFLT, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGCONT, SIG_IGN);
	signal(SIGIO, SIG_IGN);

	memset(draw_ball_flag, 0, sizeof(draw_ball_flag));
	// initial position of the ball

	ball_position[0][0] = -0.1;
	ball_position[0][1] = 0.7;

	ball_position[1][0] = 0.1;
	ball_position[1][1] = 0.7;

	ball_position[2][0] = -0.5;
	ball_position[2][1] = 0.4;

	ball_position[3][0] = -0.6;
	ball_position[3][1] = 0.3;

	ball_position[4][0] = -0.6;
	ball_position[4][1] = 0.1;

	ball_position[5][0] = -0.5;
	ball_position[5][1] = 0.1;

	ball_position[6][0] = -0.4;
	ball_position[6][1] = 0.1;

	ball_position[7][0] = -0.4;
	ball_position[7][1] = 0.3;

	ball_position[8][0] = -0.5;
	ball_position[8][1] = 0.4;

	ball_position[9][0] = 0.5;
	ball_position[9][1] = 0.4;

	ball_position[10][0] = 0.6;
	ball_position[10][1] = 0.3;

	ball_position[11][0] = 0.6;
	ball_position[11][1] = 0.1;

	ball_position[12][0] = 0.5;
	ball_position[12][1] = 0.1;

	ball_position[13][0] = 0.4;
	ball_position[13][1] = 0.1;

	ball_position[14][0] = 0.4;
	ball_position[14][1] = 0.3;

	ball_position[15][0] = 0.5;
	ball_position[15][1] = 0.4;

	direction = 0;
	memset(ctrl, 0, sizeof(ctrl));
	memset(balls_count, 0, sizeof(balls_count));
	memset(total_balls, 0, sizeof(total_balls));

	memset(flag_to_remove, 0, sizeof(flag_to_remove));
	round_count++;
	init();
	glutDisplayFunc(display);
	memset(signals, 0, sizeof(signals));
	glutIdleFunc(updateBallPosition);
	memset(signals, 0, sizeof(signals));
	signal(SIGHUP, sigset_catcherParentToLeaderA);
	signal(SIGQUIT, sigset_catcherParentToLeaderB);
	signal(SIGILL, sigset_catcherLeaderAToP1);
	signal(SIGTRAP, sigset_catcherLP1ToP2);
	signal(SIGABRT, sigset_catcherLP2ToP3);
	signal(SIGBUS, sigset_catcherLP3ToP4);
	signal(SIGFPE, sigset_catcherLP4ToP5);
	signal(SIGUSR1, sigset_catcherLP5ToLeaderA);
	signal(SIGSEGV, sigset_catcherLeaderAToLeaderB);
	signal(SIGUSR2, sigset_catcherLeaderBToP1);
	signal(SIGPIPE, sigset_catcherRP1ToP2);
	signal(SIGALRM, sigset_catcherRP2ToP3);
	signal(SIGTERM, sigset_catcherRP3ToP4);
	signal(SIGSTKFLT, sigset_catcherRP4ToP5);
	signal(SIGCHLD, sigset_catcherRP5ToLeaderB);
	signal(SIGCONT, sigset_catcherLeaderBToLeaderA);
	signal(SIGIO, sigset_catcherRestart);
	glutPostRedisplay();
}

// when the opengl recive signal from the backend code this function will handle the signal and detect which movment must be apply
void sigset_catcherParentToLeaderA(int n)
{
	signals[0] = 1;
	printf("OpenGL: Parent to Leader A\n");
}
void sigset_catcherParentToLeaderB(int n)
{
	signals[1] = 1;
	printf("OpenGL: Parent to Leader B\n");
}
void sigset_catcherLeaderAToP1(int n)
{
	signals[2] = 1;
	printf("OpenGL: Leader A to P1\n");
}
void sigset_catcherLP1ToP2(int n)
{
	signals[3] = 1;
	printf("OpenGL: P1 to P2\n");
}
void sigset_catcherLP2ToP3(int n)
{
	signals[4] = 1;
	printf("OpenGL: P2 to P3\n");
}
void sigset_catcherLP3ToP4(int n)
{
	signals[5] = 1;
	printf("OpenGL: P3 to P4\n");
}
void sigset_catcherLP4ToP5(int n)
{
	signals[6] = 1;
	printf("OpenGL: P4 to P5\n");
}
void sigset_catcherLP5ToLeaderA(int n)
{
	signals[7] = 1;
	printf("OpenGL: P5 to Leader A\n");
}
void sigset_catcherLeaderAToLeaderB(int n)
{
	signals[8] = 1;
	printf("OpenGL: Leader A to Leader B\n");
}
void sigset_catcherLeaderBToP1(int n)
{
	signals[9] = 1;
	printf("OpenGL: Leader B to P1\n");
}
void sigset_catcherRP1ToP2(int n)
{
	signals[10] = 1;
	printf("OpenGL:R P1 to P2\n");
}
void sigset_catcherRP2ToP3(int n)
{
	signals[11] = 1;
	printf("OpenGL:R P2 to P3\n");
}
void sigset_catcherRP3ToP4(int n)
{
	signals[12] = 1;
	printf("OpenGL:R P3 to P4\n");
}
void sigset_catcherRP4ToP5(int n)
{
	signals[13] = 1;
	printf("OpenGL:R P4 to P5\n");
}
void sigset_catcherRP5ToLeaderB(int n)
{
	signals[14] = 1;
	printf("OpenGL:R P5 to Leader B\n");
}
void sigset_catcherLeaderBToLeaderA(int n)
{
	signals[15] = 1;
	printf("OpenGL: Leader B to Leader A\n");
}

#include <fcntl.h>
int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	// Read the IDs from the file to recive the signals from the backend code
	char Buffer[100];
	char pidchar[100];
	char FIFOProjectWtihOPenGL[100];
	pid_t pid = getpid();
	int fd;
	sleep(1);
	//////////////////////////////////////////////////////////////////////

	char projectPID[500];
	char FIFO[13][100];

	// Open the file for reading
	FILE *file = fopen("ProjectPid.txt", "r");

	// Check if the file was opened successfully
	if (file == NULL)
	{
		printf("Error opening the file.\n");
		return 1; // Exit with error
	}

	// Read the values from the file
	if (fgets(projectPID, 500, file) != NULL)
	{
		// Remove trailing newline character
		if (projectPID[strlen(projectPID) - 1] == '\n')
		{
			projectPID[strlen(projectPID) - 1] = '\0';
		}
	}
	else
	{
		printf("Failed to read string from file\n");
	}

	// Close the file
	fclose(file);

	char *token;
	token = strtok(projectPID, " ");
	token = strtok(NULL, " ");
	strcpy(projectPID, token);

	token = strtok(projectPID, ",");
	int i = 0;
	while (token != NULL)
	{
		strcpy(FIFO[i], token);

		token = strtok(NULL, ",");
		i++;
	}
	for (int i = 0; i < 13; i++)
	{
		sprintf(FIFO[i], "./tmp/%dfifo%d", atoi(FIFO[i]), getpid());
		if (access(FIFO[i], F_OK) == -1)
		{
			// printf("OpenGl: FIFO: %s\n", FIFO[i]);
			//  Create named pipes with read and write permissions for all users
			mkfifo(FIFO[i], S_IFIFO | 0666);
		}
	}

	// Print the values to verify they were read correctly

	/******************/

	// Open the file for writing
	file = fopen("openglPID.txt", "w");

	// Check if the file was opened successfully
	if (file == NULL)
	{
		printf("Error opening the file.\n");
		return 1; // Exit with error
	}

	fprintf(file, "OpenglPID: %d\n", getpid());

	// Close the file
	fclose(file);

	//////////////////////////////////////////////////////////////////////

	char ProjectFIFOToOpenGl[BUFFER_SIZE]; // Allocate memory for FIFO name

	sleep(1);

	signal(SIGHUP, sigset_catcherParentToLeaderA);
	signal(SIGQUIT, sigset_catcherParentToLeaderB);
	signal(SIGILL, sigset_catcherLeaderAToP1);
	signal(SIGTRAP, sigset_catcherLP1ToP2);
	signal(SIGABRT, sigset_catcherLP2ToP3);
	signal(SIGBUS, sigset_catcherLP3ToP4);
	signal(SIGFPE, sigset_catcherLP4ToP5);
	signal(SIGUSR1, sigset_catcherLP5ToLeaderA);
	signal(SIGSEGV, sigset_catcherLeaderAToLeaderB);
	signal(SIGUSR2, sigset_catcherLeaderBToP1);
	signal(SIGPIPE, sigset_catcherRP1ToP2);
	signal(SIGALRM, sigset_catcherRP2ToP3);
	signal(SIGTERM, sigset_catcherRP3ToP4);
	signal(SIGSTKFLT, sigset_catcherRP4ToP5);
	signal(SIGCHLD, sigset_catcherRP5ToLeaderB);
	signal(SIGCONT, sigset_catcherLeaderBToLeaderA);
	signal(SIGIO, sigset_catcherRestart);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1500, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Football Field");

	init();

	memset(signals, 0, sizeof(signals));

	glutDisplayFunc(display);

	glutIdleFunc(updateBallPosition);

	glutMainLoop();

	return 0;
}
