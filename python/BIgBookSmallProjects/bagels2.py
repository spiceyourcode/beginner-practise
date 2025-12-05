import random
# Constants
NUM_DIGITS = 3  # Number of digits in the secret number
MAX_GUESSES = 10  # Maximum number of guesses allowed

def main():
    print_intro()
    
    while True:  # Main game loop
        secret_num = get_secret_num()
        print('I have thought up a number.')
        print(f'You have {MAX_GUESSES} guesses to get it.')

        num_guesses = 1
        while num_guesses <= MAX_GUESSES:
            guess = get_guess(num_guesses)

            clues = get_clues(guess, secret_num)
            print(clues)

            if guess == secret_num:
                print('You got it!')
                break  # Player guessed correctly

            num_guesses += 1

            if num_guesses > MAX_GUESSES:
                print('You ran out of guesses.')
                print(f'The answer was {secret_num}.')

        if not play_again():
            break

    print('Thanks for playing!')

def print_intro():
    """Prints the introduction of the game."""
    print('''Bagels, a deductive logic game.
By Al Sweigart al@inventwithpython.com

I am thinking of a {}-digit number with no repeated digits.
Try to guess what it is. Here are some clues:
When I say:    That means:
  Pico         One digit is correct but in the wrong position.
  Fermi        One digit is correct and in the right position.
  Bagels       No digit is correct.
'''.format(NUM_DIGITS))

def get_secret_num():
    """Returns a string made up of NUM_DIGITS unique random digits."""
    numbers = list('0123456789')  # Create a list of digits 0 to 9
    random.shuffle(numbers)  # Shuffle them into random order
    return ''.join(numbers[:NUM_DIGITS])  # Join the first NUM_DIGITS digits

def get_guess(guess_number):
    """Prompts the user for a valid guess."""
    while True:
        guess = input(f'Guess #{guess_number}: ')
        if len(guess) == NUM_DIGITS and guess.isdecimal():
            return guess
        print(f'Please enter a {NUM_DIGITS}-digit number with no repeated digits.')

def get_clues(guess, secret_num):
    """Returns a string with the pico, fermi, bagels clues for a guess and secret number pair."""
    if guess == secret_num:
        return 'You got it!'

    clues = []
    for i in range(len(guess)):
        if guess[i] == secret_num[i]:
            clues.append('Fermi')  # Correct digit in the correct place
        elif guess[i] in secret_num:
            clues.append('Pico')  # Correct digit in the wrong place

    return ' '.join(sorted(clues)) if clues else 'Bagels'  # Sort clues and return

def play_again():
    """Asks the player if they want to play again."""
    return input('Do you want to play again? (yes or no) ').lower().startswith('y')

# If the program is run (instead of imported), run the game:
if __name__ == '__main__':
    main()