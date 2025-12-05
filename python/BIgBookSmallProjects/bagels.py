import random 

intro = """
Welcom to Bagels! A deductive logic game, you must guess a secret three-digit number 
based on clues. The game offers one of the following hints in response to your guess:
“Pico”           when your guess has a correct digit in the wrong place,
“Fermi”          when your guess has a correct digit in the correct place
"Bagels”         If your guess has no correct digits.

You have 10 tries to guess the secret number.
"""
attempts = 10
def get_user_guess():    
    global attempts
    while attempts > 0: #continue if the attempts are greater than 0
        try:
            value=input(f"Enter your guess having a three integer i.e '123 attempt {attempts} : ") 
            if len(value)!= 3 and not value.isdigit():
                print("Invalid input. Please enter exactly three digits (e.g., '123').")
            else:
                attempts-=1
                return str(value)
        except Exception as e:
            print(f"an error occured : {e}  please try having a valid input")  
    print("No attempts left. Please try again later.")
    return None # Return None if no valid guess was made                  
def get_comp_guess():
    return str(random.randint(100,999))

def check_number(get_user_guess, get_comp_guess):
    comp = get_comp_guess()
    while True:
        user = get_user_guess()
        print('computers guess: ',comp)
        
        for i, num in enumerate(comp):
            for j , value in enumerate(user):
                # print(f"Iteration: Comp={i, num}, User={j, value}")
                if num == value:
                    if i ==j:
                        print("Match found")
         

def main():
    get_comp_guess()
    print(intro)
    while attempts > 0:
        get_user_guess()
        if get_user_guess is None:  # If no valid guess was made, exit the loop
            break
        check_number(get_user_guess, get_comp_guess)
   
if __name__ == "__main__":
    main()

    