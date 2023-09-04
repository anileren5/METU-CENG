#include <stdio.h>

float try_btc = 0.0555;
float btc_try = 18.0180;
float try_eth = 0.0833;
float eth_try = 12.0048;
float btc_eth = 1.5001;
float eth_btc = 0.6666;

int main(void)
{
    char currency_code,op_code;
    float balance_T,balance_B,balance_E;
    balance_T = balance_B = balance_E = 0;
    scanf("%c %f",&currency_code,&balance_T);
    if (currency_code != 'T') printf("Error: Operation could not be completed.\n");
    
    while (1)
    {
        scanf("%c",&op_code);
        
        if (op_code == 'Q') {
            break;
        }
        
        else if (op_code == 'R'){
            if (balance_B == 0 && balance_E != 0) printf("Our account holds %.2f TRY | %.2f ETH.\n",balance_T,balance_E);
            if (balance_B != 0 && balance_E == 0) printf("Our account holds %.2f TRY | %.2f BTC.\n",balance_T,balance_B);
            if (balance_B == 0 && balance_E == 0) printf("Our account holds %.2f TRY.\n",balance_T);
            if (balance_B != 0 && balance_E != 0) printf("Our account holds %.2f TRY | %.2f BTC | %.2f ETH.\n",balance_T,balance_B,balance_E);
        }
        
        else if (op_code == 'D'){
            char currency_code_D;
            float amount;
            
            scanf(" %c %f",&currency_code_D,&amount);
            if (currency_code_D == 'T'){
                balance_T += amount;
            }
            
            else if (currency_code_D == 'B'){
                balance_B += amount;
            }
            
            else if (currency_code_D == 'E'){
                balance_E += amount;
            }
        
            else {
                printf("Operation could not be completed.\n");
            }            
        }
        
        else if (op_code == 'W'){
            char currency_code_W;
            float amount;
            
            scanf(" %c %f",&currency_code_W,&amount);
            
            if (currency_code_W == 'T'){
                if (amount <= balance_T){
                    balance_T -= amount;
                }
                else {
                    printf("Error: Insufficient funds.\n");
                }
            }
            
            
            else if (currency_code_W == 'B') {
                if (amount <= balance_B){
                    balance_B -= amount;
                }
                else {
                    printf("Error: Insufficient funds.\n");
                }
                
            }
            
            else if (currency_code_W == 'E') {
                if (amount <= balance_E){
                    balance_E -= amount;
                }
                else {
                    printf("Error: Insufficient funds.\n");
                }
            }
            
            else {
                printf("Operation could not be completed.\n");
            }
        }
        
        else if (op_code == 'X'){
            char selling_currency, buying_currency;
            float amount;
            scanf(" %c %f %c",&selling_currency,&amount,&buying_currency);
            if (selling_currency == 'T' &&  buying_currency == 'B'){
                if (balance_T>=amount){
                    balance_T -= amount;
                    balance_B += try_btc * amount;
                    printf("You have successfully bought %.2f BTC.\n",try_btc*amount);
                }
                else {
                    float x;
                    x = (int)(try_btc * balance_T);
                    balance_T -= (btc_try) * x;
                    balance_B += x;
                    printf("This amount is too much, you have successfully bought %.2f BTC.\n",x);
                }
            }
            else if (selling_currency == 'T' && buying_currency == 'E'){
                if (balance_T>=amount){
                    balance_T -= amount;
                    balance_E += try_eth * amount;
                    printf("You have successfully bought %.2f ETH.\n",try_eth*amount);
                }
                else {
                    float x;
                    x = (int )(try_eth * balance_T);
                    balance_T -= (eth_try) * x;
                    balance_E += x;
                    printf("This amount is too much, you have successfully bought %.2f ETH.\n",x);
                }
            }
            else if (selling_currency == 'B' && buying_currency == 'T'){
                if (balance_B>=amount){
                    balance_B -= amount;
                    balance_T += btc_try * amount;
                    printf("You have successfully bought %.2f TRY.\n",btc_try*amount);
                }
                else {
                    float x;
                    x = (int)(btc_try * balance_B);
                    balance_B -= (try_btc) * x;
                    balance_T += x;
                    printf("This amount is too much, you have successfully bought %.2f TRY.\n",x);
                }
            }
            else if (selling_currency == 'B' && buying_currency == 'E'){
                if (balance_B>=amount){
                    balance_B -= amount;
                    balance_E += btc_eth * amount;
                    printf("You have successfully bought %.2f ETH.\n",btc_eth*amount);
                }
                else {
                    float x;
                    x = (int)(btc_eth * balance_B);
                    balance_B -= (eth_btc) * x;
                    balance_E += x;
                    printf("This amount is too much, you have successfully bought %.2f ETH.\n",x);
                }
            }
            else if (selling_currency == 'E' && buying_currency == 'T'){
                if (balance_E>=amount){
                    balance_E -= amount;
                    balance_T += eth_try * amount;
                    printf("You have successfully bought %.2f TRY.\n",eth_try*amount);
                }
                else {
                    float x;
                    x = (int)(eth_try * balance_E);
                    balance_E -= (try_eth) * x;
                    balance_T += x;
                    printf("This amount is too much, you have successfully bought %.2f TRY.\n",x);
                }
            }
            else if (selling_currency == 'E' && buying_currency == 'B'){
                if (balance_E>=amount){
                    balance_E -= amount;
                    balance_B += eth_btc * amount;
                    printf("You have successfully bought %.2f BTC.\n",eth_btc*amount);
                }
                else {
                    float x;
                    x = (int)(eth_btc * balance_E);
                    balance_E -= (btc_eth) * x;
                    balance_B += x;
                    printf("This amount is too much, you have successfully bought %.2f BTC.\n",x);
                }
            }
            else printf("Error: Operation could not be completed.\n");
        }     
        
        
        
        else if (op_code == 'S'){
            float amount;
            char currency_code1, currency_code2;
            float loss_percentage1,loss_percentage2;
            float expected_gain1,expected_gain2;
            float gain_1,gain_2;
            
            scanf(" %f",&amount);
            scanf(" %c %f %f",&currency_code1,&loss_percentage1,&expected_gain1);
            scanf(" %c %f %f",&currency_code2,&loss_percentage2,&expected_gain2);
            
            if (currency_code1 != 'E' && currency_code2 != 'E' && currency_code1 != 'B' && currency_code2 != 'B'){
                printf("Error: Operation could not be completed.\n");
                continue;
            }
            
            gain_1 = (((amount - amount * (loss_percentage1/100))* (1+(expected_gain1/100))) * (1 - (loss_percentage1/100))) - amount;
            gain_2 = (((amount - amount * (loss_percentage2/100))* (1+(expected_gain2/100))) * (1 - (loss_percentage2/100))) - amount;
            

            if (gain_1<0 && gain_2<0) printf("Trading currently is not logical.\n");
            else if (gain_1>gain_2){
                if (currency_code1 == 'E'){
                    balance_T -= amount;
                    balance_E += try_eth * amount;
                    printf("ETH is better with %.2f TRY gain. Buying it...\n",gain_1);
                }
                else if (currency_code1 == 'B'){
                    balance_T -= amount;
                    balance_B += try_btc * amount;
                    printf("BTC is better with %.2f TRY gain. Buying it...\n",gain_1);
                }
            }
            else if (gain_2>gain_1){
                if (currency_code2 == 'E'){
                    balance_T -= amount;
                    balance_E += try_eth * amount;
                    printf("ETH is better with %.2lf TRY gain. Buying it...\n",gain_2);
                }
                else if (currency_code2 == 'B'){
                    balance_T -= amount;
                    balance_B += try_btc * amount;
                    printf("BTC is better with %.2lf TRY gain. Buying it...\n",gain_2);
                }
            }
        }
    }
    printf("Bye...\n");
    return 0;
}
