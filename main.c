#include <stdio.h>
#include <bits/stdint-uintn.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


typedef struct {
    uint8_t octet1;
    uint8_t octet2;
    uint8_t octet3;
    uint8_t octet4;
} ipv4_addr;

typedef struct {
    ipv4_addr network_addr;
    ipv4_addr subnet_mask;
    ipv4_addr firstClient;
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

    // Calculate first client address
    subnet->firstClient.octet1 = subnet->network_addr.octet1;
    subnet->firstClient.octet2 = subnet->network_addr.octet2;
    subnet->firstClient.octet3 = subnet->network_addr.octet3;
    subnet->firstClient.octet4 = subnet->network_addr.octet4;

    // Calculate number of hosts
    uint32_t num_hosts = (1 << (32 - mask_len)) - 2;
    for (int i = 0; i < 4; i++) {
        subnet->firstClient.octet4 |= (1 << i); // set the i-th bit of broadcast address to 1
        if (subnet->firstClient.octet4 > subnet->network_addr.octet4) {
            subnet->num_hosts = num_hosts;
            break;
        }
        subnet->firstClient.octet4 &= ~(1 << i); // set the i-th bit of broadcast address to 0
    }
}

ipv4_addr ip_broadcast_address(ipv4_addr ip_address, ipv4_addr subnet_mask) {
    ipv4_addr broadcast_address;
    broadcast_address.octet1 = ip_address.octet1 | ~subnet_mask.octet1;
    broadcast_address.octet2 = ip_address.octet2 | ~subnet_mask.octet2;
    broadcast_address.octet3 = ip_address.octet3 | ~subnet_mask.octet3;
    broadcast_address.octet4 = ip_address.octet4 | ~subnet_mask.octet4;
    return broadcast_address;
}

int main() {

    char ip_str[16];
    int mask_len;
    ipv4_addr ip_address;
    subnet_info subnet;


    printf("Enter IP address (in dotted decimal notation e.g. 192.168.0.1): \n");
    fgets(ip_str, sizeof(ip_str), stdin);
    ip_str[strcspn(ip_str, "\n")] = '\0'; // Remove trailing newline from input
    if (sscanf(ip_str, "%hhu.%hhu.%hhu.%hhu", &ip_address.octet1, &ip_address.octet2, &ip_address.octet3, &ip_address.octet4) != 4) {
        printf("Invalid address format\n");
        exit(EXIT_FAILURE);
    }
    printf("Enter subnet mask (in CIDR notation):");
    scanf("%u", &mask_len);
    printf("You entered: %hhu.%hhu.%hhu.%hhu ", ip_address.octet1, ip_address.octet2, ip_address.octet3, ip_address.octet4);
    printf(" /%i\n", mask_len);



// calculate network info
    getSubnetInfo(ip_address, mask_len, &subnet);
    ipv4_addr broadcast_address = ip_broadcast_address(ip_address, subnet.subnet_mask);

//output network information
    printf("Network address: %u.%u.%u.%u\n", subnet.network_addr.octet1, subnet.network_addr.octet2,
           subnet.network_addr.octet3, subnet.network_addr.octet4);
    printf("Subnet mask: %u.%u.%u.%u\n", subnet.subnet_mask.octet1, subnet.subnet_mask.octet2,
             subnet.subnet_mask.octet3, subnet.subnet_mask.octet4);
    printf("First client IP-Address: %u.%u.%u.%u\n", subnet.firstClient.octet1, subnet.firstClient.octet2,
             subnet.firstClient.octet3, subnet.firstClient.octet4);
    printf("Number of hosts: %u\n", subnet.num_hosts);

    printf("Broadcast: %hhu.%hhu.%hhu.%hhu", broadcast_address.octet1, broadcast_address.octet2, broadcast_address.octet3, broadcast_address.octet4);
}



