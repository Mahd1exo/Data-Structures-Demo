// src/components/MinHeapSection.js
import React, { useState, useEffect } from "react";
import { 
  fetchMinHeap, 
  minheapInsert, 
  minheapExtractMin, 
  minheapClear  // imported clear method for min heap
} from "../services/api";
import MinHeapNode from "./MinHeapNode";
import { AnimatePresence, motion } from "framer-motion";
import { FaPlus, FaTrash, FaSync, FaInfoCircle } from "react-icons/fa";

function MinHeapSection() {
  const [heap, setHeap] = useState([]);
  const [inputValue, setInputValue] = useState("");
  const [errorMessage, setErrorMessage] = useState("");
  const [showInfoModal, setShowInfoModal] = useState(false);

  useEffect(() => {
    refreshHeap();
  }, []);

  async function refreshHeap() {
    try {
      const res = await fetchMinHeap();
      setHeap(res.heap || []);
      setErrorMessage("");
    } catch (err) {
      console.error("Error fetching min heap:", err);
      setErrorMessage("Error fetching min heap data.");
    }
  }

  // Validate that the input is a valid integer (allows negatives)
  function isValidInt(str) {
    return /^-?\d+$/.test(str);
  }

  async function handleInsert() {
    const trimmed = inputValue.trim();
    if (!trimmed) {
      setErrorMessage("Please enter a value.");
      return;
    }
    if (!isValidInt(trimmed)) {
      setErrorMessage("Input must be a valid integer.");
      return;
    }
    setErrorMessage("");
    try {
      const res = await minheapInsert(trimmed);
      if (res.error) {
        setErrorMessage(res.error);
      } else {
        setInputValue("");
      }
      refreshHeap();
    } catch (err) {
      console.error("Min heap insert error:", err);
      setErrorMessage("Error inserting into min heap.");
    }
  }

  async function handleExtractMin() {
    if (heap.length === 0) {
      setErrorMessage("Heap is empty. Cannot extract min.");
      return;
    }
    setErrorMessage("");
    try {
      const res = await minheapExtractMin();
      if (res.error) {
        setErrorMessage(res.error);
      }
      refreshHeap();
    } catch (err) {
      console.error("Min heap extract error:", err);
      setErrorMessage("Error extracting min element.");
    }
  }

  async function handleClear() {
    setErrorMessage("");
    try {
      const res = await minheapClear();
      if (res.error) {
        setErrorMessage(res.error);
      }
      setInputValue("");
      refreshHeap();
    } catch (err) {
      console.error("Min heap clear error:", err);
      setErrorMessage("Error clearing min heap.");
    }
  }

  function toggleInfoModal() {
    setShowInfoModal((prev) => !prev);
  }

  return (
    <div className="p-6">
      {/* Header */}
      <div className="flex justify-between items-center mb-4">
        <h2 className="text-3xl font-bold">Min Heap Manager</h2>
        <button
          onClick={toggleInfoModal}
          className="flex items-center gap-1 text-gray-600 hover:text-gray-800 transition-colors duration-300"
        >
          <FaInfoCircle className="text-2xl" />
        </button>
      </div>

      {/* Error Message */}
      {errorMessage && (
        <div className="mb-4 p-2 border border-red-300 rounded text-red-600 text-sm">
          {errorMessage}
        </div>
      )}

      {/* Info Modal */}
      <AnimatePresence>
        {showInfoModal && (
          <motion.div
            className="fixed inset-0 flex items-center justify-center z-50"
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            exit={{ opacity: 0 }}
          >
            <div
              className="absolute inset-0 bg-black opacity-40"
              onClick={toggleInfoModal}
            ></div>
            <motion.div
              className="relative bg-white rounded-xl p-6 max-w-xl mx-auto z-10 shadow-2xl"
              initial={{ scale: 0.8, y: -50 }}
              animate={{ scale: 1, y: 0 }}
              exit={{ scale: 0.8, y: 50 }}
              transition={{ duration: 0.3 }}
            >
              <h3 className="text-2xl font-bold mb-4 text-gray-800">
                About Min Heap
              </h3>
              <p className="text-gray-700 text-base leading-relaxed mb-4">
                A <strong>Min Heap</strong> is a complete binary tree where every node is
                smaller than or equal to its children. The root holds the minimum value.
              </p>
              <button
                onClick={toggleInfoModal}
                className="bg-indigo-600 hover:bg-indigo-700 text-white px-4 py-2 rounded transition-colors duration-300"
              >
                Close
              </button>
            </motion.div>
          </motion.div>
        )}
      </AnimatePresence>

      {/* Control Panel */}
      <div className="mb-4 p-4 border rounded-md shadow-sm bg-white flex items-center gap-4">
        <input
          type="text"
          placeholder="Enter value"
          className="flex-1 border border-gray-300 p-2 rounded focus:outline-none focus:ring focus:ring-blue-300"
          value={inputValue}
          onChange={(e) => setInputValue(e.target.value)}
        />
        <button
          className="bg-blue-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-blue-600 transition-colors duration-300"
          onClick={handleInsert}
        >
          <FaPlus />
          Insert
        </button>
        <button
          className="bg-red-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-red-600 transition-colors duration-300"
          onClick={handleExtractMin}
        >
          <FaTrash />
          Extract Min
        </button>
        <button
          className="bg-gray-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-gray-600 transition-colors duration-300"
          onClick={refreshHeap}
        >
          <FaSync className="animate-spin" />
          Refresh
        </button>
        <button
          className="bg-gray-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-gray-600 transition-colors duration-300"
          onClick={handleClear}
        >
          <FaTrash className="rotate-180" />
          Clear
        </button>
      </div>

      {/* Array Representation */}
      <div className="mb-4">
        <h3 className="text-xl font-semibold text-gray-800 mb-2">
          Array Representation
        </h3>
        <div className="flex flex-wrap gap-4 justify-center">
          {heap.map((item, index) => (
            <div key={index} className="flex flex-col items-center">
              <div className="text-sm text-gray-500">Index: {index}</div>
              <div className="w-16 h-16 flex items-center justify-center bg-white border border-gray-300 rounded shadow">
                {item}
              </div>
            </div>
          ))}
        </div>
      </div>

      {/* Min Heap Display */}
      <div className="mb-4 text-sm text-gray-600">
        <strong>Min Heap Structure:</strong>
      </div>
      <div className="relative border rounded bg-gray-50 shadow-sm p-4 overflow-auto min-h-[400px]">
        {heap.length > 0 ? (
          <MinHeapNode heap={heap} index={0} level={0} />
        ) : (
          <p className="text-gray-500">Min heap is empty.</p>
        )}
      </div>
    </div>
  );
}

export default MinHeapSection;
