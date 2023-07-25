#include <string.h>

unsigned short lfsr = 0xACE1u;

unsigned rand(int max)
{
    unsigned bit;
    bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    return lfsr =  ((lfsr >> 1) | (bit << 15)) % max;
}

unsigned rand2()
{
    unsigned bit;
    bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    return lfsr =  ((lfsr >> 1) | (bit << 15));
}

void randstring(int length, char* buf) {

    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";        

    for (int n = 0;n < length;n++) {            
        int key = rand2() % (int)(sizeof(charset) -1);
        buf[n] = charset[key];
    }
    buf[length] = '\0';
    return;
}

int main(){
    char buf[64];
    memset(buf,0,64);
    randstring(32,buf);
}