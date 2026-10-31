use std::*;

const SPEED: i64 = 2750;

fn main(){
    let ac: &str = "---CHOICE a---\n1. Continue staring at yourself\n2. Get dressed ";
    let bc: &str="---CHOICE b---\n1. Scroll on any social media of your choice(Pinterest/Twitter/Youtube/ETC)\n2. Stare out the window";
    let cc: &str="---CHOICE c---\n1. Go stare at the mirror\n2. Research on possible reasons you feel this way";
    print("\x1b[1;31m[!WARNING¡] THIS GAME HAS 1 OUTCOME WITH A REFFRENCE TO SUICIDE, PROCEED AT YOUR OWN RISK [¡GNINRAW!]\x1b[0m", -1);
    print("\x1b[2J\x1b[H", 500);
    print("You wake up in your room, tired, and walk into the bathroom.", SPEED);
    print("The only thing on your mind is the long day ahead… so tiring.", SPEED);
    print("Staring at the mirror, your heart aches with a sudden longing.", SPEED);
    print("Longing to be who you truly are.", SPEED);
    print("You look at yourself and \x1b[1;31mcritique\x1b[0m your bodily features.", SPEED);
    print("Why were you born this way?", SPEED);
    println!("");
    print(ac,0);
    println!("What is your choice: ");
    let mut a: i32 = 4;
    input(&mut a);
    if a==1 {
        print("You stare at yourself for about an hour, \x1b[1;31mcritiquing\x1b[0m every little detail.",SPEED);
        print("Then you realised you are going to miss the bus if you don’t hurry up.",SPEED);
        print("Panting, you reach the bus stop; the bus arrives, and you go in.",SPEED-250);
        print("Phew, that was close.",SPEED);
        print("At school, you nearly fall asleep, but luckily, your friend wakes you up.",SPEED);
        print("You wish you were more like them than the \x1b[1;31mlazy\x1b[0m child you are.",SPEED);
        print("The teacher calls your name, and it feels off, but you answer and move on.",SPEED);
        print("The school day feels like a century, but soon it comes to an end, and you are on the return bus home.",SPEED+750);
    } else {
        print("Sighing, you get ready and leave for the bus stop.",SPEED);
        print("Complaining won’t get you anywhere.",SPEED);
        print("The bus arrives, and you hop on.",SPEED);
        print("You have a really \x1b[1;32mproductive\x1b[0m school day.",SPEED);
        print("You manage to answer every question correctly at school, though your name feels a bit more off every time you hear it.",SPEED);
        print("Probably a jamias vu moment.",SPEED);
        print("The school day is full of \x1b[1;32mjoy\x1b[0m, but soon it comes to an end, and you are on the return bus home.",SPEED);
    }
    let mut b: i32 = 4;
    print(bc,0);
    println!("What is your choice: ");
    input(&mut b);
    if b == 1 {
        if a==1 {
            print("Scrolling through your phone has started feeling worse than ever before.",SPEED);
            print("You watch as other people have the life you dream of.",SPEED);
            print("This makes you feel… feel…",SPEED+250);
            print("…\x1b[1;31minvisible\x1b[0m. Yeah, that’s the word.",SPEED);
            print("A sudden feeling creeps into your head and causes your chest to tighten.",SPEED);
            print("\x1b[1;31mWhat if you were never there?\x1b[0m",SPEED+750);
            print("You distract yourself with flashes of colour on your phone's screen.",SPEED);
        } else {
            print("You scroll through your phone and feel alive.",SPEED);
            print("Watching people do what they love brings back \x1b[1;32mhappy memories\x1b[0m from your own past.", SPEED);
            print("You almost feel like posting your own videos, but decide it’s not worth the hassle.",SPEED);
            print("\x1b[1;32mWarmth\x1b[0m spreads through your chest, and you feel better than this morning.", SPEED);
            print("Thoughts of what you could do fill your mind as you ride the bus home.",SPEED);
        }
    } else {
        if a==1 {
            print("As you stare out the window, you wonder what is wrong with you.",SPEED);
            print("Everyone else seems happy and outgoing, but you can barely leave your home for school.",SPEED);
            print("They are all \x1b[1;31mfit\x1b[0m, and you can’t even get yourself to go to the gym.",SPEED);
            print("They are all \x1b[1;31msmart\x1b[0m, but you can never focus while trying to learn.",SPEED);
            print("Why are you this way? Why did you have to be so bad at everything?",SPEED);
            print("A sudden feeling creeps into your head and causes your chest to tighten.",SPEED);
            print("\x1b[1;31mWhat if you were never there?\x1b[0m",SPEED+750);
            print("You distract yourself by thinking of what you will eat for lunch.",SPEED);
        } else {
            print("The world is \x1b[1;32mbright\x1b[0m and \x1b[1;32mcheery\x1b[0m today as you watch people wander through the busy streets.",SPEED);
            print("You see a young boy returning a wallet to a man and a lady helping an elderly couple carry their groceries.",SPEED);
            print("You can’t help but think of how the world is a \x1b[1;32mbeautiful\x1b[0m place.",SPEED);
            print("And you get the opportunity to be here and experience it unfold.",SPEED);
            print("\x1b[1;32mWarmth\x1b[0m spreads through your chest, and you feel \x1b[1;32mbetter\x1b[0m than this morning.",SPEED);
            print("Thoughts of what you could do fill your mind as you ride the bus home.",SPEED);
        }
    }
    print("\nEntering your home, you set your stuff up in your bedroom.",SPEED);
    print("The night is silent, except for the occasional chirp of crickets and the croaking of frogs.",SPEED);
    print(cc,0);
    println!("what is your choice: ");
    let mut c: i32 = 4;
    input(&mut c);
    if c==1 {
        if a==1 {
            print("You go back to the bathroom and stare.",SPEED+250);
            print("Every single \x1b[1;31mminor imperfection\x1b[0m feels \x1b[1;31mmajor\x1b[0m in the moment, and you feel like you don’t have the will to continue this \x1b[1;31miserable\x1b[0m life any longer.",SPEED);
            print("You leave the bathroom, but don’t manage to leave your house the next day, or the day after", SPEED);
            print("A week goes by before your friend decides to investigate.",SPEED);
            print("They yelp as they enter your room.",SPEED);
            print("At least you made the headline for the month?",SPEED);
            print("\n\x1b[1;31m*** BAD ENDING: “Hanging Out” ***",-1);
        } else {
            print("You look into the mirror and wonder what would happen if you acted how you wanted to.",SPEED);
            print("You strike a couple of poses and \x1b[1;32mlaugh\x1b[0m as you do each one, full of \x1b[1;32mjoy\x1b[0m and \x1b[1;32menergy\x1b[0m.",SPEED);
            print("You feel \x1b[1;32mhappy\x1b[0m and \x1b[1;32malive\x1b[0m, as if you were removing your mask and letting your soul breathe.",SPEED);
            print("You do this for an hour and go to bed \x1b[1;32mradiating joy\x1b[0m.",SPEED);
            print("This is what life is meant to be like.",SPEED);
            print("\n\x1b[1;32m*** GOOD ENDING: Trying Things Out ***",-1);
        }
    } else {
        if a==1 {
            print("You lie in bed, your laptop in front of you.",SPEED);
            print("You search the web for potential reasons why you feel this way.",SPEED);
            print("The conclusion you come up with is that you are probably \x1b[1;31mdepressed\x1b[0m and \x1b[1;31msuicidal\x1b[0m.",SPEED);
            print("You now know you need help, but a part of you doesn’t want to get it.",SPEED);
            print("Keeping everything back and turning the lights off, you go to bed for tomorrow.",SPEED);
            print("\n*** NEUTRAL ENDING: A Work In Progress ***",-1);
        } else {
            print("You lie in bed, your laptop in front of you.",SPEED);
            print("You search the web for potential reasons why you feel this way.",SPEED);
            print("The conclusion you get is simple and nearly obvious.",SPEED);
            print("You are \x1b[1;32mtransgender\x1b[0m.",SPEED);
            print("\x1b[1;32mPride\x1b[0m surges through your chest as you see comments from other people like you.",SPEED);
            print("You decide to go online and buy some clothes to feel more like yourself.",SPEED);
            print("As you rest your eyes, you feel \x1b[1;32mhappy\x1b[0m knowing the clothes will arrive in a week.",SPEED);
            print("\n\x1b[1;32m*** GOOD ENDING: Transgender ***",-1);
        }
    }
}

fn input(val: &mut i32){
    while *val!=1 && *val!=2{
        let mut input = String::new();
        std::io::stdin()
            .read_line(&mut input)
            .expect("input failed");
        *val = input
            .trim()
            .parse()
            .expect("NAN");
    }
}

fn print<T: std::fmt::Display>(text: T, sleep: i64){
    let sleep_time: u64;
    if sleep<0 {
        sleep_time = 5000;
    } else {
        sleep_time = sleep as u64;
    }
    let time = time::Duration::from_millis(sleep_time);
    println!("{}", text);
    std::thread::sleep(time);
}
