// src/components/HeapSection.js

import React, { useState, useEffect } from "react";

function HeapSection() {
  const [heap, setHeap] = useState([]);
  const [inputValue, setInputValue] = useState("");

  // Example fetch call
  async function fetchHeap() {
    try {
      const res = await fetch("/api/heap");
      const data = await res.json();
      // data => { "heap": ["root","child","child2"] }
      setHeap(data.heap || []);
    } catch (err) {
      console.error("Heap fetch error:", err);
    }
  }

  // Insert
  async function handleInsert() {
    if (!inputValue) return;
    try {
      await fetch("/api/heap/insert", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ value: inputValue }),
      });
      setInputValue("");
      fetchHeap();
    } catch (err) {
      console.error("Heap insert error:", err);
    }
  }

  // Extract top
  async function handleExtractTop() {
    try {
      await fetch("/api/heap/extract-top", {
        method: "POST"
      });
      fetchHeap();
    } catch (err) {
      console.error("Heap extract-top error:", err);
    }
  }

  useEffect(() => {
    fetchHeap();
  }, []);

  return (
    <div className="flex flex-col items-center">
      <h2 className="text-xl font-semibold mb-4">Heap (Min-Heap)</h2>

      <div className="flex items-center mb-4 gap-2">
        <input
          type="text"
          placeholder="Enter value"
          className="border border-gray-300 p-2 rounded"
          value={inputValue}
          onChange={(e) => setInputValue(e.target.value)}
        />
        <button className="bg-blue-500 text-white px-3 py-2 rounded" onClick={handleInsert}>
          Insert
        </button>
        <button className="bg-red-500 text-white px-3 py-2 rounded" onClick={handleExtractTop}>
          Extract Top
        </button>
        <button className="bg-gray-200 text-black px-3 py-2 rounded" onClick={fetchHeap}>
          Refresh
        </button>
      </div>

      <div className="text-sm text-gray-600 mb-2">
        <strong>Size:</strong> {heap.length}
      </div>

      {/* Display as a simple array */}
      <div className="flex flex-wrap gap-2">
        {heap.map((val, i) => (
          <div key={i} className="bg-blue-600 text-white px-4 py-2 rounded shadow">
            {val}
          </div>
        ))}
      </div>
    </div>
  );
}

export default HeapSection;
