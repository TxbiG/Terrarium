# Phase 8 Networking and Internet Services

Phase 8 brings networking up in layers: NIC registration first, kernel network
contracts second, user-space service endpoints third, and trusted Internet tools
only after TLS/certificate policy exists.

## Current contracts

- NIC drivers register through `kernel/drivers/nics/nic.c`, which creates a
  kernel `netif` through `netif_register` and forwards RX/TX through the kernel
  networking boundary.
- `kernel/net/net.h` defines the Phase 8 kernel network contract:
  interfaces, link state, IP assignment, route table, DNS server slots,
  firewall rules, socket stats, and a packet diagnostic ring.
- `kernel/net/net.c` implements in-memory route, DNS, firewall, socket, and
  diagnostic tables. This is not a full TCP/IP stack yet; it is the stable
  ownership boundary that a stack or lwIP-style port can attach to.
- Network syscall IDs `500..509` are reserved for interface, route, DNS,
  firewall, socket, diagnostics, and ping APIs. They currently return
  `TERRA_STATUS_NOTREADY` until copy-in/copy-out and syscall policy are wired.
- `system/net` now provides wrappers for IP parsing/formatting, interface list,
  routes, DNS, firewall, and ping. Utilities should call these wrappers first
  and fall back to `/proc` or clear backend-missing errors while the kernel
  syscall handlers mature.
- `terrarium-netd` reports saved network profiles from
  `/etc/terrarium/net/interfaces.cfg`.
- `terrarium-dhcpd` now has explicit client/server mode status.
- `terrarium-firewalld` exists as the firewall policy service boundary and is
  included in the init unit manifest.

## lwIP reference boundary

`C:\Users\TobyG\Downloads\lwIP-master` was inspected as a reference for layering:
`api`, `apps`, `core`, `include`, and `netif`. Terrarium should not mix lwIP
source into kernel or service files directly. If lwIP is imported later, keep it
under a vendor/import boundary and write a Terrarium port layer around:

- `netif` link RX/TX
- timers/timeouts
- memory allocation
- packet buffers
- socket or native API adaptation
- DNS/DHCP integration
- errno/status translation

## Still missing

- Real Ethernet/IP/ARP/ICMP/UDP/TCP packet processing.
- DHCP client lease negotiation and DHCP server lease database.
- DNS resolver cache and query transport.
- Route lookup and packet forwarding.
- Firewall matching by transport ports and addresses.
- `/proc/net` and `/run/net` service endpoints backed by live kernel state.
- TLS library/import boundary, certificate store, trust policy, and per-service
  network permissions.
- Secure implementations for `curl`, `wget`, and `ssh`.
- Network profile persistence writes, Wi-Fi credentials, and wireless driver
  support.
