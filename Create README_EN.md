<p align="center">
  <img src="https://i.imgur.com/qtsmsG1.png" alt="Revolution Multiplayer Banner" width="500"/>
</p>

<h1 align="center">Revolution Multiplayer</h1>

<p align="center">
  <strong>An <em>open-source</em> movement to build a modern, modular, and free <em>base gamemode</em> for San Andreas Multiplayer (SA-MP), made for the Indonesian community.</strong>
</p>

<p align="center">
  <img alt="GitHub stars" src="https://img.shields.io/github/stars/revolutionmp/revolution?style=for-the-badge&logo=github">
  <img alt="GitHub forks" src="https://img.shields.io/github/forks/revolutionmp/revolution?style=for-the-badge&logo=github">
  <img alt="Discord" src="https://img.shields.io/discord/364749324833390593?style=for-the-badge&logo=discord&label=Discord">
</p>

---

## 📜 About This Project

**Revolution Multiplayer** was born out of frustration with the SA-MP ecosystem dominated by private, paywalled scripts.  
This project is both an answer and a movement: proof that the community can collaborate to create a high-quality gamemode accessible to everyone, free of charge.

We’re building the gamemode foundation from scratch using a modern technology stack to ensure that this project is easy to maintain, extend, and run by anyone.

---

## ✨ Key Features

* **100% Open-Source:** All of our code is public. You can read it, learn from it, and directly contribute to development.
* **Modern Architecture:** Code is structured modularly (`Core`, `Modules`, `Components`, `...`) to keep it clean, extensible, and easy to understand.
* **Database ORM:** Built with **ORM (Object-Relational Mapping)** for safer, more efficient, and modern database interaction.
* **Docker Support:** Run your development server with a single command. No more complicated manual setup.
* **Dependency Management:** Powered by **sampctl** for automatic PAWN dependency handling.
* **In-Game Voice:** Integrated foundation for in-game voice chat.

---

## 🚀 Getting Started

Spinning up your development server is simple thanks to Docker.

### Prerequisites

Make sure you have the following software installed:
* [Git](https://git-scm.com/downloads)
* [Docker](https://www.docker.com/products/docker-desktop/)
* [Docker Compose](https://docs.docker.com/compose/install/) (usually included with Docker Desktop)

### Installation

1.  **Clone this repository:**
    ```bash
    git clone https://github.com/revolutionmp/revolution.git
    cd revolution
    ```

2.  **Create the environment configuration file:**
    Copy the example `.env.example` into `.env`. This file stores your sensitive configurations.
    ```bash
    cp .env.example .env
    ```

3.  **Edit `.env`:**
    Open `.env` and fill in the required values, especially the database password.

4.  **Run with Docker Compose:**
    This command will build the image, download the database, and launch everything automatically.
    ```bash
    docker compose up -d --build
    ```

Your server should now be running! You can view the logs with:
```bash
docker compose logs -f samp
```

---

## 🤝 Contributing

We welcome contributions from everyone! If you’d like to help, here’s the workflow:

1. **Fork** this repository.
2. Create a new **Branch** for your feature or fix (`git checkout -b feature/YourFeatureName`).
3. Make your changes and **Commit** (`git commit -m 'feat: Add feature X'`).
4. **Push** your branch (`git push origin feature/YourFeatureName`).
5. Open a **Pull Request**.

Got an idea or want to discuss before coding?
Join us on **[Discord](https://discord.gg/ppByTcfZ8j)** and let’s talk!

---

## 📄 License

This project is licensed under the MIT License. See the `LICENSE` file for more details.
