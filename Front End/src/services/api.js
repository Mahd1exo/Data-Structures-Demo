// src/services/api.js

const BASE_URL = "http://localhost:8080";

/* ---------- Linked List ---------- */
export async function fetchLinkedList() {
  const res = await fetch(`${BASE_URL}/api/linked-list`);
  return await res.json();
}

export async function addFrontLinkedList(value) {
  const res = await fetch(`${BASE_URL}/api/linked-list/add-front`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}

export async function addEndLinkedList(value) {
  const res = await fetch(`${BASE_URL}/api/linked-list/add-end`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}

export async function removeFrontLinkedList() {
  const res = await fetch(`${BASE_URL}/api/linked-list/remove-front`, {
    method: "DELETE",
  });
  return await res.json();
}

export async function removeEndLinkedList() {
  const res = await fetch(`${BASE_URL}/api/linked-list/remove-end`, {
    method: "DELETE",
  });
  return await res.json();
}
export async function removeByIndexLinkedList(index) {
  const res = await fetch(`${BASE_URL}/api/linked-list/remove-by-index/${index}`, {
    method: "DELETE",
  });
  return await res.json();
}
/* ---------- Stack ---------- */
export async function fetchStack() {
  const res = await fetch(`${BASE_URL}/api/stack`);
  return await res.json();
}

export async function pushStack(value) {
  const res = await fetch(`${BASE_URL}/api/stack/push`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}

export async function popStack() {
  const res = await fetch(`${BASE_URL}/api/stack/pop`, {
    method: "DELETE",
  });
  return await res.json();
}

/* ---------- Queue ---------- */
export async function fetchQueue() {
  const res = await fetch(`${BASE_URL}/api/queue`);
  return await res.json();
}

export async function enqueue(value) {
  const res = await fetch(`${BASE_URL}/api/queue/enqueue`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}

export async function dequeue() {
  const res = await fetch(`${BASE_URL}/api/queue/dequeue`, {
    method: "DELETE",
  });
  return await res.json();
}

/* ---------- Tree ---------- */
export async function fetchTree() {
  const res = await fetch(`${BASE_URL}/api/tree`);
  return await res.json();
}

export async function treeInsert(value) {
  const res = await fetch(`${BASE_URL}/api/tree/insert`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}
export async function treeRemove(id) {
  const res = await fetch(`${BASE_URL}/api/tree/remove`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ id }),
  });
  return await res.json();
}

export async function treeInsertChild(parent, value) {
  const res = await fetch(`${BASE_URL}/api/tree/insert-child`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ parent, value }),
  });
  return await res.json();
}

export async function treeRemoveChild(parent, id) {
  const res = await fetch(`${BASE_URL}/api/tree/remove-child`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ parent, id }),
  });
  return await res.json();
}
/* ---------- Heap ---------- */
export async function fetchHeap() {
  const res = await fetch(`${BASE_URL}/api/heap`);
  return await res.json();
}

export async function heapInsert(value) {
  const res = await fetch(`${BASE_URL}/api/heap/insert`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}

export async function heapExtractTop() {
  const res = await fetch(`${BASE_URL}/api/heap/extract-top`, {
    method: "DELETE",
  });
  return await res.json();
}
/* ---------- Hash Table ---------- */
export async function fetchHash() {
  const res = await fetch(`${BASE_URL}/api/hash`);
  return await res.json();
}

export async function hashInsert(key, value) {
  const res = await fetch(`${BASE_URL}/api/hash/insert`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ key, val: value }),
  });
  return await res.json();
}

export async function hashRemove(key) {
  const res = await fetch(`${BASE_URL}/api/hash/remove`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ key }),
  });
  return await res.json();
}
export async function hashContains(key) {
  const res = await fetch(`${BASE_URL}/api/hash/contains`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ key }),
  });
  return await res.json();
}
/* ---------- BST ---------- */
export async function fetchBST() {
  try {
    const res = await fetch(`${BASE_URL}/api/bst`);
    if (!res.ok) {
      console.error("HTTP error:", res.status, res.statusText);
      throw new Error("HTTP error " + res.status);
    }
    const data = await res.json();
    return data;
  } catch (err) {
    console.error("Error in fetchBST:", err);
    throw err;
  }
}


export async function bstInsert(value) {
  const res = await fetch(`${BASE_URL}/api/bst/insert`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}

export async function bstRemove(value) {
  const res = await fetch(`${BASE_URL}/api/bst/remove`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}