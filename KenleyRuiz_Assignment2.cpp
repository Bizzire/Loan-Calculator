/*
Assignment 2 - Loan Calculator
-------------------------------------
COP3014-001 : Programmming II
KenleyRuiz_Assignment2.cpp
Created by Kenley Ruiz on 2/05/24.
-------------------------------------
Last modified:
2/09/24 - 20:50
*/

#include <iostream>
#include <iomanip>
#include <math.h>
#include <string.h>

using namespace std;

// Function to get end users input
void get_user_input(string &, string &, string &);

// Calculation functions for loan program
double calc_apr_to_r(double);
double calc_amortization(double, int, double r);
void calc_payment_per_month(double, double, double &, double &, double &);
void calc_total_payment_summary(double, double, double &, double &);

// Display functions to display data to the end user
void display_tabular_info(int, double, double, double, double, double, double &, double &);
void display_summary(double, double, double);

// User input validation functions
void check_user_input_for_numbers_only(string&);
void convert_input(string, string, string, double &, double &, int &);
void check_user_input_for_whole_numbers_only(string&);
bool check_restart_program_input(string);

int main()
{
    string init_program;
    bool restart_program;

    do
    {
        double loan_amount = 0;
        string loan_amount_string = "";
        double annual_percentage_interest = 0;
        string annual_percentage_interest_string = "";
        double monthly_rate = 0;
        double monthly_interest_payment = 0;
        double monthly_principal_payment = 0;
        double monthly_payment = 0;
        double total_payment = 0;
        double total_interest_accumulated = 0;
        int term_length = 0;
        string term_length_string = "";

        get_user_input(loan_amount_string, annual_percentage_interest_string, term_length_string);
        convert_input(loan_amount_string, annual_percentage_interest_string, term_length_string, loan_amount, annual_percentage_interest, term_length);

        monthly_rate = calc_apr_to_r(annual_percentage_interest);
        monthly_payment = calc_amortization(loan_amount, term_length, monthly_rate);

        display_tabular_info(term_length, monthly_payment, monthly_rate, monthly_interest_payment, monthly_principal_payment, loan_amount, total_payment, total_interest_accumulated);
        display_summary(monthly_payment, total_payment, total_interest_accumulated);

        cout << "input 1 to restart program or input 0 to close the program" << endl;
        cin >> init_program;
        restart_program = check_restart_program_input(init_program);
        cout << endl;
    } while (restart_program);

    return 0;
}

// Get and allocate end user input
void get_user_input(string &loan_amount_string, string &apr_string, string &term_length_string)
{
    cout << "Please input loan amount" << endl;
    cin >> loan_amount_string;
    check_user_input_for_numbers_only(loan_amount_string);
    cout << "Please input annual percentage rate" << endl;
    cin >> apr_string;
    check_user_input_for_numbers_only(apr_string);
    cout << "Please input term length in terms of months (Whole numbers only)" << endl;
    cin >> term_length_string;
    check_user_input_for_numbers_only(term_length_string);
    check_user_input_for_whole_numbers_only(term_length_string);
    cout << endl << endl;
}

// Calculate annual percentage rate to monthly rate
double calc_apr_to_r(double apr)
{
    double result_1 = 1 + (apr/100);
    double result_2 = (double) 1/12;
    double monthly_rate = (double) (pow(result_1, result_2)) - 1.0;
    return monthly_rate;
}

// Calculate base monthly payment based on loan amount, term length, and monthly rate
double calc_amortization(double loan_amount, int term_length, double r)
{
    double monthly_payment;

    double result_1 = r * (pow((1+r),term_length));
    double result_2 = pow((1+r),term_length) - 1;
    double result_3 = (double) result_1 / result_2;

    monthly_payment = loan_amount * result_3;

    return monthly_payment;
}

// Calculate the monthly interest payment, monthly principal payment, and deduct from loan amount
void calc_payment_per_month(double monthly_payment, double monthly_rate, double &monthly_interest_payment, double &monthly_principal_payment, double &loan_amount)
{
    monthly_interest_payment = loan_amount * monthly_rate;
    monthly_principal_payment = monthly_payment - monthly_interest_payment;
    loan_amount -= monthly_principal_payment;

    if(loan_amount < 0)
        loan_amount = 0;
}

// Calculate and sum total payments and interest accumulated upon the end of the program
void calc_total_payment_summary(double monthly_payment, double monthly_interest_payment, double &total_payment, double &total_interest_accumulated)
{
    total_payment += monthly_payment;
    total_interest_accumulated += monthly_interest_payment;
}

