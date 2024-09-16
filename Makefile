CC = gcc

SRCS1 = opengl.c
SRCS2 = project1.c
OBJS1 = $(SRCS1:.c=.o)
OBJS2 = $(SRCS2:.c=.o)
LIBS = -lglut -lGLU -lGL -lm

                                                             
all: opengl project1

opengl: $(OBJS1)

	$(CC) -o $@ $^ $(LIBS)
project1: $(OBJS2)

	$(CC) -o $@ $^
run: opengl project1

	./opengl &
	./project1
runOpenGl: opengl

	./opengl &
		
runProject1: project1

	./project1
	

%.o: %.c
	$(CC) -c $<

clean:
	rm -f $(OBJS1) opengl
	rm -f $(OBJS2) project1
