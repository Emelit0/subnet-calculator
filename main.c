#include <stdio.h>
#include <bits/stdint-uintn.h>
#include <stdlib.h>


typedef struct {
    uint8_t octet1;
    uint8_t octet2;
    uint8_t octet3;
    uint8_t octet4;
} ipv4_addr;

typedef struct {
    ipv4_addr network_addr;
    ipv4_addr subnet_mask;
    ipv4_addr broadcast_addr;
    uint32_t num_hosts;
} subnet_info;


void getSubnetInfo(ipv4_addr ip_addr, uint8_t mask_len, subnet_info *subnet) {

    // Calculate subnet mask
    uint32_t subnet_mask = 0;
    for (int i = 0; i < mask_len; i++) {
        subnet_mask |= (1 << (31 - i)); // set the i-th bit of subnet mask to 1
    }
    subnet->subnet_mask.octet1 = (subnet_mask >> 24) & 0xff;
    subnet->subnet_mask.octet2 = (subnet_mask >> 16) & 0xff;
    subnet->subnet_mask.octet3 = (subnet_mask >> 8) & 0xff;
    subnet->subnet_mask.octet4 = subnet_mask & 0xff;

    // Calculate network address
    subnet->network_addr.octet1 = ip_addr.octet1 & subnet->subnet_mask.octet1;
    subnet->network_addr.octet2 = ip_addr.octet2 & subnet->subnet_mask.octet2;
    subnet->network_addr.octet3 = ip_addr.octet3 & subnet->subnet_mask.octet3;
    subnet->network_addr.octet4 = ip_addr.octet4 & subnet->subnet_mask.octet4;

    // Calculate broadcast address
    subnet->broadcast_addr.octet1 = subnet->network_addr.octet1;
    subnet->broadcast_addr.octet2 = subnet->network_addr.octet2;
    subnet->broadcast_addr.octet3 = subnet->network_addr.octet3;
    subnet->broadcast_addr.octet4 = subnet->network_addr.octet4;

    // Calculate number of hosts
    uint32_t num_hosts = (1 << (32 - mask_len)) - 2;
    for (int i = 0; i < 4; i++) {
        subnet->broadcast_addr.octet4 |= (1 << i); // set the i-th bit of broadcast address to 1
        if (subnet->broadcast_addr.octet4 > subnet->network_addr.octet4) {
            subnet->num_hosts = num_hosts;
            break;
        }
        subnet->broadcast_addr.octet4 &= ~(1 << i); // set the i-th bit of broadcast address to 0
    }

    printf("%u", num_hosts);

}

int main() {

    char ip_str[16];
    int mask_len;
    ipv4_addr ip_address;
    subnet_info subnet;


    printf("Enter IP address (in dotted decimal notation e.g. 192.168.0.1): ");
    scanf("%15s", ip_str);
    scanf(ip_str, "%u, %u, %u, %u", &ip_address.octet1, &ip_address.octet2, &ip_address.octet3, &ip_address.octet4);

    printf("Enter subnet mask (in dotted decimal notation): ");
    scanf("%u", &mask_len);

    getSubnetInfo(ip_address, mask_len, &subnet);

    printf( "%i", ip_address, subnet, mask_len, subnet.num_hosts, subnet.network_addr, subnet.subnet_mask, subnet.broadcast_addr);

//    subnet_info subnet;

//    getSubnetInfo(ip_address, mask_len, &subnet);

}



