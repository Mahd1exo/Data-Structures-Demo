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

export async function addByIndexLinkedList(index, value) {
  const res = await fetch(`${BASE_URL}/api/linked-list/add-by-index`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ index, value }),
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
  const res = await fetch(`${BASE_URL}/api/linked-list/remove-by-index`, {
    method: "DELETE",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ index }),
  });
  return await res.json();
}

export async function clearLinkedList() {
  const res = await fetch(`${BASE_URL}/api/linked-list/clear`, {
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

export async function clearStack() {
  const res = await fetch(`${BASE_URL}/api/stack/clear`, {
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

export async function clearQueue() {
  const res = await fetch(`${BASE_URL}/api/queue/clear`, {
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

export async function treeClear() {
  const res = await fetch(`${BASE_URL}/api/tree/clear`, {
    method: "DELETE",
  });
  return await res.json();
}

/* ---------- Heap (Max Heap) ---------- */
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

export async function heapClear() {
  const res = await fetch(`${BASE_URL}/api/heap/clear`, {
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

export async function clearHash() {
  const res = await fetch(`${BASE_URL}/api/hash/clear`, {
    method: "POST",
    headers: { "Content-Type": "application/json" }
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

export async function bstClear() {
  const res = await fetch(`${BASE_URL}/api/bst/clear`, {
    method: "POST",
    headers: { "Content-Type": "application/json" }
  });
  return await res.json();
}

/* ---------- AVL Tree ---------- */
export async function fetchAVL() {
  const res = await fetch(`${BASE_URL}/api/avl`);
  return await res.json();
}

export async function avlInsert(value) {
  const res = await fetch(`${BASE_URL}/api/avl/insert`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}

export async function avlRemove(value) {
  const res = await fetch(`${BASE_URL}/api/avl/remove`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}

export async function avlClear() {
  const res = await fetch(`${BASE_URL}/api/avl/clear`, {
    method: "POST",
    headers: { "Content-Type": "application/json" }
  });
  return await res.json();
}

/* ---------- Min Heap ---------- */
export async function fetchMinHeap() {
  const res = await fetch(`${BASE_URL}/api/minheap`);
  return await res.json();
}

export async function minheapInsert(value) {
  const res = await fetch(`${BASE_URL}/api/minheap/insert`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}

export async function minheapExtractMin() {
  const res = await fetch(`${BASE_URL}/api/minheap/extract-min`, {
    method: "DELETE",
  });
  return await res.json();
}

export async function minheapClear() {
  const res = await fetch(`${BASE_URL}/api/minheap/clear`, {
    method: "DELETE",
  });
  return await res.json();
}
/* ---------- Sorted List ---------- */
export async function fetchSortedList() {
  const res = await fetch(`${BASE_URL}/api/sorted-list`);
  return await res.json();
}

export async function sortedListInsert(value) {
  const res = await fetch(`${BASE_URL}/api/sorted-list/insert`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}

export async function sortedListRemove(value) {
  const res = await fetch(`${BASE_URL}/api/sorted-list/remove`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}

export async function sortedListClear() {
  const res = await fetch(`${BASE_URL}/api/sorted-list/clear`, {
    method: "DELETE",
  });
  return await res.json();
}

/* ---------- Circular List ---------- */
export async function fetchCircularList() {
  const res = await fetch(`${BASE_URL}/api/circular-list`);
  return await res.json();
}

// Default insert (inserts at end)
export async function circularListInsert(value) {
  const res = await fetch(`${BASE_URL}/api/circular-list/insert`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}
export async function circularListInsertFront(value) {
  const res = await fetch(`${BASE_URL}/api/circular-list/insert-front`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}

// Insert by index
export async function circularListInsertByIndex(index, value) {
  const res = await fetch(`${BASE_URL}/api/circular-list/insert-by-index`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ index, value }),
  });
  return await res.json();
}

// Remove by value (if needed)
export async function circularListRemove(value) {
  const res = await fetch(`${BASE_URL}/api/circular-list/remove`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ value }),
  });
  return await res.json();
}

// Remove by index
export async function circularListRemoveByIndex(index) {
  const res = await fetch(`${BASE_URL}/api/circular-list/remove-by-index`, {
    method: "DELETE",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ index }),
  });
  return await res.json();
}

// Remove front
export async function circularListRemoveFront() {
  const res = await fetch(`${BASE_URL}/api/circular-list/remove-front`, {
    method: "DELETE",
  });
  return await res.json();
}

// Remove end
export async function circularListRemoveEnd() {
  const res = await fetch(`${BASE_URL}/api/circular-list/remove-end`, {
    method: "DELETE",
  });
  return await res.json();
}

export async function circularListClear() {
  const res = await fetch(`${BASE_URL}/api/circular-list/clear`, {
    method: "DELETE",
  });
  return await res.json();
}
