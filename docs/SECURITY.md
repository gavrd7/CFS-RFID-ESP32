# Security and public release checklist

- The public starter replaces personal Wi-Fi/AP defaults, local printer IP and the printer-password literal with placeholders. **It has not been hardware validated with those placeholder values.**
- **AP starts enabled by default in the public starter**, unlike the owner’s configured build, to allow initial provisioning. Change the sample AP password before use or deployment. It is not a production secret.
- Set `PRINTER_PASS` locally before attempting printer SSH access; the printer-user and remote database paths remain untouched. Do not commit your configured password.
- Changing default literals does not modify existing LittleFS `config.ini`. A board already configured will use stored settings where applicable.
- Rotate any previously exposed real credentials. Treat printer SSH access and the web UI as trusted-LAN-only; review authentication and input validation before exposing publicly or to untrusted networks.
- Printer sync/push are explicit browser actions. Keep a validated printer database backup, and do not change the known-good SCP/SSH logic casually.
- No secret scanner guarantees that all historical text is safe. Re-run `git grep` and a secret-scanning tool (e.g. gitleaks) over the complete repository history prior to making it public.
- Review any custom material files and documentation separately before publication.
