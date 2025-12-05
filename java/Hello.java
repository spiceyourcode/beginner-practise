
import java.util.Scanner;

public class Hello {
    public static void findEvenOdd(int num){

        if(num%2 == 0){
            System.out.println("The number " +num +" is an even one ");
        }
        else{
            System.out.println("The number "+ num+ " is an odd number");
        }

    }
    public static void main(String[] args) {
        System.out.println("Please anter a number: ");
        try (Scanner scan = new Scanner(System.in)) {
            int number = scan.nextInt();

            show();    
            findEvenOdd(number);
        }       
        
    }
    static void show(){
        System.out.println("This is an example of a static method");
    }

}

