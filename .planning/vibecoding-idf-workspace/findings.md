# Findings

## Source document

- File: `D:\IDF 环境搭建与 vibe coding.docx`.
- Workflow: install ESP-IDF using EIM offline package, install Cursor/VS Code, add Espressif MCP, install ESP-IDF extension, open/create a project, select target, then use status-bar build/flash/monitor actions.
- Existing EIM installation allows skipping the extension's own tool installation step.

## Current local state

- ESP-IDF v5.5.5 currently exists at `C:\Espressif\frameworks\esp-idf-v5.5.5`.
- The camera project currently exists independently at `C:\Users\heshizhe\Desktop\ESP32P4\p4_xcam_view`.
- No serial port was detected during the previous build session.
- PowerShell `Expand-Archive` rejected the `.docx` extension; use .NET ZIP APIs for read-only media extraction.
- Cursor has the official `espressif.esp-idf-extension` v2.1.0 installed.
- Extension v2.1.0 uses `idf.eimIdfJsonPath` (application scope) and `idf.currentSetup` (workspace scope) to select an EIM installation.
- The existing legacy installer metadata is `C:\Espressif\esp_idf.json` and `C:\Espressif\idf-env.json`; no `eim_idf.json` exists yet.
- The latest official EIM release found on 2026-07-22 is v0.17.1; its Windows CLI is about 22 MB.
