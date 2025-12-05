import sqlite3
from getpass import getpass

class Database:
    def __init__(self, db_name):
        self.conn = sqlite3.connect(db_name)
        self.cursor = self.conn.cursor()
        self.create_tables()

    def create_tables(self):
        self.cursor.execute('''
            CREATE TABLE IF NOT EXISTS users
            (username TEXT PRIMARY KEY, password TEXT)
        ''')
        self.cursor.execute('''
            CREATE TABLE IF NOT EXISTS messages
            (id INTEGER PRIMARY KEY AUTOINCREMENT,
             sender TEXT,
             receiver TEXT,
             message TEXT,
             timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
             FOREIGN KEY(sender) REFERENCES users(username),
             FOREIGN KEY(receiver) REFERENCES users(username))
        ''')
        self.conn.commit()

    def register_user(self, username, password):
        try:
            self.cursor.execute('INSERT INTO users VALUES (?, ?)',
                               (username, password))
            self.conn.commit()
            print("User registered successfully!")
        except sqlite3.IntegrityError:
            print("Username already exists!")

    def login_user(self, username, password):
        self.cursor.execute('SELECT * FROM users WHERE username = ? AND password = ?',
                           (username, password))
        if self.cursor.fetchone():
            print("Login successful!")
            return True
        else:
            print("Invalid username or password!")
            return False

    def send_message(self, sender, receiver, message):
        self.cursor.execute('''
            INSERT INTO messages (sender, receiver, message)
            VALUES (?, ?, ?)
        ''', (sender, receiver, message))
        self.conn.commit()
        print("Message sent successfully!")

    def view_messages(self, username):
        self.cursor.execute('''
            SELECT sender, message, timestamp
            FROM messages
            WHERE receiver = ?
            ORDER BY timestamp DESC
        ''', (username,))
        messages = self.cursor.fetchall()
        if messages:
            print("Your messages:")
            for msg in messages:
                print(f"From: {msg[0]}")
                print(f"Message: {msg[1]}")
                print(f"Time: {msg[2]}\n")
        else:
            print("No messages yet!")

class User:
    def __init__(self, username, password):
        self.username = username
        self.password = password

class MessagingApp:
    def __init__(self, db_name):
        self.db = Database(db_name)
        self.logged_in_user = None

    def run(self):
        while True:
            print("\n1. Register")
            print("2. Login")
            print("3. Exit")
            choice = input("Choose an option: ")
            if choice == '1':
                self.register()
            elif choice == '2':
                self.login()
            elif choice == '3':
                break
            else:
                print("Invalid choice. Please choose again.")

    def register(self):
        username = input("Enter username: ")
        password = getpass("Enter password: ")
        self.db.register_user(username, password)

    def login(self):
        username = input("Enter username: ")
        password = getpass("Enter password: ")
        if self.db.login_user(username, password):
            self.logged_in_user = username
            self.user_menu()

    def user_menu(self):
        while True:
            print("\n1. Send Message")
            print("2. View Messages")
            print("3. Logout")
            choice = input("Choose an option: ")
            if choice == '1':
                self.send_message()
            elif choice == '2':
                self.view_messages()
            elif choice == '3':
                self.logged_in_user = None
                break
            else:
                print("Invalid choice. Please choose again.")

    def send_message(self):
        receiver = input("Enter receiver's username: ")
        message = input("Enter your message: ")
        self.db.send_message(self.logged_in_user, receiver, message)

    def view_messages(self):
        self.db.view_messages(self.logged_in_user)

if __name__ == "__main__":
    app = MessagingApp("messaging_app.db")
    app.run()
