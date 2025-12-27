# Data Structures Demo

An interactive data structures learning project with a C/C++ backend and a React frontend. The backend exposes REST APIs for common data structures, while the frontend provides an animated UI to visualize operations like insert, remove, and clear.

---

## Features

- **Interactive visualizations** for:
  - Linked List
  - Stack
  - Queue
  - Sorted List
  - Circular List
  - Max Heap
  - Min Heap
  - Hash Table
  - Tree
  - Binary Search Tree (BST)
  - AVL Tree
- **REST API** built with CivetWeb (C) to perform data structure operations.
- **Animated UI** built with React + Tailwind + Framer Motion.
- **Parallelized initialization** of data structures and endpoint registration via a thread pool.

---

## Project Structure

```
.
├── Back End/
│   └── ds-demo-backend/
│       └── ds-demo-backend/          # C source for data structures + server
└── Front End/                        # React frontend
```

---

## Architecture Overview

### Backend (C / CivetWeb)
- **Server entry point**: `Back End/ds-demo-backend/ds-demo-backend/main.c`
  - Starts CivetWeb on **port 8080**
  - Registers endpoints
  - Includes an admin console menu for logs/history/cleanup
- **Routes**: `route_*.c` files
  - Each data structure has its own REST endpoints
- **Data structures**: `ds_*.c` files
- **Globals**: `route_globals.c` holds shared structure instances

### Frontend (React)
- **App entry**: `Front End/src/App.js`
  - Tabbed interface with animated transitions
- **Sections**: `Front End/src/components/*Section.jsx`
  - One per data structure
- **API client**: `Front End/src/services/api.jsx`
  - Fetch wrappers to backend endpoints

---

## Getting Started

### Backend

> The backend is a Visual Studio C/C++ solution (`.sln`) and uses CivetWeb.

1. Open `Back End/ds-demo-backend/ds-demo-backend.sln` in Visual Studio.
2. Build and run the solution.
3. The server will start at **http://localhost:8080**.

### Frontend

```bash
cd "Front End"
npm install
npm start
```

The app will open at **http://localhost:3000** and communicate with the backend at **http://localhost:8080**.

---

## API Overview (Examples)

Each data structure has endpoints like:

- **Linked List**
  - `GET /api/linked-list`
  - `POST /api/linked-list/add-front`
  - `POST /api/linked-list/add-end`
  - `POST /api/linked-list/add-by-index`
  - `DELETE /api/linked-list/remove-front`
  - `DELETE /api/linked-list/remove-end`
  - `DELETE /api/linked-list/remove-by-index`
  - `DELETE /api/linked-list/clear`

Similar routes exist for Stack, Queue, Tree, BST, AVL, Hash Table, Heaps, Sorted List, and Circular List.

---

## Notes

- The backend currently targets Windows (uses `windows.h` and `_CrtMemState`).
- Data structure operations are **server-driven**, so the UI reflects current backend state.

---

## License

Add a license if you plan to distribute or open-source this project.
