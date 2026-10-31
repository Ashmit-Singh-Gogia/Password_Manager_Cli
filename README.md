# pm — Command-Line Password Manager in C

A minimalist, high-performance, and self-contained command-line password manager written in C11. It provides local credential storage with a master-password authentication gate, zero external dependencies, and strict memory safety.

---

## 1. Overview & Motivation

### The Problem
Managing multiple credentials across different services often leads users to dangerous habits:
- Reusing the same password across multiple platforms.
- Storing credentials in unorganized text files, notes apps, or spreadsheets.
- Relying on bloated browser extensions or heavy cloud-based password managers for simple terminal workflows.

### The Solution
- **Fast & Minimal:** A lightweight CLI with zero external dependencies and instant terminal access.
- **Safe & Structured:** Protected by a master password gate with clean, leak-free C11 memory management.

---

## 2. System Architecture & Execution Flow

The project follows a **strict 4-layer modular architecture**, enforcing complete separation of concerns:

```
                  ┌────────────────────────┐
                  │    User Terminal       │
                  └───────────┬────────────┘
                              │ CLI Command (add, get, list, delete)
                              ▼
                  ┌────────────────────────┐
                  │  CLI Layer (cli.c)     │ ◄── Handles flags, I/O & input parsing
                  └─────┬────────────┬─────┘
                        │            │
            Master Auth │            │ CRUD Operations
                        ▼            ▼
         ┌─────────────────┐      ┌────────────────────────┐
         │ Auth (auth.c)   │      │ Vault Engine (vault.c) │ ◄── In-Memory Dynamic Array
         └────────┬────────┘      └───────────┬────────────┘
                  │                           │
                  │ Reads ~/.pm_master        │ Synchronizes state
                  ▼                           ▼
         ┌─────────────────────────────────────────────────┐
         │ Storage Layer (storage.c)                       │ ◄── TSV Serializer / Deserializer
         └────────────────────────┬────────────────────────┘
                                  │
                                  ▼
                  ┌────────────────────────┐
                  │ Disk: ~/.pm_vault.txt  │
                  └────────────────────────┘
```

### Execution Flow of a Command (e.g., `pm add`)
1. **Entry & Routing (`main.c` → `cli.c`):**
   - The program parses arguments via `cli_run(argc, argv)`.
2. **Master Authentication Gate (`auth.c`):**
   - On the first run, `auth_setup()` prompts the user to initialize and confirm a master password saved to `~/.pm_master`.
   - On subsequent runs, `auth_check()` verifies the entered password against the stored master credential before granting access.
3. **Storage Loading (`storage.c`):**
   - The CLI resolves the absolute path `~/.pm_vault.txt` via `storage_get_path()`.
   - `storage_load()` reads the persistent file and parses entries line by line into an in-memory `vault` struct.
4. **In-Memory Operation (`vault.c`):**
   - `vault_add()` verifies field length limits, checks for duplicates, rejects forbidden delimiters (`\t`, `\n`), and dynamically resizes memory capacity if needed.
5. **Persistence & Clean Teardown (`storage.c` → `vault.c`):**
   - If modified, `storage_save()` writes the updated vault back to disk safely.
   - `vault_free()` releases all dynamically allocated heap memory before exiting cleanly.

---

## 3. Data Model & Design Decisions

### Memory Model (`src/vault/vault.h`)

```c
#define SERVICE_MAX_LEN   64
#define USERNAME_MAX_LEN  64
#define PASSWORD_MAX_LEN 128

typedef struct entry {
    char service[SERVICE_MAX_LEN];
    char username[USERNAME_MAX_LEN];
    char password[PASSWORD_MAX_LEN];
} entry;

typedef struct vault {
    entry *items;        // Dynamic array on heap
    size_t count;        // Active entries stored
    size_t capacity;     // Total allocated slots
} vault;
```

#### Key Technical Decisions:
- **Dynamic Memory Management:** Uses fixed-size entry buffers with an array that dynamically doubles (`0 → 8 → 16...`) via safe `realloc`.
- **Corruption-Proof TSV Storage:** Persists as tab-separated values while strictly disallowing tabs and newlines in input fields.

---

## 4. Build & Installation

### Prerequisites
- Clang or GCC (C11 standard)
- GNU Make

### Compilation
The build system utilizes `-std=c11`, strict warnings (`-Wall -Wextra -Wpedantic`), and treats warnings as errors (`-Werror`). Dependency generation (`-MMD -MP`) tracks header changes automatically.

```bash
# Clone the repository
git clone https://github.com/your-username/Password_Manager_Cli.git
cd Password_Manager_Cli

# Build the binary
make
```

## 5. Usage & Command Reference

| Command | Description | Example |
| :--- | :--- | :--- |
| `pm help` | Display available commands and CLI usage. | `pm help` |
| `pm list` | List all saved service names in the vault. | `pm list` |
| `pm add` | Interactively prompt for service, username, and password. | `pm add` |
| `pm get <service>` | Display the username; keeps password masked by default. | `pm get github` |
| `pm get <service> --show` | Display the username and reveal the plain password. | `pm get github --show` |
| `pm delete <service>` | Permanently remove a service from the vault. | `pm delete github` |

### Interactive Walkthrough

#### 1. Adding a Credential
```bash
$ pm add
Service:  github
Username: developer
Password: SuperSecretPassword123!
Saved 'github'.
```

#### 2. Viewing Saved Services
```bash
$ pm list
  github
```

#### 3. Retrieving a Credential
```bash
$ pm get github
Service:  github
Username: developer
Password: (hidden — use 'pm get github --show' to reveal)

$ pm get github --show
Service:  github
Username: developer
Password: SuperSecretPassword123!
```

#### 4. Deleting a Service
```bash
$ pm delete github
Deleted 'github'.
```

