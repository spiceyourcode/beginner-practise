print("type help to  show options ")
game_input =""
started= False
stopped=False
while game_input != "quit":
    game_input= input(">").lower()

    if game_input == "start":
        if started:
            print("the car is alredy moving use stop to stop the car")
        else: 
            started = True   
            print("the car has started moving!!")
                     
    elif game_input == "stop":
        if stopped:
            print("The car has already stopped")
        else:
            stopped=True
            print("The car is not moving ")
    
    elif game_input == "help":
        print("""
            start - to start the car
            stop- to stop the car 
            help - do display this options  
            """)

confirmation=input("are you sure you want to quit? y/n").lower()
if confirmation == "y":
    print("Goodbye")
else:
    print("continue playing")
