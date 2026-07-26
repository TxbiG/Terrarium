# Contributing to Terrarium OS

Thank you for your interest in contributing to **Terrarium OS**! 🌱  

Even though I’m currently the only developer, contributions from the community are welcome. Whether it’s improving documentation, reporting bugs, adding apps, or helping with kernel or services development, every contribution helps.

This guide explains how you can contribute effectively and safely.

---

## Ways you can help

Terrarium OS is a modular, hybrid-kernel operating system. You can contribute in many ways:

- **Bug reports & fixes** – Found a crash, driver issue, or graphical glitch? Let me know or submit a fix.  
- **New features** – Add kernel modules, user-space services, system utilities, or preinstalled apps.  
- **Documentation improvements** – Tutorials, guides, API references, or clarification of system behavior.  
- **Hardware testing & compatibility** – Test on x86/x64, ARM32/ARM64, RISC-V, or other supported platforms. Report issues.  
- **Performance testing** – Help profile the kernel, system services, or applications for optimization.  

> Even small contributions, like fixing typos, organizing folder structures, or improving boot scripts, are appreciated.

---

## What I don’t accept

To avoid confusion:

- Issues that are purely support questions about running the OS; Visit the Docs for that.
- Changes that break system stability or hardware compatibility.  
- Contributions that do not follow coding, formatting, or safety guidelines.  

---

## Ground rules

- Maintain **cross-platform compatibility** wherever possible.  
- Follow safe coding practices in **kernel development**; unstable changes can crash the system.  
- Keep features modular and incremental — avoid large, sweeping changes in a single PR.  
- Document your changes clearly in code, README, or system docs.  
- Be respectful in communications — even if it’s just me responding!  
- Follow the [Code of Conduct](https://github.com/TxbiG/TerrariumOS/blob/main/CODE_OF_CONDUCT.md).

---

## Your first contribution

If you’re new:

- Improve `Documentation` guides or tutorials.  
- Add small apps in the `apps` folder.  
- Help with user-space services in `services`.  

Tasks suitable for beginners are labeled as `good first issue`. For guidance on using Git/GitHub and submitting pull requests:

- [Make a Pull Request](http://makeapullrequest.com/)  
- [First Timers Only](http://www.firsttimersonly.com/)  

> If you want to contribute kernel or driver code, please make sure you understand the modular architecture and safety implications.  

---

## Reporting a bug

Include the following:

1. Terrarium OS version  
2. Platform & architecture (x86/x64, ARM, RISC-V)  
3. Hardware details (CPU, GPU, memory, storage, firmware)  
4. Steps to reproduce  
5. Expected behavior  
6. Actual behavior  

> For **security issues**, do **not** open a public issue. Email `security@tbiG.com` instead.

---

## Suggesting a feature

- Open an issue describing **why** the feature is needed and **how** it might work.  
- Check if similar features already exist.  
- Keep suggestions aligned with Terrarium OS’s philosophy: **modular, hybrid kernel, cross-platform, lightweight, and user-friendly**.  

> Since I’m the sole maintainer, feature development may take time, but all suggestions are welcome!

---

## Pull request review process

- PRs will be reviewed by me (the maintainer).  
- Feedback may include requests for clarification, additional testing, or documentation.  
- Expect responses within 1–2 weeks.  
- Major contributions should include tests, documentation, and examples where possible.  

---

## Community & support

- Discord / Forum: [placeholder link]  
- GitHub Discussions: [placeholder link]  

> I welcome discussion, testing feedback, and suggestions — even if you don’t submit code.

---

## Code style & commit guidelines

- Follow **C++17** (kernel and user-space code) or relevant language guidelines for apps/services.  
- Use **modular, safe, and well-documented coding practices**.  
- Commit messages should be descriptive: `kernel: fix memory leak in scheduler` or `apps: add simple calculator app`.  
- Label issues and PRs clearly: `bug`, `enhancement`, `documentation`, `performance`, `hardware`, `question`.
