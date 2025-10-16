# BGP Lab Collection

Hands-on GNS3 labs that walk through building Border Gateway Protocol (BGP) fabrics with
FRRouting containers and lightweight host nodes. Each folder under `Bgp/` is a
self-contained scenario you can open directly in GNS3 to explore a different
aspect of BGP and VXLAN EVPN.

## Prerequisites

- **GNS3 2.2+** with the GNS3 VM or a local Docker-enabled server.
- **Docker Engine** (the projects rely on Docker containers for FRRouting and
  BusyBox nodes).
- **FRRouting Docker image** (`frrouting/frr`) available locally.
- **BusyBox Docker image** (pulled automatically on first use).


## Folder layout

| Scenario | Project file | What it covers |
|----------|--------------|----------------|
| `P1/`    | `p1.gns3project` | Base FRRouting router image, quick sanity test with a BusyBox host. |
| `P2/`    | `p2.gns3project` | Two-leaf VXLAN fabric comparing multicast vs. static VTEP signalling for BGP EVPN. |
| `P3/`    | `l3.gns3project` | Three-leaf EVPN fabric with a route-reflector spine running iBGP over OSPF underlay. |

Each project directory also contains per-node startup files (scripts or
Dockerfiles). GNS3 mounts these into the respective nodes so they boot with the
predefined networking configuration.

## Scenario overviews

### P1 – FRRouting primer

- `_amounadi-2`: Dockerfile that enables the `bgpd`, `ospfd`, and `isisd`
  daemons inside the FRR container.
- `_amounadi-1_host`: Simple BusyBox container acting as an endpoint.


### P2 – VXLAN over BGP EVPN (multicast vs. static VTEP)

This topology contains two leaves on each side, with duplicate startup scripts to
highlight different VXLAN discovery techniques:

- `_amounadi-1_g` / `_Ytaya-2_g`: Build VXLAN interfaces that rely on a
  multicast group (`239.1.1.1`) for flood-and-learn behaviour.
- `_amounadi-1_s` / `_Ytaya-2_s`: Configure VXLAN with explicit `local` and
  `remote` VTEP addresses (head-end replication).


### P3 – Spine-and-leaf EVPN fabric

A full EVPN lab with a route-reflector spine and three leaf switches:

| Node          | Role            | Key config snippets |
|---------------|-----------------|---------------------|
| `_ytaya-1`    | Spine / RR      | iBGP peer-group `ibgp`, OSPF area 0, route-reflector for EVPN AF. |
| `_amounadi-2` | Leaf 1          | VXLAN VNI 10 bridge, OSPF on `eth0`, EVPN address-family advertising all VNIs. |
| `_amounadi-3` | Leaf 2          | Same as Leaf 1 with different /30 underlay subnet. |
| `_ytaya-4`    | Leaf 3          | Additional leaf to experiment with scaling neighbours. |
| `_ytaya-*_host` | End hosts    | Lightweight BusyBox containers you can place on the VLAN bridge. |

Underlay IP ranges

- Leaf1 ↔ Spine: `10.1.1.0/30`
- Leaf2 ↔ Spine: `10.1.1.4/30`
- Leaf3 ↔ Spine: `10.1.1.8/30`

Overlay VNI 10 is carried across all leaves using VXLAN interfaces bridged with
`eth1` 

