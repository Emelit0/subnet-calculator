#include <stdio.h>

void header() {
    printf("IP CALCULATOR");
    printf("Input a IP Address and the Subnet Mask!");
    printf("Calculate the possible amount of hosts and subnets of the IP given");
    printf("can also output the given CIDR mask into decimal");
}

int main() {
    int IP1, IP2, IP3, IP4, CIDR;
    int bcIP1, bcIP2, bcIP3, bcIP4;
    int IPgiven = 0;
    int CIDRgiven = 0;
    double aHosts;

    header();

    while (IPgiven == 0) {
        printf("/n Please input a IP-address in the right format XXX.XXX.XXX.XXX: ");
        scanf("%i.%i.%i.%i.%i", &IP1, &IP2, &IP3, &IP4);

        if (IP1 > 255 || IP2 > 255 || IP3 > 255 || IP4 > 255 || IP1 == 127 || IP1 == 255 || IP1 < 1 || IP2 < 0 || IP3 < 0 || IP4 < 0) {
            printf("Please validate your input and check for the correct format");

        } else {
            IPgiven = 1;
        }

    }
}

