#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

typedef char bool;

size_t my_strlen(char *param_1){
    size_t length = 0;
    if(param_1 != NULL) {
        while(*param_1 != '\0') {
            length++;
            param_1++;
        }
    }
    return length;
}

char* reverse_string(char* str){
    int l, i;
    char *begin_ptr, *end_ptr, ch;

    l = my_strlen(str);
    begin_ptr = str;
    end_ptr = str;
    for (i = 0; i < l - 1; i++)
        end_ptr++;
    for (i = 0; i < l / 2; i++) {
        ch = *end_ptr;
        *end_ptr = *begin_ptr;
        *begin_ptr = ch;
        begin_ptr++;
        end_ptr--;
    }
    return str;
}

char* my_itoa(int num, int base){
    int i = 0;
    bool isNegative = FALSE;

    int digit_counter = 0;  // Getting count of Digits to Malloc appropriately
    int digits_temp = num;
    do{
        digits_temp /= 10;
        digit_counter++;
    } while (digits_temp != 0);
    char* str = malloc(sizeof(char) * digit_counter + 1);

    // Handle 0 explicitly, otherwise empty string is printed for 0
    if (num == 0){
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0){
        isNegative = TRUE;
        num = -num;
    }

    while (num != 0){
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    } 
    
    if (base == 16){    // If Hexadecimal, append hexadecimal prefix
        str[i++] = 'x';
        str[i++] = '0';
    }
    
    if (isNegative){    // If number is negative, append '-'
        str[i++] = '-';
    }
    
    str[i] = '\0';      // Append string terminator
 
    reverse_string(str);    // Reverse the string
    return str;
}

int my_printf(char* format, ...) {
    char character;
    char* string;
    int decimal, octal;
    unsigned int unsigned_decimal, unsigned_hexadecimal, void_pointer;
    char parse;
    int char_counter = 0;

    va_list ap;
    va_start(ap, format);

    parse = *format;

    while (parse != '\0'){
        if (parse == '%'){
            format++;
            parse = *format;

            switch(parse){
                case 's':
                    string = va_arg(ap, char *);
                    write(1, string, my_strlen(string));
                    char_counter += my_strlen(string);
                    break;
                
                case 'c':
                    character = va_arg(ap, int);
                    char* one_char;
                    one_char = &character;
                    write(1, one_char, 1);
                    char_counter++;
                    break;

                case 'd': case 'u': //Signed Decimal + Unsigned Decimal
                    decimal = va_arg(ap, int);                    
                    char* d_str = my_itoa(decimal, 10);
                    write(1, d_str, my_strlen(d_str));
                    char_counter += my_strlen(d_str);
                    break;

                case 'x': //Unsigned Hexadecimal
                    unsigned_hexadecimal = va_arg(ap, unsigned int);
                    char* h_str = my_itoa(unsigned_hexadecimal, 16);
                    write(1, h_str, my_strlen(h_str));
                    char_counter += my_strlen(h_str);
                    break;

                case 'p': //Void Pointer
                    void_pointer = va_arg(ap, unsigned int);
                    char* v_str = my_itoa(void_pointer, 16);
                    write(1, v_str, my_strlen(v_str));                    
                    char_counter += my_strlen(v_str);
                    break;
                    
                case 'o': //Unsigned Octal
                    octal = va_arg(ap, unsigned int);
                    char* o_str = my_itoa(octal, 8);
                    write(1, o_str, my_strlen(o_str));
                    char_counter += my_strlen(o_str);
                    break;
            }
        } else {
            write(1, format, 1);
            char_counter++;
        }
        format++;
        parse = *format;
    }
    va_end(ap);
    return char_counter;
}

void main(){
    char* string = "My name is Mo";
    int ch = 80;
    int d = 123456;
    int u = -8675309;
    int z = 0;
    unsigned int x = 666;
    unsigned int o = 666;
    int i;
    int *p = &i;
    
    my_printf("my_printf: string is %s\n", string);
    my_printf("my_printf: ch is %c\n", ch);
    my_printf("my_printf: decimal string is %d\n", d);
    my_printf("my_printf: unsigned decimal string is %u\n", u);
    my_printf("my_printf: 0 decimal string is %d\n", z);
    my_printf("my_printf: hexadecimal string is %x\n", x);
    my_printf("my_printf: octal string is %o\n", o);    
    my_printf("my_printf: void pointer is %p\n", p);
    my_printf("Multiple Arguments -- string: %s and octal: %o and hex: %x\n", string, o, x);
}

// gcc my_printf.c -o my_printf && ./my_printf