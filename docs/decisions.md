# Technical decisions

Initial record: November 18, 2026. A proposal is not a verified implementation.

## 001 — Arduino IDE 2 and the ESP32 core

**Status: selected.** We will use C++ with the environment taught in class. Espressif's core provides microcontroller support. We will record the versions actually used and start with built-in functions before adding libraries.

## 002 — Dashboard using familiar technologies

**Status: selected.** HTML, CSS, and JavaScript cover the planned indicators, web requests, and alerts. A charting library will be selected and justified in Stage 5 if useful. Dashboard hosting will also be decided then.

## 003 — HTTPS and managed storage

**Status: proposed for Stage 4.** The initial simple option is **ThingSpeak**: it stores measurements in channels and provides an API for writing and retrieving data. This lets us focus on the complete data flow. [Official documentation](https://www.mathworks.com/help/thingspeak/users.html).

ThingSpeak stores data, but using it does not involve designing our own SQL tables. If the course requires modeling a relational database, we will evaluate **Supabase with PostgreSQL**, including device authorization and query access control. This alternative has not been selected or configured. [Supabase database documentation](https://supabase.com/docs/guides/database/overview).

Before finalizing the decision, we will review the instructor's requirements, JavaScript access, permissions, current limits, and service terms. There is no need to create an account yet.

## 004 — Container and conversion to liters

**Status: criteria selected; container pending.** We prefer a wide opening and a constant internal cross-sectional area. We will measure the setup's usable capacity. If the cross-sectional area varies with height, we will use a calibrated height-to-volume relationship instead of automatically multiplying height percentage by nominal capacity.

## 005 — Learning and progress control

**Status: selected by the student.** Explain before writing code, work on one stage at a time, and investigate errors before moving forward. The student creates commits. Advanced improvements are deferred until the basic system has been tested.

## 006 — English documentation and manual Git commands

**Status: selected by the student.** All repository documentation, diagram labels, code comments, and commit messages will be written in English. Each suggested commit will include the exact commands for reviewing changes, staging files, creating the commit, and pushing it. The student runs these commands; the assistant does not create commits or push automatically.
