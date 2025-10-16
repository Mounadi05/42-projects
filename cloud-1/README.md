# Cloud-1 – Automated WordPress on AWS

Infrastructure-as-code project that provisions an EC2 host with Ansible and
installs a production-style WordPress stack (Nginx + PHP-FPM + MariaDB) via
Docker Compose. The playbooks spin up networking primitives, copy the
application bundle, and launch the containers with zero manual steps.

## High-level flow

1. **`setup.yaml`** runs locally to create (or reuse) a security group and a
   single EC2 instance in `us-east-1`.
2. Instance facts are written to `files/inception/.env`, which doubles as the
   configuration file consumed by the containers.
3. The playbook pauses briefly for the instance to boot and then imports
   **`inception-playbook.yaml`**, which connects over SSH and installs Docker, the
   compose bundle, and supporting directories.
4. Docker Compose builds three custom images (MariaDB, WordPress, Nginx) using
   the content in `files/inception/requirements/` and starts them with
   persistent bind-mounted volumes under `/home/admin/data`.
5. Running `clean.yaml` tears down the EC2 instance and removes runtime entries
   from `.env`.

## Prerequisites

- An AWS account with permissions to manage EC2 instances, security groups, and
  key pairs in `us-east-1`.
- Local tooling:
  - Ansible 2.14+ with the `amazon.aws` and `community.aws` collections
  - Python packages `boto3` and `botocore`
  - GNU Make (optional, for the provided helper targets)
- AWS credentials exported in your shell (e.g. `AWS_ACCESS_KEY_ID`,
  `AWS_SECRET_ACCESS_KEY`, and optionally `AWS_SESSION_TOKEN`).
- SSH private key `ansible.pem` corresponding to the AWS EC2 key pair named
  `ansible`. Download it from AWS when you create the key pair, copy it into the
  project root, and protect it with `chmod 600 cloud-1/ansible.pem`.

### Provide the SSH key

1. Create (or download) the AWS EC2 key pair named `ansible` from the AWS
  console and save the resulting `.pem` file on your local machine. AWS only
  provides this file once, so store it securely.
2. Copy it into this directory so Ansible can reference it:

  ```bash
  cp /path/to/your/ansible.pem /cloud-1/ansible.pem
  chmod 400 /cloud-1/ansible.pem
  ```

3. If you choose to keep the file in a different location, update
  `setup.yaml`'s `ansible_ssh_private_key_file` path accordingly.

### Install the required Ansible collections

```bash
ansible-galaxy collection install amazon.aws community.aws
```

### Python dependencies

```bash
pip install --user boto3 botocore
```

## Quick start

```bash
make setup

make clean
```

Both targets simply wrap `ansible-playbook setup.yaml` and `ansible-playbook
clean.yaml` with a bit of helpful logging. You can run the playbooks directly if
you prefer.

### What setup does

- Creates a security group `cloud_security_group` with HTTPS open to the world
  and SSH limited to the IP `197.230.240.146/32` (adjust in `setup.yaml` if
  needed).
- Launches a `t2.micro` EC2 instance from `ami-064519b8c76274859` with the
  `ansible` key pair.
- Adds the instance to a transient inventory group `cloud_instance` so the
  follow-up playbook can target it.
- Writes the new public DNS/IP to `files/inception/.env` for later reference.
- Waits 80 seconds to ensure cloud-init finishes, then runs the remote tasks.

### What the remote playbook configures

On the EC2 host the following steps happen:

- Updates APT cache and installs Docker Engine plus Docker Compose packages.
- Creates persistent directories under `/home/admin/data` for MariaDB and
  WordPress content.
- Copies `files/inception/` to `/home/admin/inception/`, converts the `.env`
  file to `KEY=VALUE` format, and executes `docker-compose up -d`.

The compose file builds custom images:

| Service   | Highlights |
|-----------|------------|
| `mariadb` | Debian base, enables remote access, initialises database and user via `tools/script.sh`. |
| `wordpress` | Debian with PHP-FPM 7.3, downloads WordPress 6.2.2, configures credentials using WP-CLI, exposes FastCGI on port 9000. |
| `nginx`   | Terminates TLS with a self-signed certificate, proxies PHP requests to `co_wordpress:9000`, serves static assets from the shared volume. |

All services share the `wb` volume for WordPress files and the `db` volume for
MariaDB data, both bind-mounted to the `/home/admin/data` folders created
earlier.

## Configuration

Customise runtime settings in `files/inception/.env` before running the setup.
Key parameters include database credentials (`DB_USER`, `DB_USER_PASSWORD`), the
WordPress site details (`title`, `wordpress_admin`, etc.), and the initial
secondary author account (`WP_user`). The setup playbook rewrites the `host` and
`url` entries with live values after provisioning.

If you modify ports or security rules, ensure the security group definition in
`setup.yaml` and the Nginx configuration under
`files/inception/requirements/nginx/conf` stay in sync.

## Project structure

```
cloud-1/
├── Makefile                 
├── ansible.cfg               
├── ansible.pem             
├── setup.yaml               
├── clean.yaml               
├── inception-playbook.yaml   
└── files/
    └── inception/
        ├── .env              
        ├── docker-compose.yml
        └── requirements/
            ├── mariadb/
            │   ├── Dockerfile
            │   └── tools/script.sh
            ├── nginx/
            │   ├── Dockerfile
            │   └── conf
            └── wordpress/
                ├── Dockerfile
                └── tools/
                    ├── script.sh
                    └── www.conf
```
