<p align="center">
  <img src="https://i.imgur.com/qtsmsG1.png" alt="Revolution Multiplayer Banner" width="500"/>
</p>

<h1 align="center">Revolution Multiplayer</h1>

<p align="center">
  <strong>Sebuah gerakan <em>open-source</em> untuk membangun <em>base gamemode</em> San Andreas Multiplayer (SA-MP) yang modern, modular, dan gratis untuk komunitas Indonesia.</strong>
</p>

<p align="center">
  <img alt="GitHub stars" src="https://img.shields.io/github/stars/revolutionmp/revolution?style=for-the-badge&logo=github">
  <img alt="GitHub forks" src="https://img.shields.io/github/forks/revolutionmp/revolution?style=for-the-badge&logo=github">
  <img alt="Discord" src="https://img.shields.io/discord/364749324833390593?style=for-the-badge&logo=discord&label=Discord">
</p>

---

## 📜 Tentang Proyek Ini

**Revolution Multiplayer** lahir dari keresahan terhadap ekosistem SA-MP yang didominasi oleh skrip privat dan berbayar. Proyek ini adalah sebuah jawaban dan perlawanan sebuah gerakan untuk membuktikan bahwa komunitas bisa berkolaborasi untuk menciptakan *gamemode* berkualitas tinggi yang bisa diakses oleh semua orang, tanpa biaya.

Kami membangun fondasi *gamemode* dari nol dengan menggunakan tumpukan teknologi modern untuk memastikan proyek ini mudah dikelola, dikembangkan, dan dijalankan oleh siapa saja.

---

## ✨ Fitur Utama

* **100% Open-Source:** Semua kode kami terbuka. Anda bisa melihat, belajar, dan ikut berkontribusi langsung dalam pengembangan.
* **Arsitektur Modern:** Kode disusun secara modular (`Core`, `Modules`, `Components`, `...`) agar mudah dipahami dan diperluas.
* **Database ORM:** Menggunakan pendekatan **ORM (Object-Relational Mapping)** untuk interaksi database yang lebih aman, modern, dan efisien.
* **Didukung Docker:** Menjalankan server untuk development menjadi sangat mudah, hanya dengan satu perintah. Tidak perlu lagi instalasi manual yang rumit.
* **Manajemen Dependensi:** Menggunakan **sampctl** untuk mengelola semua dependensi PAWN secara otomatis.
* **Voice In-Game:** Fondasi untuk fitur *voice chat* di dalam game sudah terintegrasi.

---

## 🚀 Memulai (Getting Started)

Memulai server development Anda sangatlah mudah berkat Docker.

### Prasyarat

Pastikan Anda sudah menginstal perangkat lunak berikut:
* [Git](https://git-scm.com/downloads)
* [Docker](https://www.docker.com/products/docker-desktop/)
* [Docker Compose](https://docs.docker.com/compose/install/) (biasanya sudah termasuk dalam Docker Desktop)

### Instalasi

1.  **Clone repositori ini:**
    ```bash
    git clone https://github.com/revolutionmp/revolution.git
    cd revolution
    ```

2.  **Buat file konfigurasi environment:**
    Salin file contoh `.env.example` menjadi `.env`. File ini akan menyimpan semua konfigurasi rahasia Anda.
    ```bash
    cp .env.example .env
    ```

3.  **Sesuaikan file `.env`:**
    Buka file `.env` dan isi semua nilai yang kosong, terutama password untuk database.

4.  **Jalankan dengan Docker Compose:**
    Perintah ini akan membangun *image*, mengunduh database, dan menjalankan semuanya secara otomatis.
    ```bash
    docker compose up -d --build
    ```

Server Anda sekarang seharusnya sudah berjalan! Anda bisa melihat lognya dengan perintah `docker compose logs -f samp`.

---

## 🤝 Cara Berkontribusi

Kami sangat terbuka untuk kontribusi dari siapa saja! Jika Anda ingin membantu, berikut adalah alur kerjanya:

1.  **Fork** repositori ini.
2.  Buat **Branch** baru untuk fitur atau perbaikan Anda (`git checkout -b fitur/NamaFitur`).
3.  Lakukan perubahan dan **Commit** pekerjaan Anda (`git commit -m 'feat: Menambahkan fitur X'`).
4.  **Push** ke branch Anda (`git push origin fitur/NamaFitur`).
5.  Buka sebuah **Pull Request**.

Jika Anda punya ide atau ingin berdiskusi sebelum mulai mengerjakan sesuatu, jangan ragu untuk bergabung dengan server **[Discord](https://discord.gg/ppByTcfZ8j)** kami!

---

## 📄 Lisensi

Proyek ini dilisensikan di bawah Lisensi GNU 3.0. Lihat file `LICENSE` untuk detail lebih lanjut.
