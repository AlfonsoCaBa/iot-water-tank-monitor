# Stage 4 — Firebase Realtime Database

**Status: implementation prepared; ESP32 compilation and live writes pending.** Stage 3 connection and recovery were reported successful by the student. No live Firebase result has been observed by the assistant.

## Console setup

1. Create a project in [Firebase Console](https://console.firebase.google.com/). Google Analytics is optional; it is not needed here.
2. Under **Build > Realtime Database**, create a database in **locked mode**. Copy its root HTTPS URL from the Data tab; use Realtime Database, not Cloud Firestore.
3. Under **Build > Authentication > Sign-in method**, enable **Email/Password**.
4. Under **Authentication > Users > Add user**, create a dedicated device account. Keep its email/password locally and copy its **UID**.
5. Open [database.rules.json](../firebase/database.rules.json), replace all three `REPLACE_WITH_DEVICE_UID` placeholders with that UID, and paste the JSON into **Realtime Database > Rules > Publish**. The UID is an identifier, not a password. These rules permit this account to read its own device, update `latest`, and append validated history records. Other access is denied.
6. Under **Project settings > General**, copy the **Web API Key**. If it is missing, register a Web app with the `</>` button and take `apiKey` from its configuration. No JavaScript SDK installation or Hosting setup is needed for this stage.
7. Fill the four `FIREBASE_...` constants in the existing ignored `firmware/water_tank_monitor/secrets.h`. Preserve its Wi-Fi credentials. Use the database root URL, not the Console page URL or a path ending in `.json`. Do not use a service-account private key.

The Firebase API key identifies the project; the device signs in to obtain an expiring ID token. Database rules authorize that authenticated user's writes. See [REST authentication](https://firebase.google.com/docs/database/rest/auth) and [email/password sign-in](https://firebase.google.com/docs/reference/rest/auth#section-sign-in-email-password).

## Data and operation

Every **20 seconds after a completed attempt**, the firmware selects a valid filtered result no older than five seconds. A single authenticated HTTPS PATCH updates:

```text
devices/
  <device UID>/
    latest/                  # Most recently stored sample
    history/
      <measurement epoch ms>/ # One sample per accepted upload
```

Each sample contains `distanceCm`, `liters`, `fillPercent`, `levelPercent`, `endpointClamped`, `measuredAt` (device UTC epoch milliseconds), and `receivedAt` (Firebase server epoch milliseconds). Height and volume percentages remain distinct. A successful silent write returns HTTP **204**. [Firebase REST writes and server timestamps](https://firebase.google.com/docs/database/rest/save-data).

HTTPS runs in a FreeRTOS background task, separate from the sensor loop. A one-slot queue keeps the latest reading; invalid measurements invalidate it. Offline samples are overwritten rather than buffered for later replay. A timeout after server acceptance can leave the outcome unknown; exactly-once delivery is not claimed.

NTP supplies UTC for TLS certificate verification and measurement timestamps. ESP32 core **4.0.0** includes the CA bundle and `cJSON` used here; no extra library installation is required. Tokens remain in RAM and the firmware signs in again before expiry. Authentication failures wait at least 60 seconds before retrying. Credentials, tokens, request URLs, and server bodies are not logged.

## Compile and check

Open `water_tank_monitor.ino` with its headers in the same folder, compile/upload, and open Serial Monitor at **115200 baud**. Wait for clock synchronization, `Firebase authenticated`, and `Cloud stored | Firebase history ID: ...`. In **Realtime Database > Data**, check `devices/<UID>/latest` and two successive history entries.

- Auth HTTP 400: check the project API key, enabled provider, and dedicated account credentials.
- Upload HTTP 401: check that all three rule placeholders match the authenticated UID and that the rules were published. Expired/invalid tokens also cause reauthentication.
- Upload HTTP 404: check the copied database URL and that the database exists.
- Negative HTTP status: check hotspot internet, NTP, TLS connectivity, and memory.

The computer-side protocol tests cover response acceptance, URL validation, freshness, upload spacing, and `millis()` rollover. They do not verify Firebase rules on the server, Arduino compilation, or physical behavior. The student performs compilation and live checks.

Stage 5 will add a dashboard and its reader authorization. Do not put the device password in browser code.
