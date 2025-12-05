import time

def welcome_message():
    print("Welcome to the Expense Manager System!")

def get_user_choice():
    while True:
        try:
            choice = input('1. Add Expense\n2. Add Saving\n3. View Totals\n4. Add Logs\n5. Read Logs\n')
            if choice in ['1', '2', '3', '4', '5']:
                return choice
            else:
                print("Invalid choice. Please try again.")
        except ValueError:
            print("Invalid input. Please try again.")

def get_integer_input(prompt):
    while True:
        try:
            return int(input(prompt))
        except ValueError:
            print("Invalid input. Please try again.")

def get_float_input(prompt):
    while True:
        try:
            return float(input(prompt))
        except ValueError:
            print("Invalid input. Please try again.")

def calculate_total(array):
    total = 0
    for i in array:
        try:
            total += float(i)
        except ValueError:
            print('Invalid input. Please try again.')
    return total

def add_transaction(total_expenses, total_savings, transactions_data, choice):
    choice=get_user_choice()
    if choice == '1':
        all_expenses = []
        iteration = get_integer_input('How many expenses do you want to add? ')
        for i in range(iteration):
            expense = input(f'Enter your {i+1} expense amount: ')
            all_expenses.append(expense)
        print(all_expenses)
        total_expenses = calculate_total(all_expenses)
        transactions_data.append(total_expenses)
        return total_expenses, total_savings, transactions_data

    elif choice == '2':
        all_savings = []
        iteration = get_integer_input('How many savings do you want to add? ')
        for i in range(iteration):
            savings = input(f'Enter your {i+1} saving amount: ')
            all_savings.append(savings)
        print(all_savings)
        total_savings = calculate_total(all_savings)
        transactions_data.append(total_savings)
        return total_expenses, total_savings, transactions_data

    elif choice == '3':
        print("What expenses do you want to see?")
        choice2 = input('1. View Savings\n2. View Expenses\n')
        if choice2 == '1':
            print(total_savings)
        elif choice2 == '2':
            print(total_expenses)
        else:
            balance = total_savings - total_expenses
            print(f'The balance is {balance}')
            transactions_data.append(balance)
        return total_expenses, total_savings, transactions_data

    elif choice == '4':
        add_logs(transactions_data)
        return total_expenses, total_savings, transactions_data

    elif choice == '5':
        read_logs()
        return total_expenses, total_savings, transactions_data

def add_logs(transactions_data):
    current_time = time.strftime("%Y-%m-%d %H:%M:%S")
    content = f'{current_time}-'
    for log in transactions_data:
        content += str(log) + '-'+'\n'
    with open('logs.txt', 'a+') as file:
        print("Content written to file")
        file.write(content + '\n')

def read_logs():
    with open('logs.txt', 'r+') as file:
        content = file.read()
        print(content)

def main():
    welcome_message()
    total_expenses = 0
    total_savings = 0
    transactions_data = []
    while True:
        choice = get_user_choice()
        total_expenses, total_savings, transactions_data = add_transaction(total_expenses, total_savings, transactions_data,choice)

if __name__ == "__main__":
    main()