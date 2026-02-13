// Encapsulation Example in Java
public class Person {
    // Private fields - cannot be accessed directly outside this class
    private String name;
    private int age;

    // Constructor
    public Person(String name, int age) {
        setName(name);
        setAge(age);
    }

    // Getter for name
    public String getName() {
        return name;
    }

    // Setter for name with validation
    public void setName(String name) {
        if (name != null && !name.trim().isEmpty()) {
            this.name = name;
        } else {
            System.out.println("Invalid name. Keeping previous value.");
        }
    }

    // Getter for age
    public int getAge() {
        return age;
    }

    // Setter for age with validation
    public void setAge(int age) {
        if (age >= 0 && age <= 120) {
            this.age = age;
        } else {
            System.out.println("Invalid age. Keeping previous value.");
        }
    }

    // Method to display person details
    public void displayInfo() {
        System.out.println("Name: " + name + ", Age: " + age);
    }

    // Main method to test encapsulation
    public static void main(String[] args) {
        Person p1 = new Person("Alice", 25);
        p1.displayInfo();

        // Trying to set invalid values
        p1.setAge(-5); // Will be rejected
        p1.setName(""); // Will be rejected

        // Updating with valid values
        p1.setAge(30);
        p1.setName("Bob");

        p1.displayInfo();
    }
}