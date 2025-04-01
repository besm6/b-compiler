/* The following program will print each number from 1 to 100.
   If a number is divisible by 3 or 5, it will print `Fizz` or
   `Buzz` instead. If a number is divisible by both 3 and 5,
   `FizzBuzz` gets printed.
*/

n 100;

main() {
    extrn printn, write, n;
    auto i;

    while(i++ < n) {
        if(0 == i % 15)
            write('FizzBuzz');
        else if(0 == i % 3)
            write('Fizz');
        else if(0 == i % 5)
            write('Buzz');
        else
            printn(i, 10);
        write('*n');
    }
}
