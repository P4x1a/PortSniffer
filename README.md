# PortSniffer

This is a C script that performs two main tasks:
1. **Subdomain Resolution**: Checks common subdomains of a domain and displays their IP addresses.
2. **Port Scanning**: Checks if common ports (like 80 for HTTP, 443 for HTTPS) are open on a given IP, utilizing threads for improved performance.

---

## Features

- **Subdomain Resolution**:
  - Checks common subdomains like `www`, `mail`, `ftp`, etc.
  - Displays the IP addresses associated with each subdomain.

- **Port Scanning**:
  - Checks common ports (21, 22, 80, 443, etc.) on a specific IP.
  - Uses threads to check multiple ports simultaneously, improving performance.
  - Displays the status of each port (open/closed) and the associated service.

---

## How to Use

### Prerequisites

- C compiler (such as `gcc`).
- `pthread` library (already included in most Unix-like systems).

### Compilation

To compile the script, use the following command:

```bash
gcc portsniffer.cpp -o portsniffer  
```
![image](https://github.com/user-attachments/assets/8cecdd95-62f9-4a5d-b8c9-2383584a2ae3)


This will create an executable called `portsniffer`.

### Execution

#### Argument Mode

You can pass the domain as an argument when running the script:

```bash
./portsniffer example.com
```
![image](https://github.com/user-attachments/assets/30d9b2a0-27b4-41cc-a70d-63dcbedeb528)


The script will resolve common subdomains and ask for the IP to perform the port scan.

#### Interactive Mode

If you run the script without arguments, it will enter interactive mode:

```bash
./portsniffer
```

The script will prompt:
1. The domain for subdomain resolution.
2. The IP for the port scan.
![image](https://github.com/user-attachments/assets/f8bd4bbe-3a9e-4b98-a477-418aa1e35f4f)


### Example Output

```plaintext
Checking subdomain: www.example.com
IP addresses of www.example.com:
  192.168.1.1

Checking subdomain: mail.example.com
IP addresses of mail.example.com:
  192.168.1.2

Enter the IP address for port scanning: 192.168.1.1
Starting port scan on common ports for host example.com (IP: 192.168.1.1)...
Port            Service         Status
----------------------------------------
21               FTP             CLOSED
22               SSH             OPEN
23               Telnet          CLOSED
25               SMTP            CLOSED
53               DNS             OPEN
80               HTTP            OPEN
110              POP3            CLOSED
143              IMAP            CLOSED
443              HTTPS           OPEN
3306             MySQL           CLOSED
3389             RDP             CLOSED
8080             HTTP Alt        CLOSED
8443             HTTPS Alt       CLOSED
```

---

## Code Structure

The script is divided into several functions to organize the code:

1. **`getAddresses`**: Retrieves and displays the IP addresses associated with a host.
2. **`getSubdomains`**: Checks common subdomains of a domain.
3. **`checkPortThread`**: Checks if a port is open (executed in a separate thread).
4. **`scanCommonPorts`**: Performs a scan on common ports using threads.
5. **`main`**: The main function that controls the program flow.

---

## Requirements

- Unix-like operating system (Linux, macOS).
- C compiler (such as `gcc`).
- `pthread` library.

---

## Future Improvements

- Add support for Windows (replacing `pthread` with Windows threading API).
- Implement a limit on simultaneous threads to avoid system overload.
- Add support for custom port scanning (beyond common ports).
- Improve error handling and code robustness.

---

## License

This project is licensed under the [MIT License](LICENSE). Feel free to use, modify, and distribute the code.

---

## Contributions

Contributions are welcome! If you encounter any issues or have suggestions for improvements, please open an **issue** or submit a **pull request**.
