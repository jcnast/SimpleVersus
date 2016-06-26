# OBJS specifies which files to compile as part of the project
OBJS = GameLoop.o GameMaster.o Object.o Circle.o Rectangle.o Bullet.o Character.o Ground.o Collision.o Texture.o ControllerMaster.o Controller.o ParticleSystem.o Particle.o AudioMaster.o
CC = g++
DEBUG = -g
FLAGS = -lSDL2 -lSDL2_image $(DEBUG)
OBJ_NAME = SimpleVersus

SimpleVersus: $(OBJS)
	$(CC) -o $(OBJ_NAME) $(FLAGS) -c

GameLoop.o: GameLoop.cc GameMaster.h ControllerMaster.h AudioMaster.h
	$(CC) GameLoop.cc $(FLAGS) -c

GameMaster.o: GameMaster.h GameMaster.cc Texture.h Object.h Ground.h Character.h Bullet.h Collision.h ParticleSystem.h AudioMaster.h
	$(CC) GameMaster.cc $(FLAGS) -c

Object.o: Object.h Object.cc Collision.h
	$(CC) Object.cc $(FLAGS) -c

Circle.o: Circle.h Circle.cc Object.h Rectangle.h Collision.h
	$(CC) Circle.cc $(FLAGS) -c

Rectangle.o: Rectangle.h Rectangle.cc Object.h Circle.h Collision.h
	$(CC) Rectangle.cc $(FLAGS) -c

Bullet.o: Bullet.h Bullet.cc Circle.h Character.h Ground.h Collision.h AudioMaster.h Texture.h GameMaster.h
	$(CC) Bullet.cc $(FLAGS) -c

Character.o: Character.h Character.cc Rectangle.h Bullet.h Ground.h Collision.h ControllerMaster.h Controller.h ParticleSystem.h Texture.h GameMaster.h AudioMaster.h
	$(CC) Character.cc $(FLAGS) -c

Ground.o: Ground.h Ground.cc Rectangle.h Bullet.h Character.h Collision.h ParticleSystem.h GameMaster.h AudioMaster.h Texture.h
	$(CC) Ground.cc $(FLAGS) -c

Collision.o: Collision.h Collision.cc Object.h
	$(CC) Collision.cc $(FLAGS) -c

Texture.o: Texture.h Texture.cc GameMaster.h
	$(CC) Texture.cc $(FLAGS) -c

ControllerMaster.o: ControllerMaster.h ControllerMaster.cc Controller.h Character.h
	$(CC) ControllerMaster.cc $(FLAGS) -c

Controller.o: Controller.h Controller.cc
	$(CC) Controller.cc $(FLAGS) -c

ParticleSystem.o: ParticleSystem.h ParticleSystem.cc Particle.h GameMaster.h
	$(CC) ParticleSystem.cc $(FLAGS) -c

Particle.o: Particle.h Particle.cc Object.h Circle.h Texture.h ParticleSystem.h GameMaster.h
	$(CC) Particle.cc $(FLAGS) -c

AudioMaster.o: AudioMaster.h AudioMaster.cc
	$(CC) AudioMaster.cc $(FLAGS) -c

clean:
	\rm *.o *~ Game

tar:
	tar cfv Game.tar GameLoop.cc GameMaster.h GameMaster.cc Object.h Object.cc Circle.h Circle.cc Rectangle.h Rectangle.cc\
										Bullet.h Bullet.cc Character.h Character.cc Ground.h Ground.cc Collision.h Collision.cc Texture.h Texture.cc\
										ControllerMaster.h ControllerMaster.cc Controller.h Controller.cc	ParticleSystem.h ParticleSystem.cc Particle.h\
										Particle.cc AudioMaster.h AudioMaster.cc