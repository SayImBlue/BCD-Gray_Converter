#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void BinConv4(int dec) { // Simple binary conversion and printing function
    for (int i = 3; i >= 0; i--) {
        /* We shift the number to the right by i bits (to a max of 3) then use & 1 to keep the least significant bit
        and we print it, meaning we print the first bit of the number then the second, third and finally fourth
        */
        printf("%d", (dec >> i) & 1);
    }
    printf(" ");
}

int ConvBin4(int* bin, int n) { // Simple decimal conversion function
    int dec = 0;
    /* The function takes an array containing singular bits of a binary number, here we multiply that bit by (1 << (n - i))
    which is 2 to the power n - i, because shifting to the left is equivalent to multiplying the number by 2, so for i = 0
    (1 << (n - i)) is 2^n times the far most left bit, then its added to dec
    */
    for (int i = 0; i <= n; i++) {
        dec += bin[i] * (1 << (n - i));
    }
    return dec;
}

void Dec_2_BCD(int n) {
    int dcount = 0, digit = n;
    int* digits = NULL; // Initializing a NULL array that we're gonna be dynamically reallocing memory to

    while (digit > 0) { // This loop parses our number digit by digit and stores each digit in the array "digits"
        digits = (int*)realloc(digits, (dcount + 1) * sizeof(int)); //Dynamic reallocatytion depending on the number of digits
        digits[dcount] = digit % 10; //This parses the for most right digit
        dcount++;
        digit /= 10; // We make sure to divide our initial number by 10 each time, to apply this process to the next digit
    }

    printf("The decimal number %d in BCD is :  ", n);
    for (int i = dcount - 1; i >= 0; i--) { // We print the 4 bits binary equivalent of each digit
        BinConv4(digits[i]);
    }
    printf("\n");
    free(digits);
}

void BCD_2_Dec(char* bin) { /* The function uses a string array as a parameter because it more efficient to convert large BCD numbers,
     even "888" is super large and poses issues when converted in previous versions of this function, 
     that used the number as an integer instead of a string
    */
    int len = strlen(bin);
    if (len % 4 != 0) { /* Verify that its a BCD number written in blocks of 4 bits, 
        this is essential because all the operations are based on the 4 bits block
        we could automatically add leading zeroes to always have blocks of 4 bits but yeah :>
        */
        printf("Invalid BCD input\n"); 
        return;
    }

    int dec = 0, multiplier = 1;

    for (int i = len - 4; i >= 0; i -= 4) { /* Here we initialize i to len - 4, meaning i corresponds to the first bit of the right for most
    block of 4 bits, it decrements by 4 to go the previous 4 bits block. The utility of this comes in the next process
    */
        int digit = 0;
        /* Here j starts at 0 and goes up to 3, notice we use bin[i + j] meaning we start with the most significant bit of the 4 bits block,
        multiply it by its corresponding power of 2 which is (3 - j) then add it to sum, when we increment j, we go to the second most significant
        bit of the 4 bits block, and multiply it by its corresponding power of 2 etc...
        This means that for j = 0, we have bin[i] and 1 << (3 - j) is 8 (it can be a bit confusing to understand, so here's an example :
        lets say we have 1000 1010, here strlen = 8, i starts at 4, thus we are working in this block : 1010, for j = 0 bin[i + j] = 1,
        which is the most significant bit of that block, when we treat all the block, i becomes 0, that means we work on : 1000)
        */
        for (int j = 0; j < 4; j++) {
            if (bin[i + j] == '1') {
                digit += (1 << (3 - j)); // Notice its the same method as ConvBin4 with a parameter of 3 (yes I could have used the function but I need to make some pretty annoying changes so yeah :> )
            }
        }
        if (digit > 9) { // Ensure correct BCD number input
            printf("Invalid BCD input\n");
            return;
        }
        dec += digit * multiplier; //Instead of using an array to print out each individual digit, we combine them in one using the powers of 10
        multiplier *= 10;
    }

    printf("The binary BCD encoded number in Decimal is : %d\n", dec);
}

