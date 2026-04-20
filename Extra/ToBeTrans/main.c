// My plan and extra stuff: https://docs.google.com/document/d/1N_UV75xefmi3kY6E9m5p1nilEVe8WA9SZ50QoChwKjk/edit?tab=t.0

#include <stdio.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#define usleep(time) Sleep((time)/1000)
char getch() {
    return _getch();
}
#else

#include <unistd.h>
#include <termios.h>

char getch(void) {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

// Source for the function above - https://stackoverflow.com/a/912796
// Posted by anon, modified by community. See post 'Timeline' for change history
// Retrieved 2026-03-27, License - CC BY-SA 3.0

#endif 

#define SPEED 2750

void print(char text[], int time){
	if (time<0){
		time=5000;
	}
	printf("%s\n",text);
	fflush(stdout);
	usleep(time*1000);
}

int main(void){
	int a=4;
	int b=4;
	int c=4;

	char ac[]="---CHOICE a---\n1. Continue staring at yourself\n2. Get dressed ";
	char bc[]="---CHOICE b---\n1. Scroll on any social media of your choice(Pinterest/Twitter/Youtube/ETC)\n2. Stare out the window";
	char cc[]="---CHOICE c---\n1. Go stare at the mirror\n2. Research on possible reasons you feel this way";

	print("\033[1;31m[!WARNING¡] THIS GAME HAS 1 OUTCOME WITH A REFFRENCE TO SUICIDE, PROCEED AT YOUR OWN RISK [¡GNINRAW!]\033[0m",-1);
	print("\033[2J\033[H",500);
	print("You wake up in your room, tired, and walk into the bathroom.",SPEED);
	print("The only thing on your mind is the long day ahead… so tiring.",SPEED);
	print("Staring at the mirror, your heart aches with a sudden longing.",SPEED);
	print("Longing to be who you truly are.",SPEED);
	print("You look at yourself and \033[1;31mcritique\033[0m your bodily features.",SPEED);
	print("Why were you born this way?",SPEED+750);
	puts("");
	print(ac,0);
	printf("what is your choice: ");
	fflush(stdout);
	while (a!=1 && a!=2){
		a=(getch()-'0');
	}
	printf("%d\n\n",a);
	if (a==1){
		print("You stare at yourself for about an hour, \033[1;31mcritiquing\033[0m every little detail.",SPEED);
		print("Then you realised you are going to miss the bus if you don’t hurry up.",SPEED);
		print("Panting, you reach the bus stop; the bus arrives, and you go in.",SPEED-250);
		print("Phew, that was close.",SPEED);
		print("At school, you nearly fall asleep, but luckily, your friend wakes you up.",SPEED);
		print("You wish you were more like them than the \033[1;31mlazy\033[0m child you are.",SPEED);
		print("The teacher calls your name, and it feels off, but you answer and move on.",SPEED);
		print("The school day feels like a century, but soon it comes to an end, and you are on the return bus home.\n\n",SPEED+750);
	} else {
		print("Sighing, you get ready and leave for the bus stop.",SPEED);
		print("Complaining won’t get you anywhere.",SPEED);
		print("The bus arrives, and you hop on.",SPEED);
		print("You have a really \033[1;32mproductive\033[0m school day.",SPEED);
		print("You manage to answer every question correctly at school, though your name feels a bit more off every time you hear it.",SPEED);
		print("Probably a jamias vu moment.",SPEED);
		print("The school day is full of \033[1;32mjoy\033[0m, but soon it comes to an end, and you are on the return bus home.\n\n",SPEED);
	}
	print(bc,0);
	printf("what is your choice: ");
	fflush(stdout);
	while (b!=1 && b!=2){
		b=(getch()-'0');
	}
	printf("%d\n\n",b);
	if (b==1){
		if (a==1){
			print("Scrolling through your phone has started feeling worse than ever before.",SPEED);
			print("You watch as other people have the life you dream of.",SPEED);
			print("This makes you feel… feel…",SPEED+250);
			print("…\033[1;31minvisible\033[0m. Yeah, that’s the word.",SPEED);
			print("A sudden feeling creeps into your head and causes your chest to tighten.",SPEED);
			print("\033[1;31mWhat if you were never there?\033[0m",SPEED+750);
			print("You distract yourself with flashes of colour on your phone's screen.",SPEED);
		} else {
			print("You scroll through your phone and feel alive.",SPEED);
			print("Watching people do what they love brings back \033[1;32mhappy memories\033[0m from your own past.",SPEED);
			print("You almost feel like posting your own videos, but decide it’s not worth the hassle.",SPEED);
			print("\033[1;32mWarmth\033[0m spreads through your chest, and you feel better than this morning.",SPEED);
			print("Thoughts of what you could do fill your mind as you ride the bus home.",SPEED);
		}
	} else {
		if (a==1){
			print("As you stare out the window, you wonder what is wrong with you.",SPEED);
			print("Everyone else seems happy and outgoing, but you can barely leave your home for school.",SPEED);
			print("They are all \033[1;31mfit\033[0m, and you can’t even get yourself to go to the gym.",SPEED);
			print("They are all \033[1;31msmart\033[0m, but you can never focus while trying to learn.",SPEED);
			print("Why are you this way? Why did you have to be so bad at everything?",SPEED);
			print("A sudden feeling creeps into your head and causes your chest to tighten.",SPEED);
			print("\033[1;31mWhat if you were never there?\033[0m",SPEED+750);
			print("You distract yourself by thinking of what you will eat for lunch.",SPEED);
		} else {
			print("The world is \033[1;32mbright\033[0m and \033[1;32mcheery\033[0m today as you watch people wander through the busy streets.",SPEED);
			print("You see a young boy returning a wallet to a man and a lady helping an elderly couple carry their groceries.",SPEED);
			print("You can’t help but think of how the world is a \033[1;32mbeautiful\033[0m place.",SPEED);
			print("And you get the opportunity to be here and experience it unfold.",SPEED);
			print("\033[1;32mWarmth\033[0m spreads through your chest, and you feel \033[1;32mbetter\033[0m than this morning.",SPEED);
			print("Thoughts of what you could do fill your mind as you ride the bus home.",SPEED);
		}
	}
	print("You enter your home and set your stuff up in your bedroom.",SPEED);
	print("The night is silent, except for the occasional chirp of crickets and the croaking of frogs.\n\n",SPEED);
	print(cc,0);
	printf("what is your choice: ");
	fflush(stdout);
	while (c!=1 && c!=2){
		c=(getch()-'0');
	}
	printf("%d\n\n",c);
	if (c==1){
		if (a==1){
			print("You go back to the bathroom and stare.",SPEED+250);
			print("Every single \033[1;31mminor imperfection\033[0m feels \033[1;31mmajor\033[0m in the moment, and you feel like you don’t have the will to continue this miserable life any longer.",SPEED);
			print("You leave the bathroom, but don’t manage to leave your house the next day, or the day after.",SPEED);
			print("A week goes by before your friend decides to investigate.",SPEED);
			print("They yelp as they enter your room.",SPEED);
			print("At least you made the headline for the month?",SPEED);
			print("\n\033[1;31m*** BAD ENDING: “Hanging Out” ***",-1);
		} else {
			print("You look into the mirror and wonder what would happen if you acted how you wanted to.",SPEED);
			print("You strike a couple of poses and \033[1;32mlaugh\033[0m as you do each one, full of \033[1;32mjoy\033[0m and \033[1;32menergy\033[0m.",SPEED);
			print("You feel \033[1;32mhappy\033[0m and \033[1;32malive\033[0m, as if you were removing your mask and letting your soul breathe.",SPEED);
			print("You do this for an hour and go to bed \033[1;32mradiating joy\033[0m.",SPEED);
			print("This is what life is meant to be like.",SPEED);
			print("\n\033[1;32m*** GOOD ENDING: Trying Things Out ***",-1);
		}
	} else {
		if (a==1){
			print("You lie in bed, your laptop in front of you.",SPEED);
			print("You search the web for potential reasons why you feel this way.",SPEED);
			print("The conclusion you come up with is that you are probably \033[1;31mdepressed\033[0m and \033[1;31msuicidal\033[0m.",SPEED);
			print("You now know you need help, but a part of you doesn’t want to get it.",SPEED);
			print("Keeping everything back and turning the lights off, you go to bed for tomorrow.",SPEED);
			print("\n*** NEUTRAL ENDING: A Work In Progress ***",-1);
		} else {
			print("You lie in bed, your laptop in front of you.",SPEED);
			print("You search the web for potential reasons why you feel this way.",SPEED);
			print("The conclusion you get is simple and nearly obvious.",SPEED);
			print("You are \033[1;32mtransgender\033[0m.",SPEED);
			print("\033[1;32mPride\033[0m surges through your chest as you see comments from other people like you.",SPEED);
			print("You decide to go online and buy some clothes to feel more like yourself.",SPEED);
			print("As you rest your eyes, you feel \033[1;32mhappy\033[0m knowing the clothes will arrive in a week.",SPEED);
			print("\n\033[1;32m*** GOOD ENDING: Transgender ***",-1);
		}
	}
}