/*
    Display all data in a table format to be easily read for the end user, each iteration will calculate month, beginning balance,
    monthly interest payment, monthly principal payment, and ending balance
*/
void display_tabular_info(int term_length, double monthly_payment, double monthly_rate, double monthly_interest_payment, double monthly_principal_payment, double loan_amount, double &total_payment, double &total_interest_accumulated)
{
    int count_years = 0;

    cout << "--------------------------------------------------------------------------------" << endl;
    cout << left << setw(6) << "Month\t" << left << setw(12) << "Beginning Balance\t" << left << setw(6) << "Interest\t" << left << setw(6) << "Principal\t" << left << setw(6) << "Ending Balance" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    
    for(int i = 1; i <= term_length; i++)
    {
        cout << left << setw(6) << i << "\t";
        cout << left << setw(18) << fixed << setprecision(2) << loan_amount << "\t";
        calc_payment_per_month(monthly_payment, monthly_rate, monthly_interest_payment, monthly_principal_payment, loan_amount);
        cout << left << setw(12) << fixed << setprecision(2) <<  monthly_interest_payment << "\t";
        cout << left << setw(12) << fixed << setprecision(2) <<  monthly_principal_payment << "\t";
        cout << left << setw(6) << fixed << setprecision(2) <<  loan_amount << "\t" <<  endl;
        calc_total_payment_summary(monthly_payment, monthly_interest_payment, total_payment, total_interest_accumulated);
        if((i % 12) == 0)
        {
            count_years++;
            cout << "--------------------------------------------------------------------------------" << endl;
            cout << "\t\t\t\tYear #" << count_years << " End" << endl;
        }
        cout << "--------------------------------------------------------------------------------" << endl;
    }
}

// Display accumulated payments throughout loan
void display_summary(double monthly_payment, double total_payment, double total_interest_accumulated)
{
    cout << endl;
    cout << fixed << setprecision(2) << "Payment Every Month\t $" << monthly_payment << endl;
    cout << fixed << setprecision(2) << "Total Payments\t $" << total_payment << endl;
    cout << fixed << setprecision(2) << "Total Interest\t $" << total_interest_accumulated << endl << endl;
}

// Check to see if user types in 1 to restart program, else program will terminate
bool check_restart_program_input(string user_input)
{
    if(user_input == "1")
        return true;
    else
        return false;
}

// Take in user input and check for numbers to validate correct data type
void check_user_input_for_numbers_only(string &user_input)
{
    bool check_for_characters = true;
    int count_characters = 0;

    for(int i = 0; i < user_input.length(); i++)
    {
        if(isalpha(user_input[i]))
            count_characters++;
    }

    if(count_characters > 0)
    {
        cout << "Can only compute numbers. Please type in correct data type (Whole number) or (Decimals)" << endl;
        cin >> user_input;
        check_user_input_for_numbers_only(user_input);
    }
}

// Take in user input and check for whole numbers only, relates to term length
void check_user_input_for_whole_numbers_only(string &user_input)
{
    bool check_for_characters = true;
    int check_for_decimal_point = 0;

    for(int i = 0; i < user_input.length(); i++)
    {
        if(user_input[i] == '.')
            check_for_decimal_point++;
    }

    if(check_for_decimal_point > 0)
    {
        cout << "Can only input whole numbers. Please type in correct data type" << endl;
        cin >> user_input;
        check_user_input_for_numbers_only(user_input);
        check_user_input_for_whole_numbers_only(user_input);
    }
}

/*
    Convert user input to appropriate data type for calculations
    Used try catch to catch an error, in the case if converting the input fails, will default to exiting the program instead of crashing.
*/
void convert_input(string loan_amount_string, string annual_percentage_interest_string, string term_length_string, double &loan_amount, double &annual_percentage_interest, int &term_length)
{
    try 
    {
        loan_amount = stod(loan_amount_string); 
    }
    catch(...)
    {
        cout << "Error converting loan_amount_string to double -- NOW EXITING....";
        exit(1);
    }
    try 
    {
        annual_percentage_interest = stod(annual_percentage_interest_string); 
    }
    catch(...)
    {
        cout << "Error converting annual_percentage_interest_string to double -- NOW EXITING....";
        exit(1);
    }
    try 
    {
        term_length = stoi(term_length_string); 
    }
    catch(...)
    {
        cout << "Error converting term_length_string to integer -- NOW EXITING....";
        exit(1);
    }
}