void Dec_2_Gray(int n) {
    int digcount = 0, started = 0;
    int* nums = NULL; //Again, we initialize a NULL array tto dynamically reallocate memory to

    /* Ok so here its a pretty know method to convert decimals to binary, knowing the fact that one byte is 8 bits and that C allocates 4 bytes (32 bits)
    to integers we use that to go over all the bits in a number and to not have to manually count the number of bits (most of the number is encoded on a small size anyways so most of the result is all zeroes)
    the (sizeof(n)) is just an additional security because certain 64 bits compilers might use 64 bits instead of 32 bits that is used by GNU for example
    Anyways, here i corresponds to the right far most bit, we use again the right shift + bitwise & to select that bit in particular, here we have a condition "started"
    because we're going over 32 bits, most of them are zeroes, so whenever we find the first non-zero bit, is started is switched to 1,
    which makes it so we start stocking bits only when all the leading zeroes are gone
    */
    for (int i = sizeof(n) * 8 - 1; i >= 0; i--) {
        int bit = (n >> i) & 1;
        nums = (int*)realloc(nums, (digcount + 1) * sizeof(int)); //The reallocating happens here depending on the digit count
        if (bit) started = 1;
        if (started) {
            nums[digcount] = bit;
            digcount++;
        }
    }

    printf("The binary Gray encoded number %d in Decimal is : 1", n);
    // Here we print the number after applying the XOR function to the bits of our number (we also put a "1" at the beginning)
    for (int i = 0; i < digcount - 1; i++) {
        printf("%d", nums[i] ^ nums[i + 1]);
    }
    printf("\n");
    free(nums);
}

void Gray_2_Dec(char* gray) {
    int len = strlen(gray);
    int* nums = (int*)malloc(len * sizeof(int));
    int* decs = (int*)malloc(len * sizeof(int));

    //This loop stores the number in "gray" aka our input into "nums" by converting the char ("0" or "1") into an integer (0 or 1) by substracting "0" from the ASCII value of the char
    for (int i = 0; i < len; i++) {
        nums[i] = gray[i] - '0';
    }

    decs[0] = nums[0]; // We do this because we use i - 1 in our logic so if we initialize i to 0 for our loop we'll have decs[-1] which doesntt exist
    for (int i = 1; i < len; i++) {
        decs[i] = decs[i - 1] ^ nums[i]; // Here we simply apply XOR to convert from Gray to Decimal (We compare the nth bit from the converted number to the n+1th number from the number to convert aka first with second etc)
    }

    printf("The binary Gray encoded number %s in Decimal is : %d\n", gray, ConvBin4(decs, len - 1)); //Here we simply print out the Decimal converted number
    free(nums);
    free(decs);
}

int main() { //There is nothing much to comment on about the main function, simple switch cases for picking choices etc
    int choice;
    char bin[128];
    int num;

    do {
        printf("What binary coded numbers are we working with ?\n");
        printf("1-BCD\n2-Gray\n3-Exit\n");
        printf("Option : ");
        scanf("%d", &choice);
        if (choice > 3) printf("Invalid choice please try again\n");
    } while (choice > 3);

    switch (choice) {
        case 1:
            printf("What conversion do you want to make ?\n");
            printf("1-Decimal to BCD\n2-BCD to Decimal\n");
            printf("Option : ");
            scanf("%d", &choice);
            if (choice == 1) {
                printf("Please enter an integer in decimal to convert : ");
                scanf("%d", &num);
                Dec_2_BCD(num);
            } else if (choice == 2) {
                printf("Please enter a BCD encoded binary number to convert : ");
                scanf("%s", bin);
                BCD_2_Dec(bin);
            } else {
                printf("Invalid choice\n");
                return 0;
            }
            break;

        case 2:
            printf("What conversion do you want to make ?\n");
            printf("1-Decimal to Gray\n2-Gray to Decimal\n");
            printf("Option : ");
            scanf("%d", &choice);
            if (choice == 1) {
                printf("Please enter an integer in decimal to convert : ");
                scanf("%d", &num);
                Dec_2_Gray(num);
            } else if (choice == 2) {
                printf("Please enter a Gray encoded binary number to convert : ");
                scanf("%s", bin);
                Gray_2_Dec(bin);
            } else {
                printf("Invalid choice\n");
            }
            break;

        case 3:
            return 0;

        default:
            return 0;
    }

    return 0;
}
