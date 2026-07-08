<div align="center">
<img width="1200" height="475" alt="GHBanner" src="https://ai.google.dev/static/site-assets/images/share-ais-513315318.png" />
</div>

# Alexandria

This repository holds two independent modules:

- **Web app** (repo root) — a Google AI Studio scaffold (Vite + TypeScript,
  Gemini API wiring). See below to run it locally.
- **[`unreal/AlexandriaLibrary/`](unreal/AlexandriaLibrary/README.md)** — an
  Unreal Engine 5.8 plugin that procedurally generates a walkable "Library of
  Alexandria" greybox level. See that module's own README for install and
  usage instructions.

## Run the web app locally

View your app in AI Studio: https://ai.studio/apps/bbf0cef3-70ec-42d8-99ee-9914dafe57c2

**Prerequisites:**  Node.js


1. Install dependencies:
   `npm install`
2. Set the `GEMINI_API_KEY` in [.env.local](.env.local) to your Gemini API key
3. Run the app:
   `npm run dev`
