public class Int {
  int x;  // Class variable x

  // Constructor with one parameter x
  public Int(int x) {
    this.x = x; // refers to the class variable x
  }

  public static void main(String[] args) {
    // Create an object of Int and pass the value 5 to the constructor
    Int myObj = new Int(5);
    System.out.println("Value of x = " + myObj.x);
  }
}