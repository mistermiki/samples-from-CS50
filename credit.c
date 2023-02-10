#include <cs50.h>
#include <stdio.h>

long get_number(void);
bool is_valid(long number);
bool is_visa(long number);
bool is_amex(long number);
bool is_master(long number);

int main(void)
{
    //promt user for card number
    long number = get_number();

    //check if the number is valid
    bool valid = is_valid(number);

    if (valid)
    {
        //check if it's VISA
        bool visa = is_visa(number);

        if (visa)
        {
            printf("VISA\n");
        }
        else
        {
            //check if it's American Express
            bool amex = is_amex(number);

            if (amex)
            {
                printf("AMEX\n");
            }
            else
            {
                //check if it's Mastercard
                bool master = is_master(number);

                if (master)
                {
                    printf("MASTERCARD\n");
                }
                else
                {
                    //the card number is valid, but the provider is unknown
                    printf("INVALID\n");
                }
            }
        }
    }
    else
    {
        //the card number is invalid
        printf("INVALID\n");
    }

}

long get_number(void)
{
    long number;

    do
    {
        number = get_long("Number: ");
    }
    while (number < 0);

    return number;
}

bool is_valid(long number)
{
    long n = number;
    int lenght = 1;
    int sum = 0;
    int sum2 = 0;

    //calculate the card number lenght
    while (n >= 10)
    {
        n = n / 10;
        lenght++;
    }

    //lenght fits in the usual card number lenghts
    if (lenght >= 13 && lenght <= 16)
    {
        //use Luhn's Algorithm to verify card number
        int last_digit;
        n = number;

        while (n > 0)
        {
            //calculate first sum
            last_digit = (n % 10);
            sum = sum + last_digit;

            //go to the next last digit
            n = n / 10;

            //calculate second sum
            last_digit = (n % 10);
            sum2 = sum2 + ((last_digit * 2) % 10) + ((last_digit * 2 / 10) % 10);

            //go to the next last digit
            n = n / 10;
        }

        //check Luhn's condition for validity
        if ((sum + sum2) % 10 == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        //card number lenght is outside usual lenghts
        return false;
    }
}

bool is_visa(long number)
{
    //get first digit
    while (number >= 10)
    {
        number = number / 10;
    }

    //check if first digit is characteristic for VISA
    if (number == 4)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_amex(long number)
{
    //get first two digits
    while (number >= 100)
    {
        number = number / 10;
    }

    //check if first two digits are characteristic for American Express
    if (number == 34 || number == 37)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_master(long number)
{
    //get first two digits
    while (number >= 100)
    {
        number = number / 10;
    }

    //check if first two digits are characteristic for Mastercard
    if (number >= 51 && number <= 55)
    {
        return true;
    }
    else
    {
        return false;
    }
}