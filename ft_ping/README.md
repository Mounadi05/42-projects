# ft_ping

A minimal ICMP echo utility written from scratch. It mirrors the classic `ping` workflow: resolve the destination, send raw ICMP echo requests, and track round-trip statistics until interrupted with `Ctrl+C`.

## Usage

```
sudo ./ft_ping <destination> [-v]
```

- `destination` can be a hostname or an IPv4 address.
- `-v` prints socket setup details.
- Use `Ctrl+C` to stop and display the summary.

## Notes

- Requires raw-socket permissions; the build step sets the `cap_net_raw` capability automatically (see `Makefile`).
- Output format and behavior align with the built-in `ping`.