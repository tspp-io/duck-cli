# Duck CLI 🦆

The official package manager for the TSPP programming language.

## Overview

`duck` is a command-line tool for managing TSPP projects, dependencies, and builds. It uses `duck.json` as the package manifest.

## Features (Planned)

- **Init**: Create new TSPP projects (`duck init`).
- **Install**: Install dependencies (`duck install <package>`).
- **Build**: Build the project (`duck build`).
- **Run**: Run the project (`duck run`).
- **Publish**: Publish packages to the registry (`duck publish`).

## Manifest Format (`duck.json`)

```json
{
  "name": "my-package",
  "version": "0.1.0",
  "exports": {
    ".": "./src/main.tspp"
  },
  "dependencies": {
    "some-lib": "^1.0.0"
  }
}
```